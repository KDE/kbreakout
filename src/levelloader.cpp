/*
    Copyright 2011 Julian Helfferich <julian.helfferich@gmail.com> 
  
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

#include "levelloader.h"
#include "brick.h"
#include "gift.h"

#include <QDomDocument>
#include <QFile>

#include <KDebug>
#include <KStandardDirs>

LevelLoader::LevelLoader(GameEngine *gameEngine)
  : m_game( gameEngine )
{
    m_levelname = QString();
    m_level = 0;
    m_levelset = 0;
}

LevelLoader::~LevelLoader()
{
    delete m_levelset;
}

int LevelLoader::level() const
{
    return m_level;
}

void LevelLoader::setLevel(int level)
{
    m_level = level;
}

QString LevelLoader::levelset() const
{
    return m_levelname;
}

void LevelLoader::setLevelset(const QString& levelname)
{
    if( levelname == m_levelname ){
        return;
    }
    m_levelname = levelname;

    // Loading document model
    // Locating the path in the filesystem
    QString path = "levelsets/" + m_levelname + ".levelset";
    path =  KStandardDirs::locate("appdata", path);
    // --
    
    delete m_levelset;
    
    m_levelset = new QDomDocument( m_levelname );
    QFile file( path );
    if( !file.open( QIODevice::ReadOnly ) ){
        kError() << "Can't open file " << path << endl;
    }
  
    QString errorString;
    int errorLine;
    int errorColumn;
    if( !m_levelset->setContent( &file, false, &errorString, &errorLine, &errorColumn ) ){
        file.close();
        kError() << "Can't read levelset from " << path << "\nError: " << errorString <<
                    " in Line " << errorLine << ", Column " << errorColumn << endl;
    }
    file.close();
    // --
}

void LevelLoader::loadLevel(QList< Brick* >& m_bricks)
{   
    // Selecting the correct level
    m_level++;
    
    if( m_levelset == 0 ){
        kError() << "Error: No levelset specified" << endl;
        return;
    }
    
    QDomElement levels = m_levelset->documentElement();
    QDomNode node = levels.firstChild();
    for( int i = 1; i < m_level; i++ ){
        node = node.nextSibling();
    }
    // --
    
    // Load level information
    if( node.isNull() || node.toElement().tagName() != "Level" ){
        // Level not found or no more levels
        return;
    }
    
    QDomAttr attribute;
    QDomElement level = node.toElement();
    if( level.isNull() ){
        kError() << "Invalid Levelset " << m_levelname << ": Can't read level information";
    }
    attribute = level.attributeNode("Name");
    QString levelName;
    if( !attribute.isNull() ){
        levelName = level.attributeNode("Name").value();
    }
    node = node.firstChild();
    // --
    
    // Load bricks and gifts
    int lineNumber = 0;
    while( !node.isNull() ){
        QDomElement info = node.toElement();
        if( info.isNull() ){ kError() << "Invalid levelset " << m_levelname << ": Can't read level information."; }
            
            if( info.tagName() == "Line" ){
                // Load one line of bricks
                attribute = info.attributeNode("Number");
                if( !attribute.isNull() ){
                    lineNumber = attribute.value().toInt();
                } else {
                    // Standard line numbering: load next line
                    lineNumber++;
                }
                QString line = info.text();
                if( line.size() > WIDTH ){
                    kError() << "Invalid levelset " << m_levelname << ": to many bricks in line "
                             << lineNumber << endl;
                }
                
                // Convert line information to bricks
                for( int x = 0; x < line.size(); x++ ){
                    char type = line[x].toAscii();
                    if (type != '-') {
                        m_bricks.append( new Brick(m_game, type, x, lineNumber) );
                    }
                }
        } else if( info.tagName() == "Gift" ){
            // Load one gift type
            // Build list of bricks without a gift
            QList<Brick *> bricksLeft = m_bricks;
            QMutableListIterator<Brick *> i(bricksLeft);
            while (i.hasNext()) {
                Brick *brick = i.next();
                if (brick->type() == "UnbreakableBrick" || brick->hasGift() ){
                    i.remove();
                }
            }
            attribute = info.attributeNode("Type");
            if( attribute.isNull() ){
                kError() << "Invalid levelset " << m_levelname
                         << ": Gift Type in Level " << levelName << " not specified" << endl;
            }
            QString giftType = attribute.value();
            bool ok;
            // Nunber of gifts of this type to be randomly distributed
            int times = info.text().toInt( &ok );
            if( ok && bricksLeft.count() < times ){
                kError() << "Invalid levelset " << m_levelname << ": In Level " << levelName
                         << " are too many gifts of type " << giftType << endl;
            }
            // Distribute gifts
            for( int i = 0; i < times; i++ ){
                Gift *gift = new Gift( giftType );
                gift->hide();
                
                int index = qrand() % bricksLeft.count();
                bricksLeft.at(index)->setGift(gift);
                bricksLeft.removeAt(index);
            }
        } else {
            kError() << "Invalid tag name " << info.tagName() << " has occured in level "
                     << levelName << " in levelset " << m_levelname << endl;
        }
        
        node = node.nextSibling();
    }
}