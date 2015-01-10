/*
    Copyright 2007-2009 Fela Winkelmolen <fela.kde@gmail.com> 
    Copyright 2010 Brian Croom <brian.s.croom@gmail.com>
  
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
#include "canvaswidget.h"
#include "ui_generalsettings.h"
#include "settings.h"
#include "globals.h"

#include <QPointer>

#include <QMenuBar>
#include <QIcon>
#include <QAction>
#include <KStandardAction>
#include <KToggleAction>
#include <KToggleFullScreenAction>
#include <KActionCollection>
#include <QKeySequence>
#include <KMessageBox>
#include <KConfigDialog>
#include <KScoreDialog>
#include <KgThemeSelector>
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
    gameEngine = new GameEngine(this);
    
    connect(canvasWidget, &CanvasWidget::focusLost, this, &MainWindow::pauseGame);
    
    connect(canvasWidget, &CanvasWidget::levelComplete, gameEngine, &GameEngine::loadNextLevel);
    connect(canvasWidget, &CanvasWidget::gameEnded, this, &MainWindow::handleEndedGame);
    connect(canvasWidget, &CanvasWidget::mousePressed, this, &MainWindow::handleMousePressed);

    connect(gameEngine, &GameEngine::loadingNewGame, canvasWidget, &CanvasWidget::newGame);
    connect(gameEngine, &GameEngine::newLine, canvasWidget, &CanvasWidget::showLine);
    connect(gameEngine, &GameEngine::newGift, canvasWidget, &CanvasWidget::putGift);
    connect(gameEngine, &GameEngine::ready, canvasWidget, &CanvasWidget::startGame);
    
    setCentralWidget(canvasWidget);
    
    setupActions();
    setFocusProxy(canvasWidget);
    
    QSize defaultSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    setupGUI(defaultSize, 
        KXmlGuiWindow::Keys | KXmlGuiWindow::Save| KXmlGuiWindow::Create);

    if (isFullScreen()) {
        menuBar()->hide();
    } else {
        menuBar()->show();
    }

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

    QAction *fullScreenAction = KStandardAction::fullScreen(this,
                          SLOT(viewFullScreen(bool)), this, actionCollection());
    // set the default primary shortcut as alternate shortcut
    // and make F the default
    QKeySequence fullScreenShortcut(i18nc("Key (shortcut) to toggle full screen", "F"));
    fullScreenAction->setShortcut(fullScreenShortcut);
    QAction *fireAction = new QAction(this);
    fireAction->setText(i18n("Fire the ball"));
    actionCollection()->setDefaultShortcut(fireAction,Qt::Key_Space);
    fireAction->setIcon(QIcon::fromTheme( QLatin1String( "kbreakout" )));
    connect(fireAction, &QAction::triggered, this, &MainWindow::fire);
    connect(fireAction, &QAction::changed, canvasWidget, &CanvasWidget::updateFireShortcut);
    actionCollection()->addAction( QLatin1String( "fire" ), fireAction);

    pauseAction = KStandardGameAction::pause(this,
              SLOT(setGamePaused(bool)), actionCollection());
    // set custom keys
    QList<QKeySequence> keys;
    keys.append(i18nc("Key (shortcut) to pause the game", "P"));
    keys.append(Qt::Key_Escape);
    // the following won't work (no more than 2 shortcuts allowed..)
    // TODO: make the pause key work
    //keys.append(Qt::Key_Pause);
    pauseAction->setShortcuts(keys);
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
    
    dialog->addPage(new KgThemeSelector(canvasWidget->getProvider()),
                    i18n("Theme"), "games-config-theme" );
    
    // TODO: when will the page be destroyed?
    dialog->addPage(new GeneralSettings( dialog ), 
                    i18nc("General settings", "General"),
                    "games-config-options");
    
    dialog->show();
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
        pauseAction->setChecked(false);
        gameEngine->start("default");
    }
}

void MainWindow::pauseGame()
{
    if (!pauseAction->isChecked()) {
        pauseAction->activate(QAction::Trigger);
    }
}

void MainWindow::setGamePaused(bool paused)
{
    canvasWidget->setGamePaused(paused);
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
    
    QPointer<KScoreDialog> ksdialog =
            new KScoreDialog(KScoreDialog::Name | KScoreDialog::Level, this);
    ksdialog->addField(KScoreDialog::Custom1, i18n("Time (hh:mm)"), "moves");
    ksdialog->addScore(scoreInfo);
    ksdialog->exec();
    
    if ( ksdialog ) {
        gameEngine->start("default");
        delete ksdialog;
    }
}

void MainWindow::fire()
{
    if (pauseAction->isChecked()) {
        pauseAction->activate(QAction::Trigger);
    } else {
        canvasWidget->fire();
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

void MainWindow::handleMousePressed()
{
    if (pauseAction->isChecked()) {
        pauseAction->activate(QAction::Trigger);
        return;
    }

    if (Settings::fireOnClick()) {
        canvasWidget->fire();
        return;
    }

    // not fire on click
    
    // check if dontAskFireOnClick is set
    // we want to override it's default effect
    // if it's set to _any_ value we want to keep the
    // settings of fireOnClick at false
    bool dontAsk = false; // true if dontAskFireOnClick was set
#if 0 //QT5
    KConfig config(componentData(), "kbreakoutrc");
    if (config.hasGroup("Notification Messages")) {
        KConfigGroup group( &config, "Notification Messages");
        if (group.hasKey("dontAskFireOnClick"))
            dontAsk = true;
    }
    #endif
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
            Settings::self()->save();
        }
    }
}

