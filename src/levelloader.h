/*
    Copyright 20011 Julian Helfferich <julian.helfferich@gmail.com>

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

#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <QObject>

class QDomDocument;
class QDomElement;

class LevelLoader : public QObject
{
    Q_OBJECT
public:
    explicit LevelLoader(QObject *parent = 0);
    ~LevelLoader();

    int level() const;
    void setLevel(int level);
    QString levelset() const;
    void setLevelset(const QString &levelname);

    void loadLevel();

signals:
    void newLine(QString line, int lineNumber);
    void newGift(QString giftType, int times, QString position);

private:
    void loadLine(QDomElement lineNode);
    void loadGift(QDomElement giftNode);

    void loadOldStyleLevel();

    QDomDocument *m_levelset;
    bool m_oldstyle;

    int m_lineNumber;
    QString m_levelname;
    int m_level;
};

// Global list of gift types
// Can be removed, when KConfig levelsets are no longer supported

const QString GIFT_TYPES[] = {
    QStringLiteral("Gift100Points"),
    QStringLiteral("Gift200Points"),
    QStringLiteral("Gift500Points"),
    QStringLiteral("Gift1000Points"),
    QStringLiteral("GiftAddLife"),

    QStringLiteral("GiftLoseLife"),
    QStringLiteral("GiftNextLevel"),
    QStringLiteral("GiftSplitBall"),
    QStringLiteral("GiftAddBall"),
    QStringLiteral("GiftDecreaseSpeed"),

    QStringLiteral("GiftIncreaseSpeed"),
    QStringLiteral("GiftUnstoppableBall"),
    QStringLiteral("GiftBurningBall"),
    QStringLiteral("GiftMagicWand"),
    QStringLiteral("GiftMagicEye"),

    QStringLiteral("GiftEnlargeBar"),
    QStringLiteral("GiftShrinkBar"),
    QStringLiteral("GiftStickyBar"),
    QStringLiteral("GiftMoreExplosion")
};

// IMPORTANT: keep in sync with the above
const int GIFT_TYPES_COUNT = 19;

#endif // LEVELLOADER_H
