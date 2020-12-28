/*
    SPDX-FileCopyrightText: 20011 Julian Helfferich <julian.helfferich@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
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
    explicit LevelLoader(QObject *parent = nullptr);
    ~LevelLoader();

    int level() const;
    void setLevel(int level);
    QString levelset() const;
    void setLevelset(const QString &levelname);

    void loadLevel();

Q_SIGNALS:
    void newLine(const QString &line, int lineNumber);
    void newGift(const QString &giftType, int times, const QString &position);

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
