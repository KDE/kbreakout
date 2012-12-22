/***************************************************************************
 *   Copyright 2012 Viranch Mehta <viranch.mehta@gmail.com>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License          *
 *   version 2 as published by the Free Software Foundation                *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include "canvasitem.h"

#include <QPainter>

//static
KGameRenderer *CanvasItem::m_renderer = 0;

//to be handled by the QML bindings plugin
void CanvasItem::setRenderer(KGameRenderer *renderer)
{
    m_renderer = renderer;
}

CanvasItem::CanvasItem(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    KGameRendererClient(m_renderer, QString())
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}

QSize CanvasItem::boundingSize()
{
    return boundingRect().toRect().size();
}

void CanvasItem::setSpriteKey(const QString &key)
{
    if (spriteKey() != key) {
        // this also puts a request for the sprite pixmap
        KGameRendererClient::setSpriteKey(key);
        setRenderSize(boundingRect().toRect().size());
        emit spriteKeyChanged();
    }
}

bool CanvasItem::isValid() const
{
    return (m_renderer && m_renderer->spriteExists(spriteKey()));
}

void CanvasItem::setImplicitSize()
{
    if (isValid()) {
        QSize size = m_renderer->boundsOnSprite(spriteKey()).size().toSize();
        setImplicitWidth(size.width());
        setImplicitHeight(size.height());
    }
}

// called when the requested pixmap becomes available
void CanvasItem::receivePixmap(const QPixmap& pixmap)
{
    m_pixmap = pixmap;
    update();
}

void CanvasItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

    QSize size = boundingSize();
    if (renderSize() != size) {
        // requests the sprite pixmap of current size
        setRenderSize(size);
        return;
    }

    if (!m_pixmap.isNull()) {
        setImplicitSize();
        painter->drawPixmap(boundingRect().toRect(), m_pixmap);
    }
}

#include "canvasitem.moc"
