/*
    Copyright 2007-2008 Fela Winkelmolen <fela.kde@gmail.com> 
  
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

#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QObject>
#include <QList>
#include <QTimer>

class MainWindow;

class LevelLoader;

// TODO: add m_ to all members

class GameEngine : public QObject
{
    Q_OBJECT
public:
    GameEngine(MainWindow *);
    ~GameEngine();
    
public slots:
    void start(const QString &levelset);
    void pause();
    void resume();
    
    // cheating keys for debugging and testing. Enabled only when
    // KDE_DEBUG environment variable is true.
    void cheatSkipLevel();
    void cheatAddLife();

signals:
    void newLine(QString line, int lineNumber);
    void newGift(QString gift, int times, QString pos);
    void ready();
    void gamePaused();
    void gameResumed();
    void gameEnded(int score, int level, int time);
    void resetMousePosition();

private:
    // convenience functions
    void showFireBallMessage();
    void deleteMovingObjects();
    void deleteAllObjects();

    MainWindow *m_mainWindow; // needed to access actionCollection()
    int m_level;
    int m_score;
    // score to add if you hit a brick
    // decreases over time since last hit
    qreal m_dScore;
    // count of remaining bricks
    // (not counting the unbreakable ones)
    int m_remainingBricks;
    
    // Canvas Items
    bool m_gameOver;
    bool m_gameWon;
    bool m_cheatsEnabled;
    LevelLoader *m_levelLoader;

    // is set to true when deleteMovingObjects() is called
    bool m_itemsGotDeleted;

private slots:
    // hides the current showed message by m_messageBox 
    // unless the game is paused, won or game over
    void hideMessage();
    void loadLevel();
    
    friend class Brick;
    friend class Gift;
};

#endif // GAMEENGINE_H
