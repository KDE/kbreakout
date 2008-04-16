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

#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QObject>
#include <QRect>
#include <QList>
#include <QSet>
#include <QTimer>
#include <QRegion>

#include "canvasitems.h"
#include "textitems.h"

class Gift;
class Brick;
class Ball;

// TODO: add m_ to all members

class GameEngine : public QObject
{
    Q_OBJECT
public:
    GameEngine();
    ~GameEngine();

public slots:
    void start(QString levelset);
    void pause();
    void resume();
    void togglePause();
    void moveBar(int newXPosition);
    void moveBarLeft();
    void moveBarRight();
    void fire();
    
    // cheating keys, debugging and testing only TODO: REMOVE
    void cheatSkipLevel() {loadNextLevel();}
    void cheatAddLife() {m_lives.append(new Life);}

signals:
    void gamePaused();
    void gameResumed(int barPosition);
    void gameEnded(int score, int level, int time);

private:
    void detectBallCollisions(Ball *ball);
    
    // auxiliary functions
    void handleBrickCollisions(Ball *ball);
    // adds the a gift to a brick without a gift
    //void addGift(const QString &type, QList <Brick *> *bricks);
    
    // convenience functions
    void showMessage(const QString &text);
    void loadNextLevel();
    void addScore(int points);
    void setScore(int score);
    void setUpdateInterval(qreal newUpdateInterval);
    void updateAttachedBalls(); // updates all balls attached to the bar
    void deleteMovingObjects();
    void deleteAllObjects();
    
    bool gameIsPaused();

    QString levelSet;
    int level;
    int score;
    int elapsedTime; // in seconds
    // score to add if you hit a brick
    // decreases over time since last hit
    qreal dScore;
    // count of remaining bricks
    // (not counting the unbreakable ones)
    int remainingBricks;
    qreal updateInterval;
    
    // moves the objects at every tick (but avoiding to repaint them)
    QTimer gameTimer; // TODO: rename to updateTimer
    // increases elapsed time every second
    // also used to check whether the game is paused
    // TODO: use KGameTimer, maybe has to start up as soon as the game starts?
    //       (because it's used to check if the game is paused...)
    QTimer elapsedTimeTimer;
    // repaints all moving objects
    QTimer repaintTimer;
    
    // Canvas Items
    bool m_gameOver;
    bool m_gameWon;
    QList<Life *> m_lives;
    Score scoreCanvas;
    LevelInfo levelInfo;
    MessageBox m_messageBox;
    QList<Brick *> m_bricks;
    QList<Gift *> m_gifts; // visible gifts
    QList<Ball *> m_balls;
    Bar m_bar;
    
    // is set to true when deleteMovingObjects() is called
    bool itemsGotDeleted;

private slots:
    // hides the current showed message by m_messageBox 
    // unless the game is paused, won or game over
    void hideMessage();
    void step();
    void repaintMovingObjects();
    void loadLevel();
    // takes a life and gives you a new ball if there are any lives left
    void handleDeath();
    void increaseElapsedTime() {++elapsedTime;}
    
    friend class Gift;
    friend class Brick;
};

#endif // GAMEENGINE_H
