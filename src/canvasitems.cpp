/*
    Copyright 2007-2008 Fela Winkelmolen <fela.kde@gmail.com> 
    Copyright 2010 Brian Croom <brian.s.croom@gmail.com>
  
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

#include "canvasitems.h"
#include "globals.h"

Bar::Bar()
    : Item("PlainBar", DEFAULT_BAR_WIDTH, BRICK_HEIGHT)
{
    moveTo(300, (HEIGHT-1)*BRICK_HEIGHT);
}

void Bar::reset()
{
    int oldWidth = width;
    width = DEFAULT_BAR_WIDTH;
    setType("PlainBar");
    loadSprite();
    
    // move the bar to retain it's center
    int d = (oldWidth - width)/2;
    
    moveBy(d, 0);
}

void Bar::enlarge()
{
    int oldWidth = width;
    width = qRound(width*RESIZE_BAR_RATIO);
    if (width > MAX_BAR_WIDTH) {
        width = MAX_BAR_WIDTH;
    }
    loadSprite();
    
    // move the bar to retain it's center
    int d = (oldWidth - width)/2;
    
    moveBy(d, 0);
}

void Bar::shrink()
{
    int oldWidth = width;
    width = qRound(width/RESIZE_BAR_RATIO);
    if (width < MIN_BAR_WIDTH) {
        width = MIN_BAR_WIDTH;
    }
    loadSprite();
    
    // move the bar to retain it's center
    int d = (oldWidth - width)/2;
    
    moveBy(d, 0);
}

int Bar::center() const
{
    return getRect().left() + getRect().width()/2;
}

int Life::count = 0;

Life::Life()
    : Item("PlainBar", qRound((BRICK_WIDTH)/1.3), qRound((BRICK_HEIGHT)/1.3))
{
    ++count;
    
    int x = -5 + qRound( BRICK_WIDTH * WIDTH 
                        - (BRICK_WIDTH * count) );
    int y = qRound(-1.2 * BRICK_HEIGHT);
     
    moveTo(x, y);
    repaint();
}

Life::~Life()
{
    --count;
}

Background::Background()
    : Item("BackgroundOverlay", (BRICK_WIDTH*WIDTH)+1, (BRICK_HEIGHT*HEIGHT)+1)
{
    moveTo(-1, -1);
    repaint();
}
