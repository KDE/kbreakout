#include "themedimage.h"

#include <QPainter>
#include <KDebug>

//static
KGameRenderer *ThemedImage::m_renderer = 0;

void ThemedImage::setRenderer(KGameRenderer *renderer)
{
    m_renderer = renderer;
}

ThemedImage::ThemedImage(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}

QString ThemedImage::spriteKey() const
{
    return m_key;
}

void ThemedImage::setSpriteKey(const QString &spriteKey)
{
    m_key = spriteKey;
}

bool ThemedImage::isValid() const
{
    return (m_renderer && m_renderer->spriteExists(m_key));
}

QSize ThemedImage::spriteSize()
{
    if (isValid())
        return m_renderer->boundsOnSprite(m_key).size().toSize();
    return QSize();
}

void ThemedImage::setImplicitSize()
{
    if (isValid()) {
        QSize size = spriteSize();
        setImplicitWidth(size.width());
        setImplicitHeight(size.height());
    }
}

QSize ThemedImage::implicitSize()
{
    return QSize(implicitWidth(), implicitHeight());
}

void ThemedImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (isValid()) {
        setImplicitSize();
        QPixmap pix = m_renderer->spritePixmap(m_key, implicitSize());
        painter->drawPixmap(boundingRect().toRect(), pix);
    }
}
