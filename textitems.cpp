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

void TextItem::setText(const QString &text) 
{
    m_text = text;
    loadSprite();
}

void TextItem::loadSprite()
{
    updateScale();
    
    int w = qRound(m_scale*width);
    int h = qRound(m_scale*height);
    
    QSize size(w, h);
    QPixmap pixmap = Renderer::self()->renderedSvgElement(elementId, size);
    
    QPainter p(&pixmap);
    int fontSize = fontUtils::fontSize(p, m_text, qRound(w*0.9), h,
                                   fontUtils::DoNotAllowWordWrap);
    
    p.setPen(QColor(255, 255, 255, 220));
    p.setFont(QFont("Helvetica", fontSize, QFont::Bold));
    p.drawText(QRectF(0, 0, w, h), 
                Qt::AlignCenter, m_text);
    setPixmap(pixmap);
    
    repaint();
}

Score::Score()
    : TextItem()
{
    width =  (BRICK_WIDTH * WIDTH)/6;
    height = qRound(BRICK_HEIGHT * 1.5);
    moveTo(0, - (height * 1.2));
    
    setScore(0);
}

void Score::setScore(int newScore)
{
    //scoreString = QString("%L1").arg(newScore);
    //scoreString = QString("%1").arg(0.01*newScore, 0, 'f', 2);
    QString displayString = QString::number(newScore);
    
    // insert spaces every 3 characters
    int stringSize = displayString.size();
    // dont count the '-'
    if (newScore < 0) --stringSize;
    for (int i = 1; i <= (stringSize-1) / 3; ++i) {
        int position = displayString.size() - 4 * i + 1;
        displayString.insert(position, " ");
    }
    
    setText(displayString);
}

LevelInfo::LevelInfo()
    : TextItem()
{
    width =  (BRICK_WIDTH * WIDTH)/5;
    height = qRound(BRICK_HEIGHT * 1.5);
    moveTo((BRICK_WIDTH * WIDTH)/5, - (height * 1.2));
}

void LevelInfo::setLevel(int newLevel)
{
    setText(i18n("Level %1", newLevel));
}

MessageBox::MessageBox()
{
    width = BRICK_WIDTH * 7;
    height = BRICK_HEIGHT * 5;
    
    int x = (BRICK_WIDTH * WIDTH - width) / 2;
    int y = (BRICK_HEIGHT * HEIGHT - height) / 2;
    moveTo(x, y);
}
