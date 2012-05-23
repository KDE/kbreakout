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

void CanvasItem::setImplicitSize()
{
    if (isValid()) {
        QSize size = m_renderer->boundsOnSprite(m_key).size().toSize();
        setImplicitWidth(size.width());
        setImplicitHeight(size.height());
    }
}

void CanvasItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);
    if (isValid()) {
        setImplicitSize();
        QPixmap pix = m_renderer->spritePixmap(m_key, boundingRect().toRect().size());
        painter->drawPixmap(boundingRect().toRect(), pix);
    }
}
