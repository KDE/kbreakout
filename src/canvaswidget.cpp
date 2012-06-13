/*
    Copyright 2012 Viranch Mehta <viranch.mehta@gmail.com>
  
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.
   
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
   
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "canvaswidget.h"
#include "canvasitem.h"
#include "globals.h"
#include "settings.h"

#include <QKeyEvent>

#include <kdeclarative.h>
#include <KStandardDirs>

CanvasWidget::CanvasWidget(KGameRenderer *renderer, QWidget *parent) :
    QDeclarativeView(parent),
    m_renderer(renderer)
{
    CanvasItem::setRenderer(m_renderer);
    qmlRegisterType<CanvasItem>("SvgLibrary", 1, 0, "CanvasItem");

    KDeclarative kdeclarative;
    kdeclarative.setDeclarativeEngine(engine());
    kdeclarative.initialize();
    // binds things like i18n and icons
    kdeclarative.setupBindings();

    setResizeMode(QDeclarativeView::SizeRootObjectToView);
    QString path = KStandardDirs::locate("appdata", "qml/main.qml");
    setSource(QUrl::fromLocalFile(path));

    // forward signals from QML
    connect(rootObject(), SIGNAL(levelComplete()), this, SIGNAL(levelComplete()));
    connect(rootObject(), SIGNAL(gameEnded(int,int,int)), this, SIGNAL(gameEnded(int,int,int)));
}

void CanvasWidget::newGame()
{
    QMetaObject::invokeMethod(rootObject(), "reset");
}

void CanvasWidget::showLine(QString line, int lineNumber)
{
    QMetaObject::invokeMethod(rootObject(), "loadLine",
                              Q_ARG(QVariant, line),
                              Q_ARG(QVariant, lineNumber));
}

void CanvasWidget::putGift(QString gift, int times, QString pos)
{
    QMetaObject::invokeMethod(rootObject(), "loadGift",
                              Q_ARG(QVariant, gift),
                              Q_ARG(QVariant, times),
                              Q_ARG(QVariant, pos));
}

void CanvasWidget::startGame()
{
    QMetaObject::invokeMethod(rootObject(), "startGame");
}

void CanvasWidget::fire()
{
    QMetaObject::invokeMethod(rootObject(), "fire");
}

void CanvasWidget::setGamePaused(bool paused)
{
    QMetaObject::invokeMethod(rootObject(), "setGamePaused",
                              Q_ARG(QVariant, paused));
}

void CanvasWidget::updateBarDirection()
{
    QMetaObject::invokeMethod(rootObject(), "updateBarDirection",
                              Q_ARG(QVariant, m_barDirection));
}

void CanvasWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        QDeclarativeView::keyPressEvent(event);
        return;
    }
    int key = event->key();
    switch (key) {
    case Qt::Key_Right:
        m_rightPressed = true;
        m_barDirection = 1;
        break;
    case Qt::Key_Left:
        m_leftPressed = true;
        m_barDirection = -1;
        break;
    default:
        QDeclarativeView::keyPressEvent(event);
        return;
    }

    updateBarDirection();
}

void CanvasWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        QDeclarativeView::keyReleaseEvent(event);
        return;
    }
    int key = event->key();
    switch (key) {
    case Qt::Key_Right:
        m_rightPressed = false;
        break;
    case Qt::Key_Left:
        m_leftPressed = false;
        break;
    default:
        QDeclarativeView::keyReleaseEvent(event);
        return;
    }

    if (!m_rightPressed && !m_leftPressed) {
        m_barDirection = 0;
    } else if (m_rightPressed && !m_leftPressed) {
        m_barDirection = 1;
    } else if (!m_rightPressed && m_leftPressed) {
        m_barDirection = -1;
    }

    updateBarDirection();
}

void CanvasWidget::focusOutEvent(QFocusEvent *event)
{
    emit focusLost();
    QDeclarativeView::focusOutEvent(event);
}
