#ifndef CANVASITEMS_H
#define CANVASITEMS_H

#include <KDE/KGameCanvas>

#include "item.h"
#include "globals.h"

class Ball : public Item
{
public:
    Ball();
    
    // direction vector
    qreal directionX;
    qreal directionY;
    
    bool toBeFired;
    qreal barPosition;
};

class Gift;

class Brick : public Item
{
public:
    Brick();
    
    Gift *gift;

    // a ball collided against the brick
    // can acces the private GameEngine members so be carefull!!!
    //virtual void handleCollision();
};

class Bar : public Item
{
public:
    Bar();
    
    // resets to default values
    void reset();
    
    // used by GiftEnlargeBar and GiftShrinkBar
    void enlarge();
    void shrink();
};

class Life : public Item
{
public:
    Life();
    ~Life();
    
private:
    static int count;
};

class Background : public Item
{
public:
    Background();
};

class Score : public Item
{
public:
    Score();
    void setScore(int newScore);

private:
    QString scoreString;
    void loadSprite();
};

#endif // CANVASITEMS_H
