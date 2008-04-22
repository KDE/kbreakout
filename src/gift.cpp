/*
    Copyright 2008 Fela Winkelmolen <fela.kde@gmail.com> 
  
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

#include "gift.h"

#include "gameengine.h"
#include "brick.h"
#include "ball.h"

#include <KDebug>

Gift::Gift(const QString &type)
{
    width = GIFT_WIDTH;
    height = GIFT_HEIGHT;
    setType(type);
    
    int sp = qrand() % (MAXIMUM_GIFT_SPEED - MINIMUM_GIFT_SPEED + 1);
    m_speed = MINIMUM_GIFT_SPEED + sp;
}

void Gift::move(qreal gameSpeed)
{
    moveBy(0, m_speed * gameSpeed);
}

void Gift::execute(GameEngine *gameEngine)
{
    m_game = gameEngine;
    m_game->addScore(GIFT_SCORE);
    
    if (type() == "Gift100Points") {
        m_game->addScore(100 - GIFT_SCORE);
    } 
    else if (type() == "Gift200Points") {
        m_game->addScore(200 - GIFT_SCORE);
    } 
    else if (type() == "Gift500Points") {
        m_game->addScore(500 - GIFT_SCORE);
    } 
    else if (type() == "Gift1000Points") {
        m_game->addScore(1000 - GIFT_SCORE);
    } 
    else if (type() == "GiftAddLife") {
        if (m_game->m_lives.count() < MAXIMUM_LIVES) {
            m_game->m_lives.append(new Life);
        }
    } 
    else if (type() == "GiftLoseLife") {
        m_game->handleDeath();
    } 
    else if (type() == "GiftNextLevel") {
        giftNextLevel();
    }
    else if (type() == "GiftMagicEye") {
        giftMagicEye();
    }
    else if (type() == "GiftMagicWand") {
        giftMagicWand();
    }
    else if (type() == "GiftSplitBall") {
        giftSplitBall();
    }
    else if (type() == "GiftAddBall") {
        m_game->m_balls.append(new Ball);
    } 
    else if (type() == "GiftUnstoppableBall") {
        giftUnstoppableBall();
    }
    else if (type() == "GiftBurningBall") {
        giftBurningBall();
    }
    else if (type() == "GiftDecreaseSpeed") {
        m_game->changeSpeed(1.0/CHANGE_SPEED_RATIO);
    }
    else if (type() == "GiftIncreaseSpeed") {
        m_game->changeSpeed(CHANGE_SPEED_RATIO);
    }
    else if (type() == "GiftEnlargeBar") {
        Bar *bar = &m_game->m_bar;
        bar->enlarge();
        m_game->moveBar(bar->position().x() + bar->getRect().width()/2);
    }
    else if (type() == "GiftShrinkBar") {
        m_game->m_bar.shrink();
    }
    else if (type() == "GiftStickyBar") {
        m_game->m_bar.setType("StickyBar");
    }
    else if (type() == "GiftMoreExplosion") {
        giftMoreExplosion();
    }
    else {
        kError() << "Unrecognized gift type!!!";
    }
}

void Gift::giftNextLevel()
{
    // assign points for each remaining brick
    foreach (Brick *brick, m_game->m_bricks) {
        // don't assign points for Unbreakable Bricks
        if (brick->type() == "UnbreakableBrick") continue;
        
        m_game->addScore(AUTOBRICK_SCORE);
        
        // add extra points for Multiple Bricks
        if (brick->type() == "MultipleBrick3")
            m_game->addScore(AUTOBRICK_SCORE*2);
        if (brick->type() == "MultipleBrick2")
            m_game->addScore(AUTOBRICK_SCORE);
    }
    m_game->loadNextLevel();
}

void Gift::giftMagicEye()
{
    // make all hidden bricks visible
    foreach (Brick *brick, m_game->m_bricks) {
        if (!brick->isDeleted() && !brick->isVisible()) {
            brick->show();
            ++m_game->remainingBricks;
        }
    }
}

void Gift::giftMagicWand()
{
    foreach (Brick *brick, m_game->m_bricks) {
        // make Unbreakbable Bricks Breakable
        if (brick->type() == "UnbreakableBrick") {
            brick->setType("BreakableBrick");
            ++m_game->remainingBricks;
        }
        
        // Make Multiple Bricks single
        if (brick->type() == "MultipleBrick3") {
            brick->setType("MultipleBrick1");
            m_game->addScore(AUTOBRICK_SCORE * 2);
        }
        if (brick->type() == "MultipleBrick2") {
            brick->setType("MultipleBrick1");
            m_game->addScore(AUTOBRICK_SCORE);
        }
    }
}

void Gift::giftSplitBall()
{
    // TODO: better copy (type, speed, etc...)
    QList<Ball *> newBalls;
    foreach (Ball *ball, m_game->m_balls) {
        Ball *newBall = new Ball;
        // give it a nice direction...
        newBall->directionX = ball->directionX;
        newBall->directionY = ball->directionY;
        if (ball->directionY > 0)
            newBall->directionY *= -1;
        else
            newBall->directionX *= -1;
        
        newBall->toBeFired = ball->toBeFired;
        newBall->setType(ball->type());
        newBall->moveTo(ball->position());
        newBalls.append(newBall);
    }
    m_game->m_balls += newBalls;
}

void Gift::giftUnstoppableBall()
{
    foreach (Ball *ball, m_game->m_balls) {
        if (ball->type() == "BurningBall") {
            ball->setType("UnstoppableBurningBall");
        } else if (ball->type() != "UnstoppableBurningBall") {
            ball->setType("UnstoppableBall");
        }
    }
}

void Gift::giftBurningBall()
{
    foreach (Ball *ball, m_game->m_balls) {
        if (ball->type() == "UnstoppableBall") {
            ball->setType("UnstoppableBurningBall");
        } else if (ball->type() != "UnstoppableBurningBall") {
            ball->setType("BurningBall");
        }
    }
}

void Gift::giftMoreExplosion()
{
    QList<Brick *> explodingBricks;
    foreach (Brick *brick, m_game->m_bricks) {
        if (brick->type() == "ExplodingBrick") {
            explodingBricks.append(brick);
        }
    }
    
    foreach (Brick *brick, explodingBricks) {
        foreach (Brick *nearbyBrick, brick->nearbyBricks()) {
            if (nearbyBrick->type() == "UnbreakableBrick") {
                ++m_game->remainingBricks;
            }
            if (nearbyBrick->type() == "HiddenBrick" && 
                    !nearbyBrick->isVisible()) {
                nearbyBrick->show();
                ++m_game->remainingBricks;
            }
            
            nearbyBrick->setType("ExplodingBrick");
        }
    }
}
