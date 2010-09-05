/*
    Copyright 2008 Fela Winkelmolen <fela.kde@gmail.com> 
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

#ifndef TEXT_ITEMS_H
#define TEXT_ITEMS_H

#include "item.h"

class TextItem : public Item
{
public:
    TextItem(int width, int height);
    
    void setText(const QString &text);
    void repaint();
    
    void show();
    void hide();
protected:
    virtual void loadSprite();
    
    QString m_text;
    KGameCanvasPixmap m_textPixmap;
};

class Score : public TextItem
{
public:
    Score();
    void setScore(int newScore);
};

class LevelInfo : public TextItem
{
public:
    LevelInfo();
    void setLevel(int newLevel);
};

class MessageBox : public TextItem
{
public:
    MessageBox();
};


class InfoMessage : public TextItem
{
public:
    InfoMessage();
};

#endif // TEXT_ITEMS_H
