/*
    Copyright 2007-2008 Fela Winkelmolen <fela.kde@gmail.com> 
  
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
#include "item.h"
#include "gameengine.h"
#include "renderer.h"
#include "globals.h"
#include "settings.h"

#include <QKeyEvent>
#include <QCursor>

#include <KDebug>

CanvasWidget::CanvasWidget(QWidget *parent) 
    : KGameCanvasWidget(parent),
      rightPressed(false),
      leftPressed(false),
      usingKeys(0)
{
    setFocus();
    
    background.putInCanvas(this);
    background.show();
    pauseOverlay.putInCanvas(this);
    
    moveBarTimer.setInterval(DEFAULT_UPDATE_INTERVAL);
    connect(&moveBarTimer, SIGNAL(timeout()), SLOT(moveBar()));
    
    updateBarTimer.setInterval(DEFAULT_UPDATE_INTERVAL);
    connect(&updateBarTimer, SIGNAL(timeout()), SLOT(updateBar()));
    lastMousePosition = QPoint(0, 0);
    
    updateBarTimer.start();
    setCursor(QCursor(Qt::BlankCursor));
}

void CanvasWidget::moveBar()
{
    if (barDirection != 0) {
        // delay to switch from keys to mouse
        const int DELAY = 20;
        usingKeys = DELAY;
    }

    if (barDirection == 1) emit barMovedRight();
    if (barDirection == -1) emit barMovedLeft();
}

void CanvasWidget::reloadSprites()
{
    QSize size(width(), height());
    QPixmap pixmap = Renderer::self()->renderedSvgElement("Background", size);
    background.setPixmap(pixmap);
    
    // pause overlay
    pixmap = QPixmap(size);
    pixmap.fill(QColor(100, 100, 100, 150));
    pauseOverlay.setPixmap(pixmap);
    
    emit spritesReloaded();
}

void CanvasWidget::handleGamePaused()
{
    releaseMouse();
    updateBarTimer.stop();
    moveBarTimer.stop();
    pauseOverlay.raise();
    pauseOverlay.show();
    setCursor(QCursor(Qt::ArrowCursor));
}

void CanvasWidget::handleGameResumed()
{
    // give feedback
    pauseOverlay.hide();
    
    // move the mouse cursor to where the bar is
    handleResetMousePosition();

    QCursor newCursor(Qt::BlankCursor);
    newCursor.setPos(cursor().pos());

    if (Settings::fireOnClick()) {
        grabMouse(newCursor);
    } else {
        setCursor(newCursor);
    }

    updateBarTimer.start();
}

void CanvasWidget::handleGameEnded()
{
    releaseMouse();
    updateBarTimer.stop();
    moveBarTimer.stop();
    setCursor(QCursor(Qt::ArrowCursor));
}

void CanvasWidget::resizeEvent(QResizeEvent */*event*/)
{
    kDebug() << "resized!\n";
    reloadSprites();
}

void CanvasWidget::handleResetMousePosition()
{
    int barPosition = GameEngine::bar().center();
    int screenX = qRound(barPosition * Item::scale()) + Item::borderLeft();
    QPoint p = mapToGlobal(QPoint(screenX, 0));
    cursor().setPos(p.x(), cursor().pos().y());
}

void CanvasWidget::updateBar()
{
    QPoint p = mapFromGlobal(cursor().pos());
    if (lastMousePosition == p) return;
    lastMousePosition = p;

    // don't move the mouse if the user was using the keys to play
    // after a while however (when usingKeys == 0) the mouse gets released
    // this avoids accidentally moving the mouse while playing using the keys
    if (usingKeys > 0) {
        --usingKeys;
        if (usingKeys == 0) {
            handleResetMousePosition();
        }
        return;
    }

    // TODO: put scaling somewhere else... (???)
    // convert the screen position to scene position
    int posX = qRound((p.x() - Item::borderLeft()) / Item::scale());
    emit mouseMoved(posX);
}

void CanvasWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        KGameCanvasWidget::keyPressEvent(event);
        return;
    }
    int key = event->key();
    switch (key) {
    case Qt::Key_S:
        emit cheatSkipLevel();
        break;
    case Qt::Key_L:
        emit cheatAddLife();
        break;
    case Qt::Key_Right:
        rightPressed = true;
        barDirection = 1;
        break;
    case Qt::Key_Left:
        leftPressed = true;
        barDirection = -1;
        break;
    default:
        KGameCanvasWidget::keyPressEvent(event);
    }

    if ((rightPressed || leftPressed) && !moveBarTimer.isActive())
        moveBarTimer.start();
}

void CanvasWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        KGameCanvasWidget::keyReleaseEvent(event);
        return;
    }
    int key = event->key();
    switch (key) {
    case Qt::Key_Right:
        rightPressed = false;
        break;
    case Qt::Key_Left:
        leftPressed = false;
        break;
    default:
        KGameCanvasWidget::keyReleaseEvent(event);
    }

    if (!rightPressed && !leftPressed) {
        moveBarTimer.stop();
    } else if (rightPressed && !leftPressed) {
        barDirection = 1;
    } else if (!rightPressed && leftPressed) {
        barDirection = -1;
    }
}

void CanvasWidget::focusOutEvent(QFocusEvent *event)
{
    emit focusLost();
    KGameCanvasWidget::focusOutEvent(event);
}

