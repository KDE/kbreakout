/*
    SPDX-FileCopyrightText: 2012 Viranch Mehta <viranch.mehta@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "canvaswidget.h"

// own
#include "globals.h"
#include "kbreakout_debug.h"
#include "settings.h"
// KDEGames
#include <kdegames_version.h>
#include <KgThemeProvider>
// Qt
#include <QGraphicsObject>
#include <QAction>
#include <QCursor>
#include <QStandardPaths>
#include <QQuickItem>

CanvasWidget::CanvasWidget(QWidget *parent) :
    KgDeclarativeView(parent),
    m_provider(new KgThemeProvider)
{
#if KDEGAMES_VERSION >= QT_VERSION_CHECK(7, 4, 0)
    m_provider->discoverThemes(QStringLiteral("themes"));
#else
    m_provider->discoverThemes("appdata", QStringLiteral("themes"));
#endif
    m_provider->setDeclarativeEngine(QStringLiteral("themeProvider"), engine());
    QString path = QStandardPaths::locate(QStandardPaths::AppDataLocation, QStringLiteral("qml/main.qml"));

    qCDebug(KBREAKOUT_General) << "QtQuick QML file: " << path;

    setSource(QUrl::fromLocalFile(path));

    // forward signals from QML
    connect(rootObject(), SIGNAL(levelComplete()), this, SIGNAL(levelComplete()));
    connect(rootObject(), SIGNAL(gameEnded(int,int,int)), this, SIGNAL(gameEnded(int,int,int)));
    connect(rootObject(), SIGNAL(mousePressed()), this, SIGNAL(mousePressed()));

    // for handling mouse cursor
    connect(rootObject(), SIGNAL(ballMovingChanged()), this, SLOT(updateCursor()));
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

bool CanvasWidget::event(QEvent *event)
{
    if (event->type() == QEvent::Leave) {
        updateCursor();
    }
    return KgDeclarativeView::event(event);
}

void CanvasWidget::newGame()
{
    QMetaObject::invokeMethod(rootObject(), "reset");
}

void CanvasWidget::showLine(const QString &line, int lineNumber)
{
    QMetaObject::invokeMethod(rootObject(), "loadLine",
                              Q_ARG(QVariant, line),
                              Q_ARG(QVariant, lineNumber));
}

void CanvasWidget::putGift(const QString &gift, int times, const QString &pos)
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

void CanvasWidget::updateCursor()
{
    const bool ballMoving = rootObject()->property("ballMoving").toBool();

    if (ballMoving) {
        resetMousePosition();
        setCursor(Qt::BlankCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

void CanvasWidget::resetMousePosition()
{
    const QQuickItem *jailItem = rootObject()->property("jailItem").value<QQuickItem*>();
    const QPoint jailItemTopLeft = mapToGlobal(QPoint(jailItem->x(), jailItem->y()));
    QPoint p = QCursor::pos();
    if (p.x() < jailItemTopLeft.x()) {
        QCursor::setPos(jailItemTopLeft.x(), p.y());
    } else if (p.x() > jailItemTopLeft.x() + jailItem->width()) {
        QCursor::setPos(jailItemTopLeft.x() + jailItem->width(), p.y());
    }

    if (p.y() < jailItemTopLeft.y()) {
        QCursor::setPos(p.x(), jailItemTopLeft.y());
    } else if (p.y() > jailItemTopLeft.y() + jailItem->height()) {
        QCursor::setPos(p.x(), jailItemTopLeft.y() + jailItem->height());
    }
}

void CanvasWidget::focusOutEvent(QFocusEvent *event)
{
    Q_EMIT focusLost();
    QQuickWidget::focusOutEvent(event);
}
