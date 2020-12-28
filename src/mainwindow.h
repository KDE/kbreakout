/*
    SPDX-FileCopyrightText: 2007-2008 Fela Winkelmolen <fela.kde@gmail.com>
    SPDX-FileCopyrightText: 2010 Brian Croom <brian.s.croom@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>

class GameEngine;
class CanvasWidget;
class KToggleAction;

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private Q_SLOTS:
    void handleEndedGame(int score, int level, int time);
    void configureSettings();
    void showHighscores();
    void startNewGame();
    void fire();
    void cheatSkipLevel();
    void cheatAddLife();
    // calls the action
    void pauseGame();
    // called by the action
    void setGamePaused(bool paused);
    void viewFullScreen(bool fullScreen);
    void handleMousePressed();

private:
    bool m_cheatsEnabled;

    void setupActions();

    KToggleAction *pauseAction;

    CanvasWidget *canvasWidget;
    GameEngine *gameEngine;

};

#endif
