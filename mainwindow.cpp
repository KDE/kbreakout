#include <QAction>
#include <QGraphicsScene>
#include <QSvgRenderer>
#include <QCursor>

#include <KIcon>
#include <KStandardAction>
#include <KActionCollection>
#include <KLocale>
#include <KStandardDirs>
#include <KMessageBox>
#include <KConfigDialog>
#include <KDE/KScoreDialog>
#include <KDE/KGameThemeSelector>
#include <KDE/KStandardGameAction>

#include "mainwindow.h"
#include "gameengine.h"
#include "canvasitems.h"
#include "canvaswidget.h"
#include "renderer.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent) 
    : KXmlGuiWindow(parent),
      canvasWidget(new CanvasWidget(this))
{
    // TODO: find a better way..
    Item::setCanvas(canvasWidget);
    new Background; // the background put's itself into the canvasWidget
    gameEngine = new GameEngine; // must be called after Item::setCanvas()
    
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
    connect(canvasWidget, SIGNAL(focusLost()),
            gameEngine, SLOT(pause()));
    
    connect(gameEngine, SIGNAL(gameEnded(int,int,int)), 
            SLOT(handleEndedGame(int,int,int)));
    
    connect(gameEngine, SIGNAL(gamePaused()), 
            canvasWidget, SLOT(handleGamePaused()));
    connect(gameEngine, SIGNAL(gameResumed(int)), 
            canvasWidget, SLOT(handleGameResumed(int)));
    
    // cheating keys, debugging and testing only TODO: REMOVE
    connect(canvasWidget, SIGNAL(cheatSkipLevel()),
            gameEngine, SLOT(cheatSkipLevel()));
    connect(canvasWidget, SIGNAL(cheatAddLife()),
            gameEngine, SLOT(cheatAddLife()));
    
    setCentralWidget(canvasWidget);
    canvasWidget->setCursor(QCursor(Qt::BlankCursor));
    
    setupActions();
    
    QSize defaultSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    setupGUI(defaultSize, 
        KXmlGuiWindow::Keys | KXmlGuiWindow::Save| KXmlGuiWindow::Create);
    
    gameEngine->start("default");
}
 
MainWindow::~MainWindow()
{
    delete gameEngine;
    delete canvasWidget;
}
 
void MainWindow::setupActions()
{
    KStandardGameAction::quit(this, SLOT(close()), actionCollection());
    
    KStandardGameAction::highscores(this, SLOT(showHighscores()), 
                                actionCollection());
    
    KStandardAction::preferences(this, SLOT(configureSettings()), 
                                actionCollection());
}

void MainWindow::configureSettings()
{
    if (KConfigDialog::showDialog("settings")) {
        return;
    }
    
    KConfigDialog *dialog = new KConfigDialog(this, "settings", 
                                              Settings::self());
    
    dialog->addPage(new KGameThemeSelector( dialog, Settings::self(), 
                    KGameThemeSelector::NewStuffDisableDownload ), 
                    i18n("Theme"), "games-config-theme" );

    connect(dialog, SIGNAL( settingsChanged(const QString&)), this, 
            SLOT(loadSettings()));
    
    dialog->show();
}

void MainWindow::loadSettings() 
{
    if (!Renderer::self()->loadTheme(Settings::theme())) {
        KMessageBox::error(this,  
           i18n( "Failed to load \"%1\" theme. Please check your installation.",
           Settings::theme()));
        return;
    }
    
    canvasWidget->reloadSprites();
}

void MainWindow::showHighscores()
{
    KScoreDialog ksdialog(KScoreDialog::Name | KScoreDialog::Level 
                          | KScoreDialog::Time, this);
    ksdialog.exec();
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
    ksdialog.exec();
    
    gameEngine->start("default");
}
