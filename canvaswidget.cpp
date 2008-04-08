#include <QMouseEvent>
#include <QKeyEvent>
#include <QSvgRenderer>
#include <QCursor>

#include <KDebug>

#include "canvaswidget.h"
#include "item.h"
#include "renderer.h"

CanvasWidget::CanvasWidget(QWidget *parent) 
    : KGameCanvasWidget(parent) 
{
    //setMouseTracking(true);
    grabKeyboard();
    //grabMouse();
    
    background.putInCanvas(this);
    background.show();
    pauseOverlay.putInCanvas(this);
    
    moveBarTimer.setInterval(UPDATE_INTERVAL);
    connect(&moveBarTimer, SIGNAL(timeout()), SLOT(moveBar()));
    
    updateBarTimer.setInterval(UPDATE_INTERVAL);
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
    pauseOverlay.raise();
    pauseOverlay.show();
    setCursor(QCursor(Qt::ArrowCursor));
}

void CanvasWidget::handleGameResumed(int /*barPosition*/)
{
    setCursor(QCursor(Qt::BlankCursor));
    pauseOverlay.hide();
    updateBarTimer.start();
}

void CanvasWidget::resizeEvent (QResizeEvent */*event*/)
{
    kDebug() << "resized!\n";
    reloadSprites();
}

/*void CanvasWidget::mouseMoveEvent(QMouseEvent *event)
{
    // convert the screen position to scene position
    // TODO: put scaling somewhere else... (???)
    int posX = qRound((event->x() - Item::borderLeft()) / Item::scale());
    emit mouseMoved(posX);
}*/

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

void CanvasWidget::mousePressEvent(QMouseEvent */*event*/)
{
    emit ballFired();
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
    case Qt::Key_Escape:
        emit escPressed();
        break;
    case Qt::Key_Pause:
    case Qt::Key_P:
        emit pausePressed();
        break;
    case Qt::Key_Space:
        emit ballFired();
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
