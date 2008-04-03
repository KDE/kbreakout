#ifndef ITEM_H
#define ITEM_H

#include <KDE/KGameCanvas>

#include "consts.h"

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
    
    // TODO: change to load file: setSvgRenderer(const QString &themeFileName)
    static void setSvgRenderer(QSvgRenderer *);
    static void setCanvas(KGameCanvasWidget *);

    static qreal scale() {return m_scale;}
    static int borderLeft() {return m_borderLeft;}
    //TODO: change the following
    static QSvgRenderer *getSvg() {return svg;}
    
    // make parent functions accessible
    void hide() {KGameCanvasPixmap::hide();}
    void show() {KGameCanvasPixmap::show();}
    bool isVisible() {return KGameCanvasItem::visible();}
    void setPixmap(const QPixmap &p) {KGameCanvasPixmap::setPixmap(p);}
    
    void setType(const QString &type);
    QString type() const {return elementId;}
    
    void setRect(const QRectF &newBoundingRect);
    
    //rect() const is virtual so DON'T use that name!!!
    QRect getRect() const;
    // TODO: pos->position position->m_position
    QPoint pos() const {return position.toPoint();}
    
    void moveTo(qreal x, qreal y);
    void moveTo(int x, int y);
    void moveTo(const QPointF &point);
    void moveBy(qreal dx, qreal dy);
    void updatePosition();

public slots:
    // load or reloads the sprite
    virtual void loadSprite();
    
protected:
    static void updateScale();
    
    static QSvgRenderer *svg;
    // canvas container of all Items
    static KGameCanvasWidget *canvas;
    
    // TODO: rename to m_scaleRatio
    static qreal m_scale;
    static int m_borderLeft;
    static int m_borderTop;
    
    int width;
    int height;
    QPointF position;
    QString elementId;
};

#endif //ITEM_H
