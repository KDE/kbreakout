/*
    Copyright 2007-2009 Fela Winkelmolen <fela.kde@gmail.com> 
  
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

#include "mainwindow.h"
#include "gameengine.h"
#include "canvasitems.h"
#include "canvaswidget.h"
#include "renderer.h"
#include "ui_generalsettings.h"
#include "settings.h"

#include <QGraphicsScene>
#include <QPointer>

#include <KMenuBar>
#include <KToolBar>
#include <KStatusBar>
#include <KIcon>
#include <KAction>
#include <KStandardAction>
#include <KToggleAction>
#include <KToggleFullScreenAction>
#include <KActionCollection>
#include <KShortcut>
#include <KLocale>
#include <KMessageBox>
#include <KConfigDialog>
#include <KScoreDialog>
#include <KGameThemeSelector>
#include <KStandardGameAction>
#include <KConfig>


class GeneralSettings : public QWidget
{
public:
    GeneralSettings(QWidget *parent)
        : QWidget(parent)
    {
        ui.setupUi(this);
    }
private:
    Ui::GeneralSettings ui;
};

MainWindow::MainWindow(QWidget *parent) 
    : KXmlGuiWindow(parent),
      canvasWidget(new CanvasWidget(this))
{
    // TODO: find a better way..
    Item::setCanvas(canvasWidget);
    new Background; // the background put's itself into the canvasWidget
    gameEngine = new GameEngine(this); // must be called after Item::setCanvas()
    
    connect(canvasWidget, SIGNAL(mouseMoved(int)),
            gameEngine, SLOT(moveBar(int)));
    connect(canvasWidget, SIGNAL(barMovedLeft()),
            gameEngine, SLOT(moveBarLeft()));
    connect(canvasWidget, SIGNAL(barMovedRight()),
            gameEngine, SLOT(moveBarRight()));
    connect(canvasWidget, SIGNAL(focusLost()),
            gameEngine, SLOT(pause()));
    
    connect(gameEngine, SIGNAL(gamePaused()), 
            canvasWidget, SLOT(handleGamePaused()));
    connect(gameEngine, SIGNAL(gameResumed()),
            canvasWidget, SLOT(handleGameResumed()));
    
    connect(gameEngine, SIGNAL(gameEnded(int,int,int)), 
            SLOT(handleEndedGame(int,int,int)));
    
    // cheating keys, debugging and testing only TODO: REMOVE
    connect(canvasWidget, SIGNAL(cheatSkipLevel()),
            gameEngine, SLOT(cheatSkipLevel()));
    connect(canvasWidget, SIGNAL(cheatAddLife()),
            gameEngine, SLOT(cheatAddLife()));
    
    setCentralWidget(canvasWidget);
    
    setupActions();
    
    QSize defaultSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    setupGUI(defaultSize, 
        KXmlGuiWindow::Keys | KXmlGuiWindow::Save| KXmlGuiWindow::Create);

    // show here (instead of in main) else the mouse can't be grabbed
    show(); 
    gameEngine->start("default");
}
 
MainWindow::~MainWindow()
{
    delete gameEngine;
    delete canvasWidget;
}
 
void MainWindow::setupActions()
{
    KStandardGameAction::gameNew(this, SLOT(startNewGame()), 
                                 actionCollection());
    
    KStandardGameAction::quit(this, SLOT(close()), actionCollection());
    
    KStandardGameAction::highscores(this, SLOT(showHighscores()), 
                                actionCollection());
    
    KStandardAction::preferences(this, SLOT(configureSettings()), 
                                actionCollection());

    KAction *fullScreenAction = KStandardAction::fullScreen(this,
                          SLOT(viewFullScreen(bool)), this, actionCollection());
    // set the default primary shortcut as alternate shortcut
    // and make F the default
    QKeySequence defaultShortcut = fullScreenAction->shortcut().primary();
    QKeySequence newPrimary(i18nc("Key (shortcut) to toggle full screen", "F"));
    KShortcut fullScreenShortcut(newPrimary, defaultShortcut);
    fullScreenAction->setShortcut(fullScreenShortcut);

    KAction *fireAction = new KAction(this);
    fireAction->setText(i18n("Fire the ball"));
    fireAction->setShortcut(Qt::Key_Space);
    fireAction->setIcon(KIcon("kbreakout"));
    connect(fireAction, SIGNAL(triggered()), this, SLOT(fire()));
    actionCollection()->addAction("fire", fireAction);

    pauseAction = KStandardGameAction::pause(this,
              SLOT(setGamePaused(bool)), actionCollection());
    // set custom keys
    QList<QKeySequence> keys;
    keys.append(i18nc("Key (shortcut) to pause the game", "P"));
    keys.append(Qt::Key_Escape);
    // the following won't work (no more than 2 shortcuts allowed..)
    // TODO: make the pause key work
    //keys.append(Qt::Key_Pause);
    pauseAction->setShortcut(KShortcut(keys));
}

void MainWindow::configureSettings()
{
    if (KConfigDialog::showDialog("settings")) {
        return;
    }
    // else it doesn't exist, thus create the dialog
    
    KConfigDialog *dialog = new KConfigDialog(this, "settings", 
                                              Settings::self());
    dialog->setModal(true);
    
    dialog->addPage(new KGameThemeSelector( dialog, Settings::self(), 
                    KGameThemeSelector::NewStuffDisableDownload ), 
                    i18n("Theme"), "games-config-theme" );
    
    // TODO: when will the page be destroyed?
    dialog->addPage(new GeneralSettings( dialog ), 
                    i18nc("General settings", "General"),
                    "games-config-options");

    connect(dialog, SIGNAL(settingsChanged(const QString&)), 
            this, SLOT(loadSettings()));
    
    dialog->show();
}

void MainWindow::loadSettings() 
{
    if (!Renderer::self()->loadTheme(Settings::theme())) {
        KMessageBox::error(this,  
           i18n("Failed to load \"%1\" theme. Please check your installation.",
           Settings::theme()));
        return;
    }
    
    canvasWidget->reloadSprites();
}

void MainWindow::showHighscores()
{
    KScoreDialog ksdialog(KScoreDialog::Name | KScoreDialog::Level, this);
    ksdialog.addField(KScoreDialog::Custom1, i18n("   Time (hh:mm)"), "moves");

    ksdialog.exec();
}

void MainWindow::startNewGame()
{
    int ret = KMessageBox::warningYesNo(
        this,
        i18n("Starting a new game will end the current one!"),
        i18n("New Game"),
        KGuiItem(i18n("Start a New Game")),
        KStandardGuiItem::cancel());
        
    if (ret == KMessageBox::Yes) {
        gameEngine->start("default");
    }
}

void MainWindow::setGamePaused(bool paused)
{
    gameEngine->setGamePaused(paused);
}

void MainWindow::fire()
{
    if (gameEngine->gameIsPaused()) {
        pauseAction->activate(QAction::Trigger);
    } else {
        gameEngine->fire();
    }
}

void MainWindow::handleEndedGame(int score, int level, int time)
{
    
    QTime t = QTime(0, 0).addSecs(time);
    // TODO: check int overflow and fix 24 hours "overflow"
    QString timeString = t.toString("HH:mm");
    
    const int ALL_LEVELS = -1;
    
    KScoreDialog::FieldInfo scoreInfo;
    scoreInfo[KScoreDialog::Score].setNum(score);
    scoreInfo[KScoreDialog::Custom1] = timeString;
    if (level == ALL_LEVELS) {
        scoreInfo[KScoreDialog::Level] = i18n("Game won!");
    } else {
        scoreInfo[KScoreDialog::Level].setNum(level);
    }
    
    canvasWidget->handleGameEnded();
    
    QPointer<KScoreDialog> ksdialog =
            new KScoreDialog(KScoreDialog::Name | KScoreDialog::Level, this);
    ksdialog->addField(KScoreDialog::Custom1, i18n("Time (hh:mm)"), "moves");
    ksdialog->addScore(scoreInfo);
    ksdialog->exec();
    
    if ( ksdialog ) {
        canvasWidget->handleGameResumed();
        gameEngine->start("default");
        delete ksdialog;
    }
}

void MainWindow::viewFullScreen(bool fullScreen)
{
    KToggleFullScreenAction::setFullScreen(this, fullScreen);
    if (fullScreen) {
        menuBar()->hide();
    } else {
        menuBar()->show();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (gameEngine->gameIsPaused()) {
        pauseAction->activate(QAction::Trigger);
        KXmlGuiWindow::mousePressEvent(event);
        return;
    }

    if (Settings::fireOnClick()) {
        gameEngine->fire();
        KXmlGuiWindow::mousePressEvent(event);
        return;
    }

    // not fire on click
    
    // check if dontAskFireOnClick is set
    // we want to override it's default effect
    // if it's set to _any_ value we want to keep the
    // settings of fireOnClick at false
    bool dontAsk = false; // true if dontAskFireOnClick was set
    KConfig config(componentData(), "kbreakoutrc");
    if (config.hasGroup("Notification Messages")) {
        KConfigGroup group( &config, "Notification Messages");
        if (group.hasKey("dontAskFireOnClick"))
            dontAsk = true;
    }
    
    if (dontAsk == false) {
        // ask the user if he wants to fire on mouse click
        int res = KMessageBox::questionYesNo(  
                    this, 
                    i18n("Do you want to fire the ball on mouse click?\n"
                        "Answering Yes will make the game steal the\n"
                        "mouse cursor, pause the game to get\n"
                        "the cursor back."), 
                    i18n("Fire on click?"), 
                    KStandardGuiItem::yes(), 
                    KStandardGuiItem::no(), 
                    "dontAskFireOnClick" // doesntAskAgainName 
        );
        
        if (res == KMessageBox::Yes) {
            Settings::setFireOnClick(true); 
            Settings::self()->writeConfig();
            canvasWidget->handleGameResumed();
        }
    }
    
    KXmlGuiWindow::mousePressEvent(event);
}

