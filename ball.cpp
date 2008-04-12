#include "ball.h"

#include "globals.h"
#include "brick.h"

Ball::Ball()
{   
    width = BALL_SIZE;
    height = BALL_SIZE;
    elementId = "PlainBall";
    
    toBeFired = true;
    barPosition = 0.6;
    
    loadSprite();
}

void Ball::collideWithBrick(Brick *brick)
{
    if (type() == "UnstoppableBall" ||
            type() == "UnstoppableBurningBall") {
        brick->setDeleted();
        return; // don't bounce
    }
    
    // calculate bounce
    QRect brickRect(brick->getRect());
    QRect ballRect(getRect());
    
    int top = brickRect.top() - ballRect.top();
    int bottom = ballRect.bottom() - brickRect.bottom();
    int left = brickRect.left() - ballRect.left();
    int right = ballRect.right() - brickRect.right();
    int max = qMax(qMax(top, bottom), qMax(left, right));
    
    // bounce
    // TODO: check this stuff
    if (max == top && directionY > 0) {
        directionY *= -1;
    } else if (max == bottom && directionY < 0) {
        directionY *= -1;
    } else if (max == left && directionX > 0) {
        directionX *= -1;
    } else if (max == right && directionX < 0) {
        directionX *= -1;
    } else {
        return; // already bounced
    }
    
    if (type() == "BurningBall" || type() == "UnstoppableBurningBall") {
        brick->explode();
    } else {
        brick->hit();
    }
}
