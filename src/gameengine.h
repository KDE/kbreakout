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

class MainWindow;

class Gift;
class Brick;
class Ball;

// TODO: add m_ to all members

class GameEngine : public QObject
{
    Q_OBJECT
public:
    GameEngine(MainWindow *);
    ~GameEngine();
    
    // TODO: rename to isPaused
    bool gameIsPaused() const;

public slots:
    // handles the timer timeout signals
    void timerTimeout();
    void start(const QString &levelset);
    void pause();
    void resume();
    void togglePause();
    void moveBar(int newXPosition);
    void moveBarLeft();
    void moveBarRight();
    void fire();
    
    // cheating keys for debugging and testing. Enabled only when
    // KDE_DEBUG environment variable is true.
    void cheatSkipLevel();
    void cheatAddLife();

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
    void showInfoMessage(const QString &text);
    void showFireBallMessage();
    void loadNextLevel();
    void addScore(int points);
    void setScore(int score);
    void changeSpeed(qreal ratio); // ratio is newSpeed/oldSpeed
    void updateAttachedBalls(); // updates all balls attached to the bar
    void deleteMovingObjects();
    void deleteAllObjects();

    MainWindow *m_mainWindow; // needed to access actionCollection()
    QString levelSet;
    int m_level;
    int m_score;
    int m_elapsedTime; // in seconds
    // score to add if you hit a brick
    // decreases over time since last hit
    qreal m_dScore;
    // count of remaining bricks
    // (not counting the unbreakable ones)
    int m_remainingBricks;
    // the number of ticks of the timer between one repaint and another
    int m_repaintInterval;
    qreal m_speed; // should never be more than 2.0
    
    // moves the objects at every tick (but avoiding to repaint them)
    QTimer m_gameTimer; // TODO: rename to updateTimer
    // increases elapsed time every second
    // also used to check whether the game is paused
    // TODO: use KGameTimer, maybe it should to start up as soon as the game starts?
    //       (because it's used to check if the game is paused...)
    QTimer m_elapsedTimeTimer;
    
    // used to add a little randomness but in a deterministic way
    // it's reset when the ball gets fired
    int randomCounter;
    
    // Canvas Items
    bool m_gameOver;
    bool m_gameWon;
    bool m_cheatsEnabled;
    QList<Life *> m_lives;
    Score m_scoreCanvas;
    LevelInfo m_levelInfo;
    MessageBox m_messageBox;
    InfoMessage m_infoMessage;
    QList<Brick *> m_bricks;
    QList<Gift *> m_gifts; // visible gifts
    QList<Ball *> m_balls;
    Bar m_bar;
    
    // is set to true when deleteMovingObjects() is called
    bool m_itemsGotDeleted;

private slots:
    // hides the current showed message by m_messageBox 
    // unless the game is paused, won or game over
    void hideMessage();
    void step();
    void repaintMovingObjects();
    void loadLevel();
    // takes a life and gives you a new ball if there are any lives left
    void handleDeath();
    void increaseElapsedTime() {++m_elapsedTime;}
    
    friend class Gift;
    friend class Brick;
};

#endif // GAMEENGINE_H
