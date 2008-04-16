/*
    Copyright 2008 Fela Winkelmolen <fela.kde@gmail.com> 
  
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

#ifndef GIFT_H
#define GIFT_H

#include "item.h"

class GameEngine;

class Gift : public Item
{
public:
    Gift(const QString &type);
    
    void move();
    void execute(GameEngine *engine);
    
private:
    // convenience functions
    void giftNextLevel();
    void giftMagicEye();
    void giftMagicWand();
    void giftSplitBall();
    void giftUnstoppableBall();
    void giftBurningBall();
    void giftMoreExplosion();
    
    GameEngine *engine;
    int speed;
};

#endif // GIFT_H
