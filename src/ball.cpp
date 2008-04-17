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

#include "ball.h"

#include "globals.h"
#include "brick.h"

#include <KDebug>

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
    if (type() == "UnstoppableBall") {
        brick->forcedHit();
        return; // don't bounce
    }
    if (type() == "UnstoppableBurningBall") {
        brick->explode();
        return; // don't bounce
    }
    
    
    // calculate bounce
    QRectF brickRect(brick->getRect());
    QRectF ballRect(getRect());
    
    int top = qRound(ballRect.bottom() - brickRect.top());
    int bottom = qRound(brickRect.bottom() - ballRect.top());
    int left = qRound(ballRect.right() - brickRect.left());
    int right = qRound(brickRect.right() - ballRect.left());
    int min = qMin(qMin(top, bottom), qMin(left, right));
    
    // bounce
    // TODO: check this stuff
    if (min == top && directionY > 0) {
        directionY *= -1;
        kDebug() << brickRect.intersects(getRect());
        moveBy(0, -top);
        kDebug() << brickRect.intersects(getRect());
    } else if (min == bottom && directionY < 0) {
        kDebug() << brickRect.intersects(getRect());
        directionY *= -1;
        moveBy(0, bottom);
        kDebug() << brickRect.intersects(getRect());
    } else if (min == left && directionX > 0) {
        kDebug() << brickRect.intersects(getRect());
        directionX *= -1;
        moveBy(-left, 0);
        kDebug() << brickRect.intersects(getRect());
    } else if (min == right && directionX < 0) {
        kDebug() << brickRect.intersects(getRect());
        directionX *= -1;
        moveBy(right, 0);
        kDebug() << brickRect.intersects(getRect());
    } else {
        return; // already bounced
    }
    
    if (type() == "BurningBall") {
        brick->explode();
    } else {
        brick->hit();
    }
}

/*
void Ball::collideWithBricks(const QList<Brick *> &bricks)
{
    
}*/
