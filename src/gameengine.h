/*
    SPDX-FileCopyrightText: 2007-2008 Fela Winkelmolen <fela.kde@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QObject>

class LevelLoader;

class GameEngine : public QObject
{
    Q_OBJECT
public:
    explicit GameEngine(QObject *parent = nullptr);
    ~GameEngine();

public Q_SLOTS:
    void start(const QString &levelset);
    void loadNextLevel();

Q_SIGNALS:
    void loadingNewGame();
    void newLine(const QString &line, int lineNumber);
    void newGift(const QString &gift, int times, const QString &pos);
    void ready();

private:
    void deleteAllObjects();

    LevelLoader *m_levelLoader;

private Q_SLOTS:
    void loadLevel();
};

#endif // GAMEENGINE_H
