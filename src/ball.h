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
    
    void collideWithBricks(const QList<Brick *> &bricks);

private:
    void collideWithTwoBricks(const QList<Brick *> &bricks);
    void collideWithBrick(Brick *brick);
};

#endif // BALL_H
