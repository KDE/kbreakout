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

#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include <KgDeclarativeView>
class KgThemeProvider;
class CanvasWidget : public KgDeclarativeView
{
    Q_OBJECT
public:
    explicit CanvasWidget(QWidget *parent = 0);
    virtual ~CanvasWidget();
    KgThemeProvider *getProvider()
    {
        return m_provider;
    }

signals:
    void levelComplete();
    void gameEnded(int score, int level, int elapsedTime);
    void focusLost();
    void mousePressed();

public slots:
    void fire();
    void cheatSkipLevel();
    void cheatAddLife();
    void setGamePaused(bool paused);
    void updateFireShortcut();

public slots:
    void newGame();
    void showLine(QString line, int lineNumber);
    void putGift(QString gift, int times, QString pos);
    void updateBarDirection();
    void startGame();
    void updateCursor();
    void resetCursor();
    void resetMousePosition();

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    //used when moving the bar with the keys
    int m_barDirection;

    // used to track which direction keys are pressed between key events
    bool m_rightPressed;
    bool m_leftPressed;
    KgThemeProvider *m_provider;
};

#endif //CANVASWIDGET_H
