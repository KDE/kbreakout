#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>

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

private slots:
    void handleEndedGame(int score, int level, int time);
    void configureSettings();
    void showHighscores();
    void loadSettings();
    
private:
    void setupActions();
    
    CanvasWidget *canvasWidget;
    GameEngine *gameEngine;
    
protected:
    void mousePressEvent(QMouseEvent *event);
};
 
#endif
