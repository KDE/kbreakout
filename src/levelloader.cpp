/*
    Copyright 2011 Julian Helfferich <julian.helfferich@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "levelloader.h"
#include "globals.h"
#include "kbreakout_debug.h"

#include <QDomDocument>
#include <QFile>
#include <QStandardPaths>

// These can be removed when KConfig style levelsets are no longer supported
#include <KConfig>
#include <KConfigGroup>

LevelLoader::LevelLoader(QObject *parent)
    : QObject(parent)
{
    m_level = 0;
    m_levelset = nullptr;
}

LevelLoader::~LevelLoader()
{
    delete m_levelset;
}

int LevelLoader::level() const
{
    return m_level;
}

void LevelLoader::setLevel(int level)
{
    m_level = level;
}

QString LevelLoader::levelset() const
{
    return m_levelname;
}

void LevelLoader::setLevelset(const QString &levelname)
{
    if (levelname == m_levelname) {
        return;
    }
    m_levelname = levelname;

    // Loading document model
    // Locating the path in the filesystem
    QString path = QStringLiteral("levelsets/%1.levelset").arg(m_levelname);
    path = QStandardPaths::locate(QStandardPaths::AppDataLocation, path);
    // --

    delete m_levelset;

    m_levelset = new QDomDocument(m_levelname);
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qCCritical(KBREAKOUT_General) << "Can't open file " << path << endl;
    }

    QString errorString;
    int errorLine;
    int errorColumn;
    if (!m_levelset->setContent(&file, false, &errorString, &errorLine, &errorColumn)) {
        file.close();
        // Testing whether levelset is of old KConfig style
        KConfig kconfigfile(path, KConfig::SimpleConfig);
        if (kconfigfile.hasGroup(QStringLiteral("level1"))) {
            // Levelset is in KConfig style
            m_oldstyle = true;
            qCCritical(KBREAKOUT_General) << "Warning: Using deprecated KConfig-levelset. Please change to XML-Style.\n";
        } else {
            qCCritical(KBREAKOUT_General) << "Can't read levelset from " << path << "\nError: " << errorString <<
                        " in Line " << errorLine << ", Column " << errorColumn << endl;
        }
    } else {
        // Successfully loaded QDom-style levelset
        m_oldstyle = false;
    }
    file.close();
    // --
}

void LevelLoader::loadLevel()
{
    // Check if levelset is of KConfig-type
    if (m_oldstyle) {
        loadOldStyleLevel();
        return;
    }
    // Selecting the correct level
    m_level++;

    if (m_levelset == nullptr) {
        qCCritical(KBREAKOUT_General) << "Error: No levelset specified" << endl;
        return;
    }

    QDomElement levels = m_levelset->documentElement();
    QDomNode node = levels.firstChild();
    for (int i = 1; i < m_level; i++) {
        node = node.nextSibling();
    }
    // --

    // Load level information
    if (node.isNull() || node.toElement().tagName() != QLatin1String("Level")) {
        // Level not found or no more levels
        return;
    }

    QDomAttr attribute;
    QDomElement level = node.toElement();
    if (level.isNull()) {
        qCCritical(KBREAKOUT_General) << "Invalid Levelset " << m_levelname << ": Can't read level information";
    }
    attribute = level.attributeNode(QStringLiteral("Name"));
    QString levelName;
    if (!attribute.isNull()) {
        levelName = level.attributeNode(QStringLiteral("Name")).value();
    }
    node = node.firstChild();
    // --

    // Load bricks and gifts
    m_lineNumber = 0;
    while (!node.isNull()) {
        QDomElement info = node.toElement();
        if (info.isNull()) {
            qCCritical(KBREAKOUT_General) << "Invalid levelset " << m_levelname << ": Can't read level information.";
        }

        if (info.tagName() == QLatin1String("Line")) {
            // Load one line of bricks
            loadLine(info);
        } else if (info.tagName() == QLatin1String("Gift")) {
            // Load one gift type
            loadGift(info);
        } else {
            qCCritical(KBREAKOUT_General) << "Invalid tag name " << info.tagName() << " has occurred in level "
                        << levelName << " in levelset " << m_levelname << endl;
        }

        node = node.nextSibling();
    }
}

void LevelLoader::loadLine(QDomElement lineNode)
{
    // Reading the line number
    QDomAttr attribute = lineNode.attributeNode(QStringLiteral("Number"));
    QDomElement attributeNode = lineNode.firstChildElement(QStringLiteral("Number"));
    if (!attribute.isNull()) {
        m_lineNumber = attribute.value().toInt();
    } else if (!attributeNode.isNull()) {
        m_lineNumber = attributeNode.text().toInt();
    } else {
        // Standard line numbering: load next line
        m_lineNumber++;
    }

    // Reading the brick information
    attribute = lineNode.attributeNode(QStringLiteral("Bricks"));
    attributeNode = lineNode.firstChildElement(QStringLiteral("Bricks"));
    QString line;
    if (!attribute.isNull()) {
        line = attribute.value();
    } else if (!attributeNode.isNull()) {
        line = attributeNode.text();
    } else {
        line = lineNode.text();
    }

    if (line.size() > WIDTH) {
        qCCritical(KBREAKOUT_General) << "Invalid levelset " << m_levelname << ": too many bricks in line "
                    << m_lineNumber << endl;
    }

    emit newLine(line, m_lineNumber);
}

void LevelLoader::loadGift(QDomElement giftNode)
{
    bool nodeTextRead = false;
    // Reading the brick type
    QDomAttr attribute = giftNode.attributeNode(QStringLiteral("Type"));
    QDomElement attributeNode = giftNode.firstChildElement(QStringLiteral("Type"));
    QString giftType;
    if (!attribute.isNull()) {
        giftType = attribute.value();
    } else if (!attributeNode.isNull()) {
        giftType = attributeNode.text();
        nodeTextRead = true;
    } else {
        giftType = giftNode.text();
        nodeTextRead = true;
    }

    // Reading number of gifts to be distributed. If not specified one gift is placed.
    attribute = giftNode.attributeNode(QStringLiteral("Count"));
    attributeNode = giftNode.firstChildElement(QStringLiteral("Count"));
    int times = 1;
    bool ok = true;
    if (!attribute.isNull()) {
        times = attribute.value().toInt(&ok);
    } else if (!attributeNode.isNull()) {
        times = attributeNode.text().toInt(&ok);
        nodeTextRead = true;
    } else if (!nodeTextRead) {
        times = giftNode.text().toInt(&ok);
        if (!ok) {
            times = 1;
        }
    }

    // If only one brick to be placed: see if position is given
    QString position;
    if (times == 1) {
        attribute = giftNode.attributeNode(QStringLiteral("Position"));
        attributeNode = giftNode.firstChildElement(QStringLiteral("Position"));
        if (!attribute.isNull()) {
            position = attribute.value();
        } else if (!attributeNode.isNull()) {
            position = attributeNode.text();
            nodeTextRead = true;
        } else if (!nodeTextRead && giftNode.text().contains(QLatin1Char(','))) {
            position = giftNode.text();
            nodeTextRead = true;
        }
    }

    emit newGift(giftType, times, position);
}

void LevelLoader::loadOldStyleLevel()
{
    // Selecting the correct level
    m_level++;

    // Loading the levelset
    QString path = QStringLiteral("levelsets/%1.levelset").arg(m_levelname);
    path = QStandardPaths::locate(QStandardPaths::AppDataLocation, path);
    KConfig file(path, KConfig::SimpleConfig);

    QString levelName(QLatin1String("level") + QString::number(m_level));

    if (!file.hasGroup(levelName)) {
        // No more levels or no levels found
        return;
    }

    // Loading level information
    KConfigGroup lvl = file.group(levelName);

    // add bricks

    int y = 1;
    QString key(QLatin1String("line") + QString::number(y));

    while (lvl.hasKey(key)) {
        // one line of bricks to be converted
        QString line = lvl.readEntry(key, "error");
        if (line == QLatin1String("error")) {
            qCCritical(KBREAKOUT_General) << "Something strange happened!!\n";
            return;
        }

        qCDebug(KBREAKOUT_General) << line << endl;

        if (line.size() > WIDTH) {
            qCCritical(KBREAKOUT_General) << "Invalid file: too many bricks\n";
        }

        emit newLine(line, y);

        ++y;
        key = QLatin1String("line") + QString::number(y);
    }

    // add gifts

    for (int i = 0; i < GIFT_TYPES_COUNT; ++i) {
        key = GIFT_TYPES[i];
        if (!lvl.hasKey(key)) {
            continue;
        }

        QString line = lvl.readEntry(key, "error");
        if (line == QLatin1String("error")) {
            qCCritical(KBREAKOUT_General) << "Impossible reading " << m_level << ":" << key << endl;
            return;
        }
        bool ok;
        int times = line.toInt(&ok);
        if (!ok) {
            qCCritical(KBREAKOUT_General) << m_levelname << ":" << key << " invalid number!!" << endl;
            continue;
        }

        emit newGift(key, times, QString());
    }
}
