/*
    SPDX-FileCopyrightText: 2007-2008 Fela Winkelmolen <fela.kde@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "gameengine.h"

// own
#include "kbreakout_debug.h"
#include "levelloader.h"
// Qt
#include <QTimer>
#include <QTime>

GameEngine::GameEngine(QObject *parent)
    : QObject(parent)
{
    m_levelLoader = new LevelLoader(this);
    connect(m_levelLoader, &LevelLoader::newLine, this, &GameEngine::newLine);
    connect(m_levelLoader, &LevelLoader::newGift, this, &GameEngine::newGift);
}

GameEngine::~GameEngine()
{
    deleteAllObjects();
    delete m_levelLoader;
}

void GameEngine::start(const QString &l)
{
    m_levelLoader->setLevelset(l);
    m_levelLoader->setLevel(0);

    Q_EMIT loadingNewGame();

    loadLevel();
}

void GameEngine::loadLevel()
{
    deleteAllObjects();

    m_levelLoader->loadLevel();

    Q_EMIT ready();
}

void GameEngine::loadNextLevel()
{
    QTimer::singleShot(200, this, &GameEngine::loadLevel);
}

inline void GameEngine::deleteAllObjects()
{
    /*qCDebug(KBREAKOUT_General) << "all object deleted";
    deleteMovingObjects();
    qDeleteAll(m_bricks);
    m_bricks.clear();
    qDeleteAll(m_gifts);
    m_gifts.clear();*/
}
