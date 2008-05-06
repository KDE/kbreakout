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

#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include <QTimer>

#include <KGameCanvas>

class CanvasWidget : public KGameCanvasWidget
{
    Q_OBJECT
public:
    CanvasWidget(QWidget *parent=0);

    // reloads the sprites
    void reloadSprites();
    
signals:
    void spritesReloaded();
    // the position is in game coordinates not screen coordinates
    void mouseMoved(int positionX); // TODO: rename
    void ballFired();
    void barMovedLeft();
    void barMovedRight();
    void pausePressed();
    void escPressed();
    void focusLost();
    // cheating keys, debugging and testing only TODO: REMOVE
    void cheatSkipLevel();
    void cheatAddLife();

private slots:
    void moveBar();
    void updateBar();
    void handleGamePaused();
    void handleGameResumed(int barPosition);

protected:
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void focusOutEvent(QFocusEvent *event);
    
    // TODO: use QTimeLine
    QTimer moveBarTimer; // when using the keyboard
    QTimer updateBarTimer; // when using the mouse
    QPoint lastMousePosition;
    
    KGameCanvasPixmap background;
    KGameCanvasPixmap pauseOverlay;
    
    // used when moving the bar with the keys
    int barDirection;
};

#endif //CANVASWIDGET_H
