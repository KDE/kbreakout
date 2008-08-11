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
#include "gift.h"
#include "brick.h"
#include "ball.h"

#include <cmath>
#include <math.h>

#include <QTimer>
#include <QAction>

#include <KLocale>
#include <KConfig>
#include <KActionCollection>
#include <KStandardDirs>
#include <KConfigGroup>
#include <KDebug>

GameEngine::GameEngine(MainWindow *mainWindow)
    : m_mainWindow(mainWindow), randomCounter(0)
{
    m_gameTimer.setInterval(REPAINT_INTERVAL);
    connect(&m_gameTimer, SIGNAL(timeout()), SLOT(timerTimeout()));
    
    m_elapsedTimeTimer.setInterval(1000);
    connect(&m_elapsedTimeTimer, SIGNAL(timeout()), SLOT(increaseElapsedTime()));
}

GameEngine::~GameEngine()
{
    deleteAllObjects();
}

void GameEngine::start(const QString& l)
{
    levelSet = l;
    
    while(!m_lives.isEmpty()) {
        delete m_lives.takeFirst();
    }
    for (int i = 0; i < INITIAL_LIVES; ++i){
        m_lives.append(new Life);
    }
    
    m_gameOver = false;
    m_gameWon = false;
    m_level = 1;
    m_elapsedTime = 0;
    setScore(0);
    
    loadLevel();
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    
    m_elapsedTimeTimer.start();
    m_gameTimer.start();
}

bool GameEngine::gameIsPaused()
{
    return !m_elapsedTimeTimer.isActive();
}

void GameEngine::pause()
{
    if (gameIsPaused()) return;
    if (m_gameWon || m_gameOver) return;
    
    m_elapsedTimeTimer.stop();
    m_gameTimer.stop();
    emit gamePaused();
    showMessage(i18n("Game Paused!"));
}

void GameEngine::resume()
{
    if (!gameIsPaused()) return;
    
    m_elapsedTimeTimer.start();
    m_gameTimer.start();
    
    // only restart the m_gameTimer if there are objects moving
    bool movingObjects = false;
    foreach (Ball *ball, m_balls) {
        if (!ball->toBeFired) {
            movingObjects = true;
            continue;
        }
    }
    
    int barPosition = m_bar.position().x() + m_bar.getRect().width()/2;
    hideMessage();
    emit gameResumed(barPosition);
}

void GameEngine::togglePause()
{
    if (gameIsPaused()) {
        resume();
    } else {
        pause();
    }
}

void GameEngine::moveBar(int newPos) 
{
    if (gameIsPaused()) return;
    
    // width of the game
    int width = BRICK_WIDTH * WIDTH;
    // width of the bar
    int w = m_bar.getRect().width();
    int y = m_bar.getRect().y();
    int x = newPos - w/2;
    
    if (x < 0) x = 0;
    else if (x > width - w) x = width - w;
    
    m_bar.moveTo(x, y);
}

void GameEngine::moveBarLeft()
{
    moveBar(m_bar.getRect().left() + m_bar.getRect().width()/2 - BAR_MOVEMENT);
}

void GameEngine::moveBarRight()
{
    moveBar(m_bar.getRect().left() + m_bar.getRect().width()/2 + BAR_MOVEMENT);
}

void GameEngine::fire()
{
    if (gameIsPaused()) {
        resume();
        return;
    }

    foreach (Ball *ball, m_balls) {
        if (!ball->toBeFired) continue;
        // else
        ball->toBeFired = false;
        
        qreal ballCenter= ball->getRect().left() + (ball->getRect().width())/2;
        qreal barCenter = m_bar.getRect().left() + (m_bar.getRect().width())/2;
        qreal angle = M_PI / 3;
        angle *= (barCenter - ballCenter) / (m_bar.getRect().width()/2);
        angle += M_PI_2;
        
        ball->directionX =  cos(angle) * BALL_SPEED;
        ball->directionY = -sin(angle) * BALL_SPEED;
    }
    
    m_dScore = BRICK_SCORE;
    m_infoMessage.hide();
    
    randomCounter = 0;
}

// TODO: external level loarder???
void GameEngine::loadLevel()
{
    
    QString path = "levelsets/" + levelSet + ".levelset";
    path =  KStandardDirs::locate("appdata", path);
    KConfig file(path, KConfig::SimpleConfig);
    
    QString levelName("level" + QString::number(m_level));
    
    if (!file.hasGroup(levelName)) {
        if (m_level == 1) {
            kError() << "Invalid '" << levelName << "' in levelset " 
                     << levelSet << endl;
        } else {
            m_gameWon = true;
            kDebug() << m_score;
            addScore(GAME_WON_SCORE + m_lives.size() * LIFE_SCORE);
            kDebug() << m_lives.size() << m_score;
            showMessage(i18n("Well done! You won the game!"));
            emit gameEnded(m_score, -1, m_elapsedTime);
        }
        return;
    }
    
    KConfigGroup lvl = file.group(levelName);    
    int y = 1;
    QString key("line" + QString::number(y));
    
    deleteAllObjects();
    m_remainingBricks = 0;
    
    while(lvl.hasKey(key)) {
        // one line of bricks, to be converted
        QString line = lvl.readEntry(key, "error");
        if (line == "error") {
            kError() << "Something strange happened!!\n";
            return;
        }
        
        kDebug() << line << endl;
        
        if (line.size() > WIDTH) {
            kError() << "Invalid file: to many bricks\n";
        }
        
        // convert the string, each char represents a brick
        for (int x = 0; x < line.size(); ++x) {
            char type = line[x].toAscii();
            if (type != '-') {
                m_bricks.append(new Brick(this, type, x, y));
            }
        }
        
        ++y;
        key = "line" + QString::number(y);
    }
    
    // add gifts
    
    // bricks whitout gifts
    QList<Brick *> bricksLeft = m_bricks;
    QMutableListIterator<Brick *> i(bricksLeft);
    while (i.hasNext()) {
        Brick *brick = i.next();
        if (brick->type() == "UnbreakableBrick")
            i.remove();
    }
    
    for (int i = 0; i < GIFT_TYPES_COUNT; ++i) {
        key = GIFT_TYPES[i];
        if (!lvl.hasKey(key)) continue;
        
        QString line = lvl.readEntry(key, "error");
        if (line == "error") {
            kError() << "Impossible Reading " << m_level << ":" << key << endl;
            return;
        }
        bool ok;
        int n = line.toInt(&ok);
        if (!ok) {
            kError() << levelName << ":" << key << " invalid number!!" << endl;
            continue;
        }
        if (bricksLeft.count() < n) {
            kError() << levelName << ": to many gifts!! " << endl;
            break;
        }
        for (int i = 0; i < n; ++i) {
            Gift *gift = new Gift(key); // key is the gift type
            gift->hide();
            
            int index = qrand() % bricksLeft.count();
            bricksLeft.at(index)->setGift(gift);
            bricksLeft.removeAt(index);
        }
    }
    
    m_balls.append(new Ball);
    moveBar(m_bar.getRect().x() + m_bar.getRect().width()/2);
    m_bar.reset();
    updateAttachedBalls();
    
    m_gameTimer.setInterval(REPAINT_INTERVAL);
    m_speed = 1.8;
    m_repaintInterval = 1;
    m_levelInfo.setLevel(m_level);
    if (gameIsPaused()) resume();
    showMessage(i18n("Level %1", m_level));
    QTimer::singleShot(2000, this, SLOT(hideMessage()));
    
    showFireBallMessage();
}

void GameEngine::timerTimeout() {
    step(); // call step() at every tick
    
    // only repaint every m_repaintInterval ticks
    static int tick = 0;
    tick = (tick + 1) % m_repaintInterval;
    
    if (tick == 0){
        repaintMovingObjects();
    }
}

void GameEngine::changeSpeed(qreal ratio) {
    kDebug() << "Update interval =" << m_gameTimer.interval();
    m_speed *= ratio;
    if (m_speed > 2.0) {
        // make sure the minimum update interval is respected
        if (m_gameTimer.interval() < MINIMUM_UPDATE_INTERVAL * 2) {
            m_speed = 2.0;
            return;
        }
        // else
        
        // half the speed
        m_speed /= 2.0;
        // and double the number of ticks of the timer per time unit
        m_gameTimer.setInterval(m_gameTimer.interval()/2);
        m_repaintInterval *= 2;
        m_gameTimer.start();
    }
    if (m_speed < 1.0) {
        if (m_gameTimer.interval() >= REPAINT_INTERVAL) {
            if (m_speed < MINIMUM_SPEED) m_speed = MINIMUM_SPEED;
            return;
        }
        // else
        
        // double the speed
        m_speed *= 2.0;
        // and double the number of ticks of the timer per time unit
        m_gameTimer.setInterval(m_gameTimer.interval()*2);
        m_repaintInterval /= 2;
        m_gameTimer.start();
    }

}

void GameEngine::step()
{
    // needed to exit from the loop if the arrays that they cicle
    // change (items get deleted)    
    m_itemsGotDeleted = false;
    
    m_dScore *= SCORE_AUTO_DECREASE;
    foreach (Ball *ball, m_balls) {
        if (ball->toBeFired) continue;
        
        // TODO: add function ball->move(speed)
        ball->moveBy(ball->directionX * m_speed, ball->directionY * m_speed);
        // collision detection
        detectBallCollisions(ball);
        if (m_itemsGotDeleted) return;
    }
    
    QMutableListIterator<Gift *> i(m_gifts);
    while (i.hasNext()) {
        Gift *gift = i.next();
        if (!gift->isVisible()) continue; // do nothing

        qreal giftSpeed = std::sqrt(m_speed / m_gameTimer.interval());
        gift->move(giftSpeed, m_gameTimer.interval());
        if (gift->getRect().bottom() > BRICK_HEIGHT * HEIGHT) {
            i.remove();
            delete gift;
        }
        else if (m_bar.getRect().intersects(gift->getRect())) {
            gift->execute(this);
            if (m_itemsGotDeleted) return;
            i.remove();
            delete gift;
        }
    }
}

void GameEngine::repaintMovingObjects()
{
    m_bar.repaint();
    
    foreach (Ball *ball, m_balls) {
        ball->repaint();
    }
    
    foreach (Gift *gift, m_gifts) {
        if (!gift->isVisible()) continue;
        
        gift->repaint();
    }

    // avoid infinite loops of the ball
    ++randomCounter;
    if (randomCounter == 1024) {
        randomCounter = 0;
        foreach (Ball *ball, m_balls) {
            if (qrand() % 2) {
                ball->directionX += 0.002;
            } else {
                ball->directionY += 0.002;
            }
        }

        // increase the speed a little
        changeSpeed(AUTO_SPEED_INCREASE);
    }
    
    // move attached balls if needed
    updateAttachedBalls();
}

void GameEngine::detectBallCollisions(Ball *ball)
{
    // never run this function more than two time recursively
    static bool firstTime = true;
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
        qreal barCenter  = m_bar.getRect().left() + (m_bar.getRect().width())/2;
        
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
    }
    // bounce against the bricks (and optionally break them)
    else {
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
    }
}

void GameEngine::handleDeath()
{
    hideMessage();
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
    }
}


void GameEngine::handleBrickCollisions(Ball *ball)
{
    if (m_itemsGotDeleted) return;
    QRect rect = ball->getRect();

    QList<Brick *> bricksIntersecting;
    foreach (Brick *brick, m_bricks) {
        if (brick->isDeleted()) continue;
        QRect brickRect = brick->getRect();
        
        if (brickRect.intersects(rect)) {
            bricksIntersecting.append(brick);
        }
    }
    
    if (!bricksIntersecting.isEmpty()) {
        ball->collideWithBricks(bricksIntersecting);
    }
}

//======= convenience functions =================//

void GameEngine::showMessage(const QString &text)
{
    m_messageBox.setText(text);
    m_messageBox.raise();
    m_messageBox.show();
}

void GameEngine::showInfoMessage(const QString &text)
{
    m_infoMessage.setText(text);
    m_infoMessage.raise();
    m_infoMessage.show();
}

void GameEngine::showFireBallMessage()
{
    QAction *fireAction = m_mainWindow->actionCollection()->action("fire");
    QString shortcut = fireAction->shortcut().toString(QKeySequence::NativeText);
    showInfoMessage(i18n("Press %1 to fire the ball", shortcut));
}

void GameEngine::hideMessage()
{
    if (gameIsPaused()) return;
    if (m_gameWon && m_gameOver) return;
    
    // else
    m_messageBox.hide();
}

void GameEngine::loadNextLevel()
{
    // assign points for each remaining brick
    foreach (Brick *brick, m_bricks) {
        // don't assign points for Unbreakable Bricks
        if (brick->type() == "UnbreakableBrick") continue;
        if (brick->isDeleted()) continue;
        
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
    addScore(LEVEL_SCORE);
}

void GameEngine::addScore(int points)
{
    m_score += points;
    m_scoreCanvas.setScore(m_score);
}

void GameEngine::setScore(int newScore)
{
    m_score = newScore;
    m_scoreCanvas.setScore(m_score);
}

void GameEngine::updateAttachedBalls()
{
    foreach (Ball *ball, m_balls) {
        if (!ball->toBeFired) continue;
        // else
        int ballX = m_bar.getRect().left() + 
                qRound(ball->barPosition * m_bar.getRect().width());
        ball->moveTo(ballX, m_bar.getRect().top() - BALL_SIZE);
        ball->repaint();
    }
}

// TODO: check (in debugger?) why this function is called so much...
inline void GameEngine::deleteMovingObjects()
{
    kDebug() << "Deleting objects...\n";
    m_itemsGotDeleted = true;
    while (!m_balls.isEmpty()) delete m_balls.takeFirst();
    
    QMutableListIterator<Gift *> i(m_gifts);
    while (i.hasNext()) {
        Gift *gift = i.next(); 
        if (gift->isVisible())
            delete gift;
            i.remove();
    }
}

inline void GameEngine::deleteAllObjects()
{
    kDebug() << "all object deleted";
    deleteMovingObjects();
    while (!m_bricks.isEmpty()) delete m_bricks.takeFirst();
    while (!m_gifts.isEmpty()) delete m_gifts.takeFirst();
}

#include "gameengine.moc"
