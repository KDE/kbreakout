/*
    Copyright 20011 Julian Helfferich <julian.helfferich@gmail.com> 
  
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

#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <QList>
#include <QString>

class QDomDocument;
class Brick;
class GameEngine;

class LevelLoader
{
public:
    LevelLoader( GameEngine *gameEngine );
    ~LevelLoader();
    
    int level() const;
    void setLevel( int level );
    QString levelset() const;
    void setLevelset( const QString& levelname );
  
    void loadLevel( QList<Brick *> &m_bricks );
private:
    QDomDocument *m_levelset;
    GameEngine *m_game;
    
    QString m_levelname;
    int m_level;
};

#endif // LEVELLOADER_H