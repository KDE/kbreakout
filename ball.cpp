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
    
    if (type() == "BurningBall") {
        brick->explode();
    } else {
        brick->hit();
    }
}
