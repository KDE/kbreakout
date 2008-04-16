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

#ifndef CANVASITEMS_H
#define CANVASITEMS_H

#include "item.h"

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

#endif // CANVASITEMS_H
