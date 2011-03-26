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
    void loadOldStyleLevel( QList<Brick *> &m_bricks );
    static QString getTypeFromChar(char type);
    
    QDomDocument *m_levelset;
    bool m_oldstyle;
    GameEngine *m_game;
    
    QString m_levelname;
    int m_level;
};

// Global list of gift types
// Can be removed, when KConfig levelsets are no longer supported

const QString GIFT_TYPES[] = {
    "Gift100Points",
    "Gift200Points",
    "Gift500Points",
    "Gift1000Points",
    "GiftAddLife",
    
    "GiftLoseLife", 
    "GiftNextLevel",
    "GiftSplitBall",
    "GiftAddBall",
    "GiftDecreaseSpeed",
    
    "GiftIncreaseSpeed",
    "GiftUnstoppableBall",
    "GiftBurningBall",
    "GiftMagicWand",
    "GiftMagicEye",
    
    "GiftEnlargeBar",
    "GiftShrinkBar",
    "GiftStickyBar",
    "GiftMoreExplosion"
};

// IMPORTANT: keep in sync with the above
const int GIFT_TYPES_COUNT = 19;

#endif // LEVELLOADER_H