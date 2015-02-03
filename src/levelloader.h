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
    LevelLoader(QObject *parent = 0);
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
    QLatin1Literal("Gift100Points"),
    QLatin1Literal("Gift200Points"),
    QLatin1Literal("Gift500Points"),
    QLatin1Literal("Gift1000Points"),
    QLatin1Literal("GiftAddLife"),

    QLatin1Literal("GiftLoseLife"),
    QLatin1Literal("GiftNextLevel"),
    QLatin1Literal("GiftSplitBall"),
    QLatin1Literal("GiftAddBall"),
    QLatin1Literal("GiftDecreaseSpeed"),

    QLatin1Literal("GiftIncreaseSpeed"),
    QLatin1Literal("GiftUnstoppableBall"),
    QLatin1Literal("GiftBurningBall"),
    QLatin1Literal("GiftMagicWand"),
    QLatin1Literal("GiftMagicEye"),

    QLatin1Literal("GiftEnlargeBar"),
    QLatin1Literal("GiftShrinkBar"),
    QLatin1Literal("GiftStickyBar"),
    QLatin1Literal("GiftMoreExplosion")
};

// IMPORTANT: keep in sync with the above
const int GIFT_TYPES_COUNT = 19;

#endif // LEVELLOADER_H
