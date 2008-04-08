// needed for floor
#include <cmath>

#include <QSvgRenderer>

#include <KDebug>

#include "item.h"
#include "renderer.h"

// static
KGameCanvasWidget *Item::canvas = 0;
qreal Item::m_scale = 1.0;
int Item::m_borderLeft = 0;
int Item::m_borderTop = 0;

// static
void Item::setCanvas(KGameCanvasWidget *c)
{
    canvas = c;
}

Item::Item()
    :KGameCanvasPixmap(canvas), width(0), height(0)
{
    if (canvas == 0) {
        kError() << "Item::Item(): a scene must be set "
                    "before calling the constructor!!" << endl;
        return;
    }
    
    connect(canvas, SIGNAL(spritesReloaded()), SLOT(loadSprite()));
    
    show();
}

void Item::updateScale()
{
    qreal scaleX = static_cast<double>(canvas->width())
                   / (BRICK_WIDTH * WIDTH);
    qreal scaleY = static_cast<double>(canvas->height())
                   / ((BRICK_HEIGHT + 1) * HEIGHT);
    m_scale = qMin(scaleX, scaleY);
    //kDebug() << "m_scale: " << m_scale << endl;
    // assure the m_scaled height of bricks is an int
    // making the m_scale a little smaller if needed
    m_scale = static_cast<qreal>(floor(m_scale*BRICK_HEIGHT))/BRICK_HEIGHT;
    if (m_scale <= 0) m_scale = 1.0 / BRICK_HEIGHT;
    //kDebug() << "m_scale: " << m_scale << endl;
    m_borderLeft = qRound( (static_cast<qreal>(canvas->width()) 
                        - m_scale * (BRICK_WIDTH * WIDTH)) / 2 );
    m_borderTop = qRound( (static_cast<qreal>(canvas->height()) 
                       - m_scale * (BRICK_HEIGHT * (HEIGHT-1))) / 2 );
}

void Item::loadSprite()
{
    updateScale();
    
    //TODO: should not be needed, or at least I should have a closer look at it
    if (qRound(m_scale*width) == 0 || qRound(m_scale*height) == 0) return;
    
    QSize size(qRound(m_scale*width), qRound(m_scale*height));
    setPixmap(Renderer::self()->renderedSvgElement(elementId, size));
    
    updatePosition(); //TODO: needed??????
}

Item::~Item()
{
    // shouldn't already be called, really... but somehow without this the whole
    // scene is repainted when an Item is deleted... 
    // (TODO: maybe because the destructor should be virtual?)
}

void Item::setType(const QString &type)
{
    elementId = type;
    loadSprite();
}

void Item::setRect(const QRectF &/*newRect*/)
{
    kDebug() << "setRect() does nothing, change it!\n";
    /*QRectF oldRect = boundingRect();
    m_scale((newRect.width()-2) / oldRect.width(), 
        (newRect.height()-2) / oldRect.height());
    setPos(newRect.x() + 1, newRect.y() + 1);*/
}

QRect Item::getRect() const
{
    return QRect(static_cast<int>(m_position.x()),
                 static_cast<int>(m_position.y()),
                 width, height);
}

void Item::moveTo(qreal x, qreal y)
{
    m_position = QPointF(x, y);
    updatePosition();
}

/*void Item::moveTo(int x, int y)
{
    moveTo(static_cast<qreal>(x), static_cast<qreal>(y));
}*/

void Item::moveTo(const QPointF &point) {
    moveTo(point.x(), point.y());
}

void Item::moveBy(qreal dx, qreal dy)
{
    m_position += QPointF(dx, dy);
    updatePosition();
}

void Item::updatePosition()
{
    //kDebug() << elementId << ": (" << " ," << ")\n";
    KGameCanvasPixmap::moveTo(
      static_cast<int>(m_scale * m_position.x()) + m_borderLeft,
      static_cast<int>(m_scale * m_position.y()) + m_borderTop);
}
