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

#include "textitems.h"

#include "renderer.h"
#include "fontutils.h"
#include <KLocale>

TextItem::TextItem()
{
    elementId = "Display";
} 

void TextItem::loadSprite()
{
    updateScale();
    
    QSize size(qRound(m_scale*width), qRound(m_scale*height));
    QPixmap pixmap = Renderer::self()->renderedSvgElement(elementId, size);
    
    QPainter p(&pixmap);
    int fontSize = fontUtils::fontSize(p, displayString, width, height,
                                   fontUtils::DoNotAllowWordWrap);
    
    p.setPen(QColor(255,255,255,150));
    p.setFont(QFont("Helvetica", fontSize, QFont::Bold));
    p.drawText(QRectF(0, 0, m_scale*width, m_scale*height*0.86), 
                Qt::AlignCenter, displayString);
    setPixmap(pixmap);
    
    repaint();
}

Score::Score()
    : TextItem()
{
    height = static_cast<int>(BRICK_HEIGHT * 1.5);
    width =  (BRICK_WIDTH * WIDTH)/6;
    moveTo(0, - (height * 1.2));
    
    setScore(0);
}

void Score::setScore(int newScore)
{
    //scoreString = QString("%L1").arg(newScore);
    //scoreString = QString("%1").arg(0.01*newScore, 0, 'f', 2);
    displayString = QString::number(newScore);
    
    // insert spaces every 3 characters
    int stringSize = displayString.size();
    for (int i = 1; i <= (stringSize-1) / 3; ++i) {
        int position = displayString.size() - 4 * i + 1;
        displayString.insert(position, " ");
    }
    
    loadSprite();
}

LevelInfo::LevelInfo()
    : TextItem()
{
    height = static_cast<int>(BRICK_HEIGHT * 1.5);
    width =  (BRICK_WIDTH * WIDTH)/5;
    moveTo((BRICK_WIDTH * WIDTH)/5, - (height * 1.2));
}

void LevelInfo::setLevel(int newLevel)
{
    displayString = i18n("Level %1", newLevel);
    
    loadSprite();
}
