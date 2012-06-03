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

// static
//Bar *GameEngine::m_bar_ptr = 0;


GameEngine::GameEngine(MainWindow *mainWindow)
    : m_mainWindow(mainWindow)
{
    m_cheatsEnabled = !qgetenv("KDE_DEBUG").isEmpty();

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
    setScore(0);
    
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

void GameEngine::cheatSkipLevel() 
{
    if (m_cheatsEnabled) {
        loadNextLevel();
    }
}

void GameEngine::cheatAddLife() 
{
    if (m_cheatsEnabled) {
        //m_lives.append(new Life);
    }
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

void GameEngine::detectBallCollisions(/*Ball *ball*/)
{
    // never run this function more than two time recursively
    /*static bool firstTime = true;
    QRect rect = ball->getRect();
    
    // bounce a little early in some cases so the avarage position is centered
    rect.translate(qRound(ball->directionX/2), qRound(ball->directionY/2));
    
    // bounce against the wall
    if (rect.left() < 0 && ball->directionX < 0) {
        ball->directionX *= -1;
    } else if (rect.right() + 1 > BRICK_WIDTH * WIDTH
             && ball->directionX > 0) {
        ball->directionX *= -1;
    } else if (rect.top() < 0 && ball->directionY < 0) {
        ball->directionY *= -1;
    } else if (rect.bottom()+1 > ( HEIGHT) * BRICK_HEIGHT 
             && ball->directionY > 0) {
        // delete the ball
        m_balls.removeAll(ball);
        delete ball;
        m_itemsGotDeleted = true;
        if (m_balls.isEmpty()) {
            addScore(LOSE_LIFE_SCORE);
            
            showMessage(i18n("Oops! You have lost the ball!"));
            QTimer::singleShot(1000, this, SLOT(handleDeath()));
            m_gameTimer.stop();
        
            deleteMovingObjects();
        }
        return;
    }
    // bounce against the bar
    else if (m_bar.getRect().intersects(rect) && ball->directionY > 0) {
        
        qreal ballCenter = ball->getRect().left() + (ball->getRect().width())/2;
        qreal barCenter  = m_bar.center();
        
        if (ballCenter > m_bar.getRect().left() &&
                ballCenter < m_bar.getRect().right()) {
            // the bar has been hit
            
            if (m_bar.type() == "StickyBar") {
                ball->toBeFired = true;
                
                qreal diff = ball->getRect().left() - m_bar.getRect().left();
                
                ball->barPosition = diff / m_bar.getRect().width();
                updateAttachedBalls();
            }
            
            qreal angle = M_PI / 3;
            angle *= (barCenter - ballCenter) / (m_bar.getRect().width()/2);
            angle += M_PI_2;
            
            qreal speed = sqrt(pow(ball->directionX, 2) + 
                           pow(ball->directionY, 2));
            if (angle > 0 && angle < M_PI) {
                ball->directionX =  cos(angle) * speed;
                ball->directionY = -sin(angle) * speed;
            }
        }
    } else { // bounce against the bricks (and optionally break them)
        handleBrickCollisions(ball);
    }
    
    // never run this function more than two time recursively
    if (firstTime) {
        firstTime = false;
        // check if there is another collision
        if ( ! m_itemsGotDeleted) {
            detectBallCollisions(ball);
        }
    } else {
        firstTime = true;
        return;
    }*/
}

void GameEngine::handleDeath()
{
    /*hideMessage();
    deleteMovingObjects();
    m_bar.reset();
    if (m_lives.isEmpty()) {
        m_gameOver = true;
        showMessage(i18n("Game Over!"));
        emit gameEnded(m_score, m_level, m_elapsedTime);
    } else {
        delete m_lives.takeLast();
        // TODO: put following in a convenience function 
        // (called also when a new level is loaded..)
        Ball *ball = new Ball;
        m_balls.append(ball);
        m_gameTimer.setInterval(REPAINT_INTERVAL);
        m_repaintInterval = 1;
        m_speed = 1.8;
        m_gameTimer.start();
        updateAttachedBalls();
        showFireBallMessage();
    }*/
}


void GameEngine::handleBrickCollisions(/*Ball *ball*/)
{
    /*if (m_itemsGotDeleted) {
        return;
    }
    QRect rect = ball->getRect();

    QList<Brick *> bricksIntersecting;
    foreach (Brick *brick, m_bricks) {
        if (brick->isDeleted()) {
            continue;
        }
        QRect brickRect = brick->getRect();
        
        if (brickRect.intersects(rect)) {
            bricksIntersecting.append(brick);
        }
    }
    
    if (!bricksIntersecting.isEmpty()) {
        ball->collideWithBricks(bricksIntersecting);
    }*/
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
    // assign points for each remaining brick
    /*foreach (Brick *brick, m_bricks) {
        // don't assign points for Unbreakable Bricks
        if (brick->type() == "UnbreakableBrick") {
            continue;
        }
        if (brick->isDeleted()) {
            continue;
        }
        addScore(AUTOBRICK_SCORE);
        
        // add extra points for Multiple Bricks
        if (brick->type() == "MultipleBrick3") {
            addScore(AUTOBRICK_SCORE*2);
        }
        if (brick->type() == "MultipleBrick2") {
            addScore(AUTOBRICK_SCORE);
        }
    }
    ++m_level;
    deleteMovingObjects();
    QTimer::singleShot(200, this, SLOT(loadLevel()));
    addScore(LEVEL_SCORE);*/
}

void GameEngine::addScore(int points)
{
    m_score += points;
    //m_scoreCanvas.setScore(m_score);
}

void GameEngine::setScore(int newScore)
{
    m_score = newScore;
    //m_scoreCanvas.setScore(m_score);
}

void GameEngine::updateAttachedBalls()
{
    /*foreach (Ball *ball, m_balls) {
        if (!ball->toBeFired) {
            continue;
        }
        // else
        int ballX = m_bar.getRect().left() + 
                qRound(ball->barPosition * m_bar.getRect().width());
        ball->moveTo(ballX, m_bar.getRect().top() - BALL_SIZE);
        ball->repaint();
    }*/
}

// TODO: check (in debugger?) why this function is called so much...
inline void GameEngine::deleteMovingObjects()
{
    /*kDebug() << "Deleting objects...\n";
    m_itemsGotDeleted = true;
    qDeleteAll(m_balls);
    m_balls.clear();
    
    QMutableListIterator<Gift *> i(m_gifts);
    while (i.hasNext()) {
        Gift *gift = i.next(); 
        if (gift->isVisible()) {
            delete gift;
            i.remove();
        }
    }*/
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
