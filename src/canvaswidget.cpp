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
#include "globals.h"
#include "kbreakout_debug.h"
#include "settings.h"

#include <QGraphicsObject>
#include <QAction>
#include <QCursor>
#include <QStandardPaths>
#include <QQuickItem>

#include <KgThemeProvider>

CanvasWidget::CanvasWidget(QWidget *parent) :
    KgDeclarativeView(parent),
    m_provider(new KgThemeProvider)
{
    m_provider->discoverThemes("appdata", QStringLiteral("themes"));
    m_provider->setDeclarativeEngine(QStringLiteral("themeProvider"), engine());
    QString path = QStandardPaths::locate(QStandardPaths::AppDataLocation, QStringLiteral("qml/main.qml"));

    qCDebug(KBREAKOUT_General) << "QtQuick QML file: " << path;

    setSource(QUrl::fromLocalFile(path));

    // forward signals from QML
    connect(rootObject(), SIGNAL(levelComplete()), this, SIGNAL(levelComplete()));
    connect(rootObject(), SIGNAL(gameEnded(int,int,int)), this, SIGNAL(gameEnded(int,int,int)));
    connect(rootObject(), SIGNAL(mousePressed()), this, SIGNAL(mousePressed()));

    // for handling mouse cursor
    connect(rootObject(), SIGNAL(pausedChanged()), this, SLOT(updateCursor()));
    connect(this, &CanvasWidget::gameEnded, this, &CanvasWidget::resetCursor);
}

CanvasWidget::~CanvasWidget()
{
    delete m_provider;
}

void CanvasWidget::updateFireShortcut()
{
    QAction *fireAction = qobject_cast<QAction *>(sender());
    QString shortcut = fireAction->shortcut().toString(QKeySequence::NativeText);
    rootObject()->setProperty("fireShortcut", shortcut);
}

void CanvasWidget::resizeEvent(QResizeEvent *event)
{
    QQuickWidget::resizeEvent(event);
    QMetaObject::invokeMethod(rootObject(), "updateGeometry");
}

void CanvasWidget::newGame()
{
    QMetaObject::invokeMethod(rootObject(), "reset");

    setCursor(QCursor(Qt::BlankCursor));
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

void CanvasWidget::cheatSkipLevel()
{
    QMetaObject::invokeMethod(rootObject(), "cheatSkipLevel");
}

void CanvasWidget::cheatAddLife()
{
    QMetaObject::invokeMethod(rootObject(), "cheatAddLife");
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

void CanvasWidget::resetCursor()
{
    setCursor(QCursor(Qt::ArrowCursor));
}

void CanvasWidget::updateCursor()
{
    bool paused = rootObject()->property("paused").toBool();

    if (paused) {
        resetCursor();
    } else {
        // FIXME: move the cursor to where the bar is
        resetMousePosition();
        QCursor newCursor(Qt::BlankCursor);
        newCursor.setPos(cursor().pos());
        setCursor(newCursor);
    }
}

void CanvasWidget::resetMousePosition()
{
    // FIXME: the cursor's position is supposed to be reset,
    // just doesn't work!
    int barPosition = rootObject()->property("barCenter").toInt();
    QPoint p = mapToGlobal(QPoint(barPosition, 0));
    cursor().setPos(p.x(), cursor().pos().y());
}

void CanvasWidget::focusOutEvent(QFocusEvent *event)
{
    emit focusLost();
    QQuickWidget::focusOutEvent(event);
}
