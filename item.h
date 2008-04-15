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

#ifndef ITEM_H
#define ITEM_H

#include <KDE/KGameCanvas>

#include "globals.h"

class QSvgRenderer;

// all items (balls, bricks, ...) inerit from this class
// initially svg and the abstract canvas are set to 0, before calling the 
// constructor setSvgRenderer() and setCanvasAbstract() should be called!!!
// TODO: should be and abstract class???
class Item : public QObject, private KGameCanvasPixmap
{
    Q_OBJECT
public:
    Item();
    ~Item();
    
    static void setCanvas(KGameCanvasWidget *);

    static qreal scale() {return m_scale;}
    static int borderLeft() {return m_borderLeft;}
    
    // make superclass functions accessible
    using KGameCanvasPixmap::hide;
    using KGameCanvasPixmap::show;
    using KGameCanvasPixmap::raise;
    //using KGameCanvasPixmap::visible;
    using KGameCanvasPixmap::setPixmap;
    //void hide() {KGameCanvasPixmap::hide();}
    //void show() {KGameCanvasPixmap::show();}
    bool isVisible() {return KGameCanvasItem::visible();}
    //void setPixmap(const QPixmap &p) {KGameCanvasPixmap::setPixmap(p);}
    
    void setType(const QString &type);
    QString type() const {return elementId;}
    
    void setRect(const QRectF &newBoundingRect);
    
    //rect() const is virtual so DON'T use that name!!!
    QRect getRect() const;
    QPoint position() const {return m_position.toPoint();}
    
    void moveTo(qreal x, qreal y);
    //void moveTo(int x, int y);
    void moveTo(const QPointF &point);
    void moveBy(qreal dx, qreal dy);
    void repaint();

public slots:
    // load or reloads the sprite
    virtual void loadSprite();
    
protected:
    static void updateScale();
    
    // canvas container of all Items
    static KGameCanvasWidget *canvas;
    
    // TODO: rename to m_scaleRatio
    static qreal m_scale;
    static int m_borderLeft;
    static int m_borderTop;
    
    int width;
    int height;
    QPointF m_position;
    QString elementId;
};

#endif //ITEM_H
