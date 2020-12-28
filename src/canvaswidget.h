/*
    SPDX-FileCopyrightText: 2012 Viranch Mehta <viranch.mehta@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include <KgDeclarativeView>
class KgThemeProvider;
class CanvasWidget : public KgDeclarativeView
{
    Q_OBJECT
public:
    explicit CanvasWidget(QWidget *parent = nullptr);
    ~CanvasWidget() override;
    KgThemeProvider *getProvider()
    {
        return m_provider;
    }

Q_SIGNALS:
    void levelComplete();
    void gameEnded(int score, int level, int elapsedTime);
    void focusLost();
    void mousePressed();

public Q_SLOTS:
    void fire();
    void cheatSkipLevel();
    void cheatAddLife();
    void setGamePaused(bool paused);
    void updateFireShortcut();

public Q_SLOTS:
    void newGame();
    void showLine(const QString &line, int lineNumber);
    void putGift(const QString &gift, int times, const QString &pos);
    void updateBarDirection();
    void startGame();
    void updateCursor();
    void resetMousePosition();

protected:
    void focusOutEvent(QFocusEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    bool event(QEvent *event) override;

private:
    //used when moving the bar with the keys
    int m_barDirection;

    // used to track which direction keys are pressed between key events
    bool m_rightPressed;
    bool m_leftPressed;
    KgThemeProvider *m_provider;
};

#endif //CANVASWIDGET_H
