/*
    SPDX-FileCopyrightText: 2007-2009 Fela Winkelmolen <fela.kde@gmail.com>
    SPDX-FileCopyrightText: 2010 Brian Croom <brian.s.croom@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "mainwindow.h"

// own
#include "gameengine.h"
#include "canvaswidget.h"
#include "ui_generalsettings.h"
#include "settings.h"
#include "globals.h"
// KDEGames
#include <KScoreDialog>
#include <KgThemeSelector>
#include <KStandardGameAction>
// KF
#include <KStandardAction>
#include <KToggleFullScreenAction>
#include <KActionCollection>
#include <KMessageBox>
#include <KConfigDialog>
#include <KConfig>
// Qt
#include <QPointer>
#include <QMenuBar>
#include <QIcon>
#include <QAction>
#include <QKeySequence>

class GeneralSettings : public QWidget
{
    Q_OBJECT
public:
    explicit GeneralSettings(QWidget *parent)
        : QWidget(parent)
    {
        ui.setupUi(this);
    }
    ~GeneralSettings() override {}
private:
    Ui::GeneralSettings ui;
};

MainWindow::MainWindow(QWidget *parent)
    : KXmlGuiWindow(parent),
      canvasWidget(new CanvasWidget(this))
{
    gameEngine = new GameEngine(this);

    m_cheatsEnabled = !qEnvironmentVariableIsEmpty("KDE_DEBUG");

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
             KXmlGuiWindow::Keys | KXmlGuiWindow::Save | KXmlGuiWindow::Create);

    if (isFullScreen()) {
        menuBar()->hide();
    } else {
        menuBar()->show();
    }

    // show here (instead of in main) else the mouse can't be grabbed
    show();
    gameEngine->start(QStringLiteral("default"));
}

MainWindow::~MainWindow()
{
    delete gameEngine;
    delete canvasWidget;
}

void MainWindow::setupActions()
{
    KStandardGameAction::gameNew(this, &MainWindow::startNewGame, actionCollection());

    KStandardGameAction::quit(this, &MainWindow::close, actionCollection());

    KStandardGameAction::highscores(this, &MainWindow::showHighscores, actionCollection());

    KStandardAction::preferences(this, &MainWindow::configureSettings, actionCollection());

    QAction *fullScreenAction = KStandardAction::fullScreen(this,
                                &MainWindow::viewFullScreen, this, actionCollection());
    // set the default primary shortcut as alternate shortcut
    // and make F the default
    QKeySequence fullScreenShortcut(i18nc("Key (shortcut) to toggle full screen", "F"));
    fullScreenAction->setShortcut(fullScreenShortcut);
    QAction *fireAction = new QAction(this);
    fireAction->setText(i18n("Fire the ball"));
    actionCollection()->setDefaultShortcut(fireAction, Qt::Key_Space);
    fireAction->setIcon(QIcon::fromTheme(QStringLiteral("kbreakout")));
    connect(fireAction, &QAction::triggered, this, &MainWindow::fire);
    connect(fireAction, &QAction::changed, canvasWidget, &CanvasWidget::updateFireShortcut);
    actionCollection()->addAction(QStringLiteral("fire"), fireAction);

    if (m_cheatsEnabled) {
        QAction *cheatSkipLevelAction = new QAction(this);
        cheatSkipLevelAction->setText(i18n("Skip level"));
        actionCollection()->setDefaultShortcut(cheatSkipLevelAction, Qt::Key_S);
        cheatSkipLevelAction->setIcon(QIcon::fromTheme(QStringLiteral("kbreakout")));
        connect(cheatSkipLevelAction, &QAction::triggered, this, &MainWindow::cheatSkipLevel);
        actionCollection()->addAction(QStringLiteral("cheatSkipLevel"), cheatSkipLevelAction);

        QAction *cheatAddLifeAction = new QAction(this);
        cheatAddLifeAction->setText(i18n("Add life"));
        actionCollection()->setDefaultShortcut(cheatAddLifeAction, Qt::Key_L);
        cheatAddLifeAction->setIcon(QIcon::fromTheme(QStringLiteral("kbreakout")));
        connect(cheatAddLifeAction, &QAction::triggered, this, &MainWindow::cheatAddLife);
        actionCollection()->addAction(QStringLiteral("cheatAddLife"), cheatAddLifeAction);
    }

    pauseAction = KStandardGameAction::pause(this, &MainWindow::setGamePaused, actionCollection());
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
    if (KConfigDialog::showDialog(QStringLiteral("settings"))) {
        return;
    }
    // else it doesn't exist, thus create the dialog

    KConfigDialog *dialog = new KConfigDialog(this, QStringLiteral("settings"),
            Settings::self());
    dialog->setModal(true);

    dialog->addPage(new KgThemeSelector(canvasWidget->getProvider()),
                    i18n("Theme"), QStringLiteral("games-config-theme"));

    // TODO: when will the page be destroyed?
    dialog->addPage(new GeneralSettings(dialog),
                    i18nc("General settings", "General"),
                    QStringLiteral("games-config-options"));

    dialog->show();
}

void MainWindow::showHighscores()
{
    KScoreDialog ksdialog(KScoreDialog::Name | KScoreDialog::Level, this);
    ksdialog.addField(KScoreDialog::Custom1, i18n("   Time (hh:mm)"), QStringLiteral("moves"));

    ksdialog.exec();
}

void MainWindow::startNewGame()
{
    int ret = KMessageBox::warningTwoActions(
                  this,
                  i18n("Starting a new game will end the current one!"),
                  i18n("New Game"),
                  KGuiItem(i18n("Start a New Game")),
                  KStandardGuiItem::cancel());

    if (ret == KMessageBox::PrimaryAction) {
        pauseAction->setChecked(false);
        gameEngine->start(QStringLiteral("default"));
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
    QString timeString = t.toString(QStringLiteral("HH:mm"));

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
    ksdialog->addField(KScoreDialog::Custom1, i18n("Time (hh:mm)"), QStringLiteral("moves"));
    ksdialog->addScore(scoreInfo);
    ksdialog->exec();

    if (ksdialog) {
        gameEngine->start(QStringLiteral("default"));
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

void MainWindow::cheatSkipLevel()
{
    if (pauseAction->isChecked()) {
        pauseAction->activate(QAction::Trigger);
    } else {
        canvasWidget->cheatSkipLevel();
    }
}

void MainWindow::cheatAddLife()
{
    if (pauseAction->isChecked()) {
        pauseAction->activate(QAction::Trigger);
    } else {
        canvasWidget->cheatAddLife();
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
        KConfigGroup group(&config, "Notification Messages");
        if (group.hasKey("dontAskFireOnClick")) {
            dontAsk = true;
        }
    }
#endif
    if (dontAsk == false) {
        // ask the user if he wants to fire on mouse click
        int res = KMessageBox::questionTwoActions(
                      this,
                      i18n("Do you want to fire the ball on mouse click?\n"
                           "Answering Yes will make the game steal the\n"
                           "mouse cursor, pause the game to get\n"
                           "the cursor back."),
                      i18n("Fire on click?"),
                      KGuiItem(i18nc("@action;button", "Use Mouse Click"), QStringLiteral("input-mouse")),
                      KGuiItem(i18nc("@action;button", "Ignore Mouse Click"), QStringLiteral("dialog-cancel")),
                      QStringLiteral("dontAskFireOnClick") // doesntAskAgainName
                  );

        if (res == KMessageBox::PrimaryAction) {
            Settings::setFireOnClick(true);
            Settings::self()->save();
        }
    }
}

#include "mainwindow.moc"
#include "moc_mainwindow.cpp"
