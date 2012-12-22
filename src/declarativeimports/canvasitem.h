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

#ifndef CANVASITEM_H
#define CANVASITEM_H

#include <QDeclarativeItem>
#include <KGameRenderer>
#include <KGameRendererClient>

class CanvasItem : public QDeclarativeItem, KGameRendererClient
{
    Q_OBJECT

    /*
     * The sprite key of the element from the theme SVG
     */
    Q_PROPERTY(QString spriteKey READ spriteKey WRITE setSpriteKey NOTIFY spriteKeyChanged)

    /*
     * Read-only: States whether the sprite key supplied is valid or not
     */
    Q_PROPERTY(bool valid READ isValid)

public:
    CanvasItem(QDeclarativeItem *parent = 0);

    static void setRenderer(KGameRenderer*);

    void setSpriteKey(const QString &spriteKey);

    bool isValid() const;

    void setImplicitSize();

    void receivePixmap(const QPixmap& pixmap);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget=0);

signals:
    void spriteKeyChanged();

private:
    static KGameRenderer *m_renderer;
    QPixmap m_pixmap;

    QSize boundingSize();

};

#endif
