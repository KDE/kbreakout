#include <QAction>
#include <QGraphicsScene>
#include <QSvgRenderer>

#include <KIcon>
#include <KStandardAction>
#include <KActionCollection>
#include <KLocale>
#include <KStandardDirs>
#include <KMessageBox>
#include <KDE/KScoreDialog>

#include "mainwindow.h"
#include "gameengine.h"
#include "canvasitems.h"
#include "canvaswidget.h"
#include "renderer.h"

MainWindow::MainWindow(QWidget *parent) : KXmlGuiWindow(parent)
{
    canvasWidget = new CanvasWidget(this);

    QSvgRenderer *svg = new QSvgRenderer(
            KStandardDirs::locate("appdata", "default.svg"));
    Item::setSvgRenderer(svg);
    Item::setCanvas(canvasWidget);
    new Background;
    gameEngine = new GameEngine;
    
    connect(canvasWidget, SIGNAL(mouseMoved(int)),
            gameEngine, SLOT(moveBar(int)));
    connect(canvasWidget, SIGNAL(ballFired()),
            gameEngine, SLOT(fire()));
    connect(canvasWidget, SIGNAL(barMovedLeft()),
            gameEngine, SLOT(moveBarLeft()));
    connect(canvasWidget, SIGNAL(barMovedRight()),
            gameEngine, SLOT(moveBarRight()));
    connect(canvasWidget, SIGNAL(pausePressed()),
            gameEngine, SLOT(togglePause()));
    
    connect(gameEngine, SIGNAL(gameEnded(int,int,int)), 
            SLOT(handleEndedGame(int,int,int)));
            
    
    // cheating keys, debugging and testing only TODO: REMOVE
    connect(canvasWidget, SIGNAL(cheatSkipLevel()),
            gameEngine, SLOT(cheatSkipLevel()));
    connect(canvasWidget, SIGNAL(cheatAddLife()),
            gameEngine, SLOT(cheatAddLife()));
    
    setCentralWidget(canvasWidget);
    
    setupActions();
    
    Renderer::self()->loadTheme("default");
    gameEngine->start("default");
}
 
//FIXME: add ~MainWindow
 
void MainWindow::setupActions()
{
    KStandardAction::quit(this, SLOT(close()), actionCollection());
    setupGUI();
}

void MainWindow::handleEndedGame(int score, int level, int time)
{
    
    QTime t = QTime(0, 0).addSecs(time);
    // TODO: check int overflow and fix 24 hours "overflow"
    QString timeString = t.toString("HH:mm");    
    kDebug()<<"score: "<<score<<"level: "<<level<<"time: "<<timeString<<endl;
    
    // TODO: fix score < 0
    
    KScoreDialog::FieldInfo scoreInfo;
    scoreInfo[KScoreDialog::Score].setNum(score);
    scoreInfo[KScoreDialog::Level].setNum(level);
    scoreInfo[KScoreDialog::Time] = timeString;
    
    KScoreDialog ksdialog(KScoreDialog::Name | KScoreDialog::Level 
                          | KScoreDialog::Time, this);
    ksdialog.addScore(scoreInfo);
    
    canvasWidget->releaseKeyboard(); // TODO: ???
    ksdialog.exec();
    canvasWidget->grabKeyboard(); // TODO: ???
    
    gameEngine->start("default");
}
