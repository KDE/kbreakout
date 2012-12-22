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

#ifndef KGDECLARATIVEVIEW_H
#define KGDECLARATIVEVIEW_H

#include <QDeclarativeView>

class KGameRenderer;

/**
 * @class KgDeclarativeView
 * @since 4.9.1
 * @short A QDeclarativeView that adds KDE specific module paths and javascript bindings.
 *
 * This class is a QDeclarativeView that sets up a KDE specific environment
 * like custom QML module paths and javascript function bindings (like i18n)
 * to the declarative view.
 *
 * It takes a KGameRenderer object and makes it available to QML via the
 * "renderer" property in its root context. This is useful if you want to
 * make a QML component that wants to make use of the KGameRenderer
 * shared across the game instance. While this is optional, there may be
 * some QML components that require it. So it is recommended that you
 * supple the declarative view with a renderer.
 *
 * The KGameRenderer object can either be passed in the constructor or
 * set up later using the setupRenderer() method.
 */
class KgDeclarativeView : public QDeclarativeView
{
    Q_OBJECT
public:
    ///Construcs a new KgDeclarativeView with KDE specific environment.
    ///@param renderer The KGameRenderer object for use by potential
    ///QML components for KDE Games
    ///@param parent The parent widget for this view (usually the main
    ///window of the game)
    KgDeclarativeView(KGameRenderer *renderer=0, QWidget *parent=0);

    ///Set the KGameRenderer object for use by potential QML components
    ///for KDE games. The object is accessible through the "renderer"
    ///property from QML.
    void setupRenderer(KGameRenderer *renderer);

};

#endif //KGDECLARATIVEVIEW_H
