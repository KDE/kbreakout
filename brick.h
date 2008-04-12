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

#ifndef BRICK_H
#define BRICK_H

#include "item.h"

class Gift;
class GameEngine;

class Brick : public Item
{
    Q_OBJECT
public:
    Brick(GameEngine *game, char typeChar, int posX, int posY);
    
    Gift *gift; // TODO: make private
    
    void setDeleted();
    void hit();
    void explode();
    
    bool isDeleted() {return m_deleted;}
    
    // a ball collided against the brick
    // can acces the private GameEngine members so be carefull!!! (TODO: (??))
    //virtual void handleCollision();

public slots:
    void burn();
    void hide();

private:
    void handleDeletion();
    void setTypeFromChar(char type);
    QList<Brick *> nearbyBricks();

    GameEngine *m_game;
    bool m_deleted;
};

#endif //BRICK_H
