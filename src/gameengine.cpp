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

#include "mainwindow.h"
#include "levelloader.h"
#include "settings.h"
#include "globals.h"

#include <cmath>
#include <math.h>

#include <QTimer>
#include <QAction>

#include <KLocale>
#include <KActionCollection>
#include <KDebug>

GameEngine::GameEngine(MainWindow *mainWindow)
    : m_mainWindow(mainWindow)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    m_levelLoader = new LevelLoader( this );
    connect(m_levelLoader, SIGNAL(newLine(QString,int)), this, SIGNAL(newLine(QString,int)));
    connect(m_levelLoader, SIGNAL(newGift(QString,int,QString)), this, SIGNAL(newGift(QString,int,QString)));
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

    m_gameOver = false;
    m_gameWon = false;
    m_level = 1;

    emit loadingNewGame();
    
    loadLevel();
}

void GameEngine::pause()
{
    /*if (gameIsPaused()) {
        return;
    }
    if (m_gameWon || m_gameOver) {
        return;
    }
    m_elapsedTimeTimer.stop();
    m_gameTimer.stop();
    emit gamePaused();*/
}

void GameEngine::resume()
{
    /*if (!gameIsPaused()) {
        return;
    }
    m_elapsedTimeTimer.start();
    m_gameTimer.start();

    emit gameResumed();*/
}

void GameEngine::loadLevel()
{
    deleteAllObjects();
    m_remainingBricks = 0;
    
    m_levelLoader->loadLevel();
    /*if (m_bricks.isEmpty()) {
        if (m_levelLoader->level() == 1) {
            // No level in the levelset
            kError() << "Invalid levelset " << m_levelLoader->levelset() << endl;
        } else {
            // No more levels: game won
            m_gameWon = true;
            kDebug() << m_score;
            addScore( GAME_WON_SCORE + m_lives.size() * LIFE_SCORE );
            kDebug() << m_lives.size() << m_score;
            showMessage( i18n("Well done! You won the game") );
            emit gameEnded(m_score, -1, m_elapsedTime);
            deleteMovingObjects();
        }
    }*/
    
    //m_speed = 1.8;
    //m_repaintInterval = 1;
    //m_levelInfo.setLevel(m_level);
    //showMessage(i18n("Level %1", m_level));
    //QTimer::singleShot(2000, this, SLOT(hideMessage()));
    
    emit ready();
}

//======= convenience functions =================//

void GameEngine::showFireBallMessage()
{
    if (Settings::fireOnClick()) {
        return; // don't show message
    }
    QAction *fireAction = m_mainWindow->actionCollection()->action("fire");
    QString shortcut = fireAction->shortcut().toString(QKeySequence::NativeText);
    //showInfoMessage(i18n("Press %1 to fire the ball", shortcut));
}

void GameEngine::hideMessage()
{
    /*if (gameIsPaused()) {
        return;
    }
    if (m_gameWon && m_gameOver) {
        return;
    }*/
    
    // else
    //m_messageBox.hide();
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
