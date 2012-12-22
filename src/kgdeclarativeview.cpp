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

#include "kgdeclarativeview.h"

#include <QDeclarativeEngine>
#include <QDeclarativeContext>

#include <kdeclarative.h>
#include <KGameRenderer>

KgDeclarativeView::KgDeclarativeView(KGameRenderer *renderer, QWidget *parent) :
    QDeclarativeView(parent)
{
    KDeclarative kdeclarative;
    kdeclarative.setDeclarativeEngine(engine());
    kdeclarative.initialize();

    // binds things like i18n and icons
    kdeclarative.setupBindings();

    // bind our theme renderer so QML components can make use of it for constructing KGameRenderer
    setupRenderer(renderer);

    setResizeMode(QDeclarativeView::SizeRootObjectToView);
}

void KgDeclarativeView::setupRenderer(KGameRenderer *renderer)
{
    if (renderer) {
        engine()->rootContext()->setContextProperty("renderer", (QObject*) renderer);
    }
}

#include "kgdeclarativeview.moc"
