#include "svgitem.h"

SvgItem::SvgItem(KGameRenderer *renderer) :
    QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap),
    m_renderer(renderer)
{
}

QPixmap SvgItem::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    return m_renderer->spritePixmap(id, m_renderer->boundsOnSprite(id).size().toSize());
}
