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

class LevelLoader;

class GameEngine : public QObject
{
    Q_OBJECT
public:
    explicit GameEngine(QObject *parent = nullptr);
    ~GameEngine();

public Q_SLOTS:
    void start(const QString &levelset);
    void loadNextLevel();

Q_SIGNALS:
    void loadingNewGame();
    void newLine(const QString &line, int lineNumber);
    void newGift(const QString &gift, int times, const QString &pos);
    void ready();

private:
    void deleteAllObjects();

    LevelLoader *m_levelLoader;

private Q_SLOTS:
    void loadLevel();
};

#endif // GAMEENGINE_H
