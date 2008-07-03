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

#include "brick.h"

#include "globals.h"
#include "gameengine.h"
#include "gift.h"

#include <QTimer>
#include <KDebug>

Brick::Brick(GameEngine *gameEngine, char typeChar, int x, int y)
    : m_game(gameEngine),
      m_deleted(false)
{
    m_gift = 0;
    width = BRICK_WIDTH;
    height = BRICK_HEIGHT;
    
    ++m_game->m_remainingBricks;
    moveTo(x*BRICK_WIDTH, (y-1)*BRICK_HEIGHT);
    setTypeFromChar(typeChar);
}

Brick::~Brick()
{
}

void Brick::setGift(Gift *gift)
{
    m_gift = gift;
}

void Brick::forcedHit()
{
    if (m_deleted) return;
    
    if (type() == "ExplodingBrick") {
        explode();
    } else {
        handleDeletion();
    }
    hide();
}

void Brick::hit()
{
    if (type() == "HiddenBrick" && !isVisible()) {
        show();
        ++m_game->m_remainingBricks;
    } else if (type() == "MultipleBrick3") {
        setType("MultipleBrick2");
        // TODO: make a convenience function out of the following two
        m_game->addScore(qRound(m_game->m_dScore));
        m_game->m_dScore = BRICK_SCORE;
    } else if (type() == "MultipleBrick2") {
        setType("MultipleBrick1");
        m_game->addScore(qRound(m_game->m_dScore));
        m_game->m_dScore = BRICK_SCORE;
    } else if (type() == "ExplodingBrick") {
        explode();
    } else if (type() != "UnbreakableBrick") {
        forcedHit();
    }
}

void Brick::explode()
{
    if (m_deleted) return;
    
    burn();
    QTimer::singleShot(BURNING_SPEED, this, SLOT(burnNearbyBricks()));
    
}

void Brick::burnNearbyBricks()
{
    foreach (Brick *b, nearbyBricks()) {
        b->burn();
    }
}

void Brick::burn()
{
    if (m_deleted) return;
    
    if (type() == "ExplodingBrick") {
        // makes sure it doesn't explode twice
        setType("BurningBrick");
        explode();
    } else {
        handleDeletion();
    }
    
    setType("BurningBrick");
    show(); // make sure hidden bricks are shown
    QTimer::singleShot(BURNING_INTERVAL, this, SLOT(hide()));
}


void Brick::handleDeletion()
{
    if (m_deleted) {
        kError() << "Already deleted!!!!!!!!!!!!!!!!";
    }
    
    m_deleted = true;

    if (m_gift != 0) {
        m_gift->startFall(getRect().left(), getRect().top());
        m_game->m_gifts.append(m_gift);
    }
    
    --m_game->m_remainingBricks;
    
    m_game->addScore(qRound(m_game->m_dScore));
    m_game->m_dScore = BRICK_SCORE;

    // these two kind of bricks aren't counted in the remainingBricks
    if ( (type() == "HiddenBrick" && !isVisible())
            || type() == "UnbreakableBrick") {
        ++m_game->m_remainingBricks;
        return; // never need to load the next level
    }
    
    if (m_game->m_remainingBricks == 0) {
        m_game->loadNextLevel();
    }
}

void Brick::hide()
{
    Item::hide();
}

void Brick::setTypeFromChar(char type) 
{
    switch (type) {
    case '1': setType("PlainBrick1"); break;
    case '2': setType("PlainBrick2"); break;
    case '3': setType("PlainBrick3"); break;
    case '4': setType("PlainBrick4"); break;
    case '5': setType("PlainBrick5"); break;
    case '6': setType("PlainBrick6"); break;
    case 'm': setType("MultipleBrick3"); break;
    case 'x': setType("ExplodingBrick"); break;
    case 'u': 
        setType("UnbreakableBrick");
        --m_game->m_remainingBricks;
        break;
    case 'h': 
        setType("HiddenBrick");
        hide();
        --m_game->m_remainingBricks;
        break;
    default:
        kError() << "Invalid File: unknown character '" 
                    << type << "'\n";
        setType("PlainBrick1");
    }
}

QList<Brick *> Brick::nearbyBricks()
{
    QList<Brick *> result;
    QRect brickRect = getRect();
    // coordinates of the center of the brick
    int x = brickRect.x() + BRICK_WIDTH / 2;
    int y = brickRect.y() + BRICK_HEIGHT / 2; 
    
    // points to the left, right, top and bottom of the brick
    QList<QPoint> nearbyPoints;
    nearbyPoints.append(QPoint(x - BRICK_WIDTH, y));
    nearbyPoints.append(QPoint(x + BRICK_WIDTH, y));
    nearbyPoints.append(QPoint(x, y - BRICK_HEIGHT));
    nearbyPoints.append(QPoint(x, y + BRICK_HEIGHT));
    
    foreach (Brick *b, m_game->m_bricks) {
        if (b->isDeleted()) continue;
        foreach (const QPoint& p, nearbyPoints) {
            if (b->getRect().contains(p)) {
                result.append(b);
            }
        }
    }
    
    return result;
}
