#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QObject>
#include <QRect>
#include <QList>
#include <QSet>
#include <QTimer>
#include <QRegion>

#include "canvasitems.h"

class Gift;

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
    void addBrick(char type, int xCoordinate, int yCoordinate);
    void deleteBrick(Brick *brick);
    /**
     * returns the bricks to the left, right, top and bottom of the given one
     * if present
     */
    QSet<Brick *> nearbyBricks(Brick *brick);
    // adds the a gift to a brick without a gift
    //void addGift(const QString &type, QList <Brick *> *bricks);
    
    // convenience functions
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
    QList<Life *> m_lives;
    Score scoreCanvas;
    QList<Brick *> m_bricks;
    QList<Gift *> m_gifts; // visible gifts
    QList<Ball *> m_balls;
    Bar m_bar;
    
    // is set to true when deleteMovingObjects() is called
    bool itemsGotDeleted;

private slots:
    void step();
    void repaintMovingObjects();
    void loadLevel();
    // takes a life and gives you a new ball if there are any lives left
    void handleDeath();
    void increaseElapsedTime() {++elapsedTime;}
    
    friend class Gift;
};

#endif // GAMEENGINE_H
