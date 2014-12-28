/*
    Copyright 2007-2008 Fela Winkelmolen <fela.kde@gmail.com> 
  
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

#include "gameengine.h"

#include "levelloader.h"

#include <QTimer>
#include <QTime>

GameEngine::GameEngine(QObject *parent)
    : QObject(parent)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    m_levelLoader = new LevelLoader( this );
    connect(m_levelLoader, &LevelLoader::newLine, this, &GameEngine::newLine);
    connect(m_levelLoader, &LevelLoader::newGift, this, &GameEngine::newGift);
}

GameEngine::~GameEngine()
{
    deleteAllObjects();
    delete m_levelLoader;
}

void GameEngine::start(const QString& l)
{
    m_levelLoader->setLevelset(l);
    m_levelLoader->setLevel(0);

    emit loadingNewGame();
    
    loadLevel();
}

void GameEngine::loadLevel()
{
    deleteAllObjects();
    
    m_levelLoader->loadLevel();
    
    emit ready();
}

void GameEngine::loadNextLevel()
{
    QTimer::singleShot(200, this, SLOT(loadLevel()));
}

inline void GameEngine::deleteAllObjects()
{
    /*kDebug() << "all object deleted";
    deleteMovingObjects();
    qDeleteAll(m_bricks);
    m_bricks.clear();
    qDeleteAll(m_gifts);
    m_gifts.clear();*/
}
