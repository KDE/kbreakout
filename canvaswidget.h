#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include <QTimer>

#include <KDE/KGameCanvas>

class CanvasWidget : public KGameCanvasWidget
{
    Q_OBJECT
public:
    CanvasWidget(QWidget *parent=0);

    // reloads the sprite
    void loadSprite();
    
signals:
    void spriteReloaded();
    // the position is in game coordinates not screen coordinates
    void mouseMoved(int positionX); // TODO: rename
    void ballFired();
    void barMovedLeft();
    void barMovedRight();
    void pausePressed();
    void escPressed();
    // cheating keys, debugging and testing only TODO: REMOVE
    void cheatSkipLevel();
    void cheatAddLife();

private slots:
    void moveBar();
    void updateBar();

protected:
    void resizeEvent(QResizeEvent *event);
    //void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    
    // TODO: use QTimeLine
    QTimer moveBarTimer;
    QTimer updateBarTimer;
    QPoint lastMousePosition;
    
    KGameCanvasPixmap background;
    
    // used when moving the bar with the keys
    int barDirection;
};

#endif //CANVASWIDGET_H
