#include "canvasitem.h"

#include <QPainter>

//static
KGameRenderer *CanvasItem::m_renderer = 0;

void CanvasItem::setRenderer(KGameRenderer *renderer)
{
    m_renderer = renderer;
}

CanvasItem::CanvasItem(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}

QString CanvasItem::spriteKey() const
{
    return m_key;
}

void CanvasItem::setSpriteKey(const QString &spriteKey)
{
    m_key = spriteKey;
}

bool CanvasItem::isValid() const
{
    return (m_renderer && m_renderer->spriteExists(m_key));
}

QSize CanvasItem::spriteSize()
{
    if (isValid())
        return m_renderer->boundsOnSprite(m_key).size().toSize();
    return QSize();
}

void CanvasItem::setImplicitSize()
{
    if (isValid()) {
        QSize size = spriteSize();
        setImplicitWidth(size.width());
        setImplicitHeight(size.height());
    }
}

QSize CanvasItem::implicitSize()
{
    return QSize(implicitWidth(), implicitHeight());
}

void CanvasItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (isValid()) {
        setImplicitSize();
        QPixmap pix = m_renderer->spritePixmap(m_key, implicitSize());
        painter->drawPixmap(boundingRect().toRect(), pix);
    }
}
