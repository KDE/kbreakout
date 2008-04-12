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

#include <QMouseEvent>
#include <QKeyEvent>
#include <QCursor>

#include <KDebug>

#include "canvaswidget.h"
#include "item.h"
#include "renderer.h"
#include "globals.h"

CanvasWidget::CanvasWidget(QWidget *parent) 
    : KGameCanvasWidget(parent) 
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
    updateBarTimer.stop();
    moveBarTimer.stop();
    pauseOverlay.raise();
    pauseOverlay.show();
    setCursor(QCursor(Qt::ArrowCursor));
}

void CanvasWidget::handleGameResumed(int barPosition)
{
    // give feedback
    QCursor newCursor(Qt::BlankCursor);
    setCursor(newCursor);
    pauseOverlay.hide();
    
    // move the mouse cursor to where the bar is
    int screenY = mapFromGlobal(cursor().pos()).y();
    int screenX = qRound(barPosition * Item::scale()) + Item::borderLeft();
    QPoint p = mapToGlobal(QPoint(screenX, screenY));
    newCursor.setPos(p.x(), p.y());
    setCursor(newCursor);
    updateBarTimer.start();
}

void CanvasWidget::resizeEvent (QResizeEvent */*event*/)
{
    kDebug() << "resized!\n";
    reloadSprites();
}

void CanvasWidget::updateBar()
{
    QPoint p = mapFromGlobal(cursor().pos());
    if (lastMousePosition == p) return;
    lastMousePosition = p;
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
        barDirection = 1;
        moveBarTimer.start();
        break;
    case Qt::Key_Left:
        barDirection = -1;
        moveBarTimer.start();
        break;
    default:
        KGameCanvasWidget::keyPressEvent(event);
    }
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
    case Qt::Key_Left:
        moveBarTimer.stop();
        break;
    default:
        KGameCanvasWidget::keyReleaseEvent(event);
    }
}

void CanvasWidget::focusOutEvent(QFocusEvent *event)
{
    emit focusLost();
    KGameCanvasWidget::focusOutEvent(event);
}
