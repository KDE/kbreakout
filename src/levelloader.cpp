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

// These can be removed when KConfig style levelsets are no longer supported
#include <KConfig>
#include <KConfigGroup>

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
        // Testing whether levelset is of old KConfig style
        KConfig kconfigfile(path, KConfig::SimpleConfig);
        if( kconfigfile.hasGroup( "level1" ) ){
            // Levelset is in KConfig style
            m_oldstyle = true;
            kError() << "Warning: Using deprecated KConfig-levelset. Please change to XML-Style.\n";
        } else {
            kError() << "Can't read levelset from " << path << "\nError: " << errorString <<
                        " in Line " << errorLine << ", Column " << errorColumn << endl;
        }
    } else {
        // Successfully loaded QDom-style levelset
        m_oldstyle = false;
    }
    file.close();
    // --
}

void LevelLoader::loadLevel(QList< Brick* >& m_bricks)
{   
    // Check if levelset is of KConfig-type
    if( m_oldstyle ){
        loadOldStyleLevel( m_bricks );
        return;
    }
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
    m_lineNumber = 0;
    while( !node.isNull() ){
        QDomElement info = node.toElement();
        if( info.isNull() ){ kError() << "Invalid levelset " << m_levelname << ": Can't read level information."; }
            
            if( info.tagName() == "Line" ){
                // Load one line of bricks
                loadLine( info, m_bricks );
        } else if( info.tagName() == "Gift" ){
            // Load one gift type
            loadGift( info, m_bricks );
        } else {
            kError() << "Invalid tag name " << info.tagName() << " has occured in level "
                     << levelName << " in levelset " << m_levelname << endl;
        }
        
        node = node.nextSibling();
    }
}

void LevelLoader::loadLine(QDomElement lineNode, QList< Brick* >& bricks)
{
    QDomAttr attribute = lineNode.attributeNode("Number");
    if( !attribute.isNull() ){
        m_lineNumber = attribute.value().toInt();
    } else {
        // Standard line numbering: load next line
        m_lineNumber++;
    }
    QString line = lineNode.text();
    if( line.size() > WIDTH ){
        kError() << "Invalid levelset " << m_levelname << ": to many bricks in line "
                 << m_lineNumber << endl;
    }
    
    // Convert line information to bricks
    for( int x = 0; x < line.size(); x++ ){
        char charType = line[x].toAscii();
        if (charType != '-') {
            bricks.append( new Brick(m_game, getTypeFromChar(charType), x, m_lineNumber) );
        }
    }
}

void LevelLoader::loadGift(QDomElement giftNode, QList< Brick* >& bricks)
{
    // Build list of bricks without a gift
    QList<Brick *> bricksLeft = bricks;
    QMutableListIterator<Brick *> i(bricksLeft);
    while (i.hasNext()) {
        Brick *brick = i.next();
        if (brick->type() == "UnbreakableBrick" || brick->hasGift() ){
            i.remove();
        }
    }
    QDomAttr attribute = giftNode.attributeNode("Type");
    if( attribute.isNull() ){
        kError() << "Invalid levelset " << m_levelname
                 << ": Gift Type in Level " << m_level << " not specified" << endl;
    }
    QString giftType = attribute.value();
    bool ok;
    // Nunber of gifts of this type to be randomly distributed
    int times = giftNode.text().toInt( &ok );
    if( ok && bricksLeft.count() < times ){
        kError() << "Invalid levelset " << m_levelname << ": In Level " << m_level
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
}

void LevelLoader::loadOldStyleLevel ( QList< Brick* >& m_bricks )
{
    // Selecting the correct level
    m_level++;
    
    // Loading the levelset
    QString path = "levelsets/" + m_levelname + ".levelset";
    path = KStandardDirs::locate("appdata", path);
    KConfig file(path, KConfig::SimpleConfig);
    
    QString levelName("level" + QString::number(m_level));
    
    if (!file.hasGroup(levelName)) {
        // No more levels or no levels found
        return;
    }
    
    // Loading level information
    KConfigGroup lvl = file.group(levelName);
    
    // add bricks
    
    int y = 1;
    QString key("line" + QString::number(y));
    
    while(lvl.hasKey(key)) {
        // one line of bricks to be converted
        QString line = lvl.readEntry(key, "error");
        if (line == "error") {
            kError() << "Something strange happened!!\n";
            return;
        }
        
        kDebug() << line << endl;
        
        if (line.size() > WIDTH) {
            kError() << "Invalid file: too many bricks\n";
        }
        
        // Convert the string, each char represents a brick
        for (int x = 0; x < line.size(); ++x ) {
            char charType = line[x].toAscii();
            if (charType != '-') {
                m_bricks.append(new Brick(m_game, getTypeFromChar(charType), x, y));
            }
        }
        
        ++y;
        key = "line" + QString::number(y);
    }
    
    // add gifts
    
    //bricks without gifts
    QList<Brick *> bricksLeft = m_bricks;
    QMutableListIterator<Brick *> i(bricksLeft);
    while (i.hasNext()) {
        Brick *brick = i.next();
        if (brick->type() == "UnbreakableBrick")
            i.remove();
    }
    
    for (int i = 0; i < GIFT_TYPES_COUNT; ++i) {
        key = GIFT_TYPES[i];
        if (!lvl.hasKey(key)) continue;
        
        QString line = lvl.readEntry(key, "error");
        if (line == "error") {
            kError() << "Impossible reading " << m_level << ":" << key << endl;
            return;
        }
        bool ok;
        int n = line.toInt(&ok);
        if (!ok) {
            kError() << m_levelname << ":" << key << " invalid number!!" << endl;
            continue;
        }
        if (bricksLeft.count() < n) {
            kError() << m_levelname << ": too many gifts!! " << endl;
            break;
        }
        for (int i = 0; i < n; ++i) {
            Gift *gift = new Gift(key); // key is the gift type
            gift->hide();
            
            int index = qrand() % bricksLeft.count();
            bricksLeft.at(index)->setGift(gift);
            bricksLeft.removeAt(index);
        }
    }
}

QString LevelLoader::getTypeFromChar(char type) 
{
    switch (type) {
    case '1': return "PlainBrick1";
    case '2': return "PlainBrick2";
    case '3': return "PlainBrick3";
    case '4': return "PlainBrick4";
    case '5': return "PlainBrick5";
    case '6': return "PlainBrick6";
    case 'm': return "MultipleBrick3";
    case 'x': return "ExplodingBrick";
    case 'u': return "UnbreakableBrick";
    case 'h': return "HiddenBrick";
    default:
        kError() << "Invalid File: unknown character '" 
                    << type << "'\n";
        return "PlainBrick1";
    }
}