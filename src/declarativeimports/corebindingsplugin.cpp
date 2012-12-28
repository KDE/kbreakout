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

#include "corebindingsplugin.h"

#include "canvasitem.h"

#include <QDeclarativeContext>
#include <KGameRenderer>

void CoreBindingsPlugin::initializeEngine(QDeclarativeEngine *engine, const char * /*uri*/)
{
    QObject *property = engine->rootContext()->contextProperty("renderer").value<QObject*>();
    KGameRenderer *renderer = (KGameRenderer*) property;
    CanvasItem::setRenderer(renderer);
}

void CoreBindingsPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("org.kde.games.core"));

    qmlRegisterType<CanvasItem>(uri, 0, 1, "CanvasItem");
}

#include "corebindingsplugin.moc"
