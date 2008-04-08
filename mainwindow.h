#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>
#include <KTextEdit>
#include <KDebug>

class GameEngine;
//class Scene; 
class View;
class CanvasWidget;

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent=0);
    ~MainWindow();

public slots:
    void handleEndedGame(int score, int level, int time);
    void handleGamePaused();
    void handleGameResumed(int barPosition);
    void configureSettings();
    void loadSettings();


private:
    CanvasWidget *canvasWidget;
    GameEngine *gameEngine;
    
    void setupActions();
    
protected:
    void focusOutEvent(QFocusEvent * event);
};
 
#endif
