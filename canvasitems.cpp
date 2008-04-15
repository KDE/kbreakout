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

#include "canvasitems.h"

#include "globals.h"
#include "renderer.h"
#include "fontutils.h"

#include <cmath>
#include <KDebug>

Bar::Bar()
{
    width = DEFAULT_BAR_WIDTH;
    height = BRICK_HEIGHT;
    elementId = "PlainBar";
    
    moveTo(300, (HEIGHT-1)*BRICK_HEIGHT);

    //loadSprite();
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
    if (width > MAX_BAR_WIDTH) width = MAX_BAR_WIDTH;
    loadSprite();
    
    // move the bar to retain it's center
    int d = (oldWidth - width)/2;
    
    moveBy(d, 0);
}

void Bar::shrink()
{
    int oldWidth = width;
    kDebug() << "old width = " << width << endl;
    width = qRound(width/RESIZE_BAR_RATIO);
    if (width < MIN_BAR_WIDTH) width = MIN_BAR_WIDTH;
    loadSprite();
    
    // move the bar to retain it's center
    int d = (oldWidth - width)/2;
    
    moveBy(d, 0);
}

int Life::count = 0;

Life::Life()
{
    ++count;
    
    width = (BRICK_WIDTH)/2;
    height = (BRICK_HEIGHT)/2;
    elementId = "PlainBar";
    
    int x = -5 + qRound( BRICK_WIDTH * WIDTH 
                        - (0.6 * BRICK_WIDTH * count) );
    int y = qRound(-0.75*BRICK_HEIGHT);
     
    moveTo(x, y);
    
    loadSprite();
}

Life::~Life()
{
    --count;
}

Background::Background()
{
    int w = BRICK_WIDTH * WIDTH;
    int h = BRICK_HEIGHT * HEIGHT;
    //qreal border = BACKGROUND_BORDER * w;
    // BORDER_RATIO is to make it a little bit bigger...
    // TODO: not sure (souldn't it be 2??? but this seems to work better...)
    width = w + 1;
    height = h + 1;
    elementId = "BackgroundOverlay";
    moveTo(-1, -1);
        
    loadSprite();
}

Score::Score()
{
    height = static_cast<int>(BRICK_HEIGHT * 1.5);
    width =  (BRICK_WIDTH * WIDTH)/6;
    moveTo(0, - (height * 1.2));
    
    elementId = "Display";
    
    setScore(0);
}

void Score::loadSprite()
{
    updateScale();
    
    QSize size(qRound(m_scale*width), qRound(m_scale*height));
    QPixmap pixmap = Renderer::self()->renderedSvgElement(elementId, size);
    
    QPainter p(&pixmap);
    int fontSize = fontUtils::fontSize(p, scoreString, width, height,
                                   fontUtils::DoNotAllowWordWrap);
    
    p.setPen(QColor(255,255,255,150));
    p.setFont(QFont("Helvetica", fontSize, QFont::Bold));
    p.drawText(QRectF(0, 0, m_scale*width, m_scale*height*0.86), 
                Qt::AlignCenter, scoreString);
    setPixmap(pixmap);
    
    repaint();
}

void Score::setScore(int newScore)
{
    //scoreString = QString("%L1").arg(newScore);
    //scoreString = QString("%1").arg(0.01*newScore, 0, 'f', 2);
    scoreString = QString::number(newScore);
    
    // insert spaces every 3 characters
    int stringSize = scoreString.size();
    for (int i = 1; i <= (stringSize-1) / 3; ++i) {
        int position = scoreString.size() - 4 * i + 1;
        scoreString.insert(position, " ");
    }
    
    loadSprite();
}
