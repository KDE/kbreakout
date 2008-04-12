#ifndef BALL_H
#define BALL_H

#include "item.h"

class Brick;

class Ball : public Item
{
public:
    Ball();
    
    // direction vector
    qreal directionX;
    qreal directionY;
    
    bool toBeFired;
    qreal barPosition;
    
    // TODO: pass multiple bricks and improve the algorithm
    // returns false if the direction is wrong
    void collideWithBrick(Brick *brick);
};

#endif // BALL_H
