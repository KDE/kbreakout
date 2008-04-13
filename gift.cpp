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
    speed = MINIMUM_GIFT_SPEED + sp;
}

void Gift::move()
{
    moveBy(0, speed);
}

void Gift::execute(GameEngine *e)
{
    engine = e;
    engine->addScore(GIFT_SCORE);
    
    if (type() == "Gift100Points") {
        engine->addScore(100 - GIFT_SCORE);
    } 
    else if (type() == "Gift200Points") {
        engine->addScore(200 - GIFT_SCORE);
    } 
    else if (type() == "Gift500Points") {
        engine->addScore(500 - GIFT_SCORE);
    } 
    else if (type() == "Gift1000Points") {
        engine->addScore(1000 - GIFT_SCORE);
    } 
    else if (type() == "GiftAddLife") {
        if (engine->m_lives.count() < MAXIMUM_LIVES) {
            engine->m_lives.append(new Life);
        }
    } 
    else if (type() == "GiftLoseLife") {
        engine->handleDeath();
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
        engine->m_balls.append(new Ball);
    } 
    else if (type() == "GiftUnstoppableBall") {
        giftUnstoppableBall();
    }
    else if (type() == "GiftBurningBall") {
        giftBurningBall();
    }
    else if (type() == "GiftDecreaseSpeed") {
        qreal ui = engine->updateInterval * CHANGE_SPEED_RATIO;
        engine->setUpdateInterval(ui);
    }
    else if (type() == "GiftIncreaseSpeed") {
        qreal ui = engine->updateInterval / CHANGE_SPEED_RATIO;
        engine->setUpdateInterval(ui);
    }
    else if (type() == "GiftEnlargeBar") {
        Bar *bar = &engine->m_bar;
        bar->enlarge();
        engine->moveBar(bar->position().x() + bar->getRect().width()/2);
    }
    else if (type() == "GiftShrinkBar") {
        engine->m_bar.shrink();
    }
    else if (type() == "GiftStickyBar") {
        engine->m_bar.setType("StickyBar");
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
    foreach (Brick *brick, engine->m_bricks) {
        // don't assign points for Unbreakable Bricks
        if (brick->type() == "UnbreakableBrick") continue;
        
        engine->addScore(AUTOBRICK_SCORE);
        
        // add extra points for Multiple Bricks
        if (brick->type() == "MultipleBrick3")
            engine->addScore(AUTOBRICK_SCORE*2);
        if (brick->type() == "MultipleBrick2")
            engine->addScore(AUTOBRICK_SCORE);
    }
    engine->loadNextLevel();
}

void Gift::giftMagicEye()
{
    // make all hidden bricks visible
    foreach (Brick *brick, engine->m_bricks) {
        if (!brick->isDeleted() && !brick->isVisible()) {
            brick->show();
            ++engine->remainingBricks;
        }
    }
}

void Gift::giftMagicWand()
{
    foreach (Brick *brick, engine->m_bricks) {
        // make Unbreakbable Bricks Breakable
        if (brick->type() == "UnbreakableBrick") {
            brick->setType("BreakableBrick");
            ++engine->remainingBricks;
        }
        
        // Make Multiple Bricks single
        if (brick->type() == "MultipleBrick3") {
            brick->setType("MultipleBrick1");
            engine->addScore(AUTOBRICK_SCORE * 2);
        }
        if (brick->type() == "MultipleBrick2") {
            brick->setType("MultipleBrick1");
            engine->addScore(AUTOBRICK_SCORE);
        }
    }
}

void Gift::giftSplitBall()
{
    // TODO: better copy (type, speed, etc...)
    QList<Ball *> newBalls;
    foreach (Ball *ball, engine->m_balls) {
        Ball *newBall = new Ball;
        // give it a nice direction...
        newBall->directionX = ball->directionX;
        newBall->directionY = ball->directionY;
        if (ball->directionY > 0)
            newBall->directionY *= -1;
        else
            newBall->directionX *= -1;
        
        newBall->toBeFired = false;
        newBall->moveTo(ball->position());
        newBalls.append(newBall);
    }
    engine->m_balls += newBalls;
}

void Gift::giftUnstoppableBall()
{
    foreach (Ball *ball, engine->m_balls) {
        if (ball->type() == "BurningBall") {
            ball->setType("UnstoppableBurningBall");
        } else if (ball->type() != "UnstoppableBurningBall") {
            ball->setType("UnstoppableBall");
        }
    }
}

void Gift::giftBurningBall()
{
    foreach (Ball *ball, engine->m_balls) {
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
    foreach (Brick *brick, engine->m_bricks) {
        if (brick->type() == "ExplodingBrick") {
            explodingBricks.append(brick);
        }
    }
    
    foreach (Brick *brick, explodingBricks) {
        foreach (Brick *nearbyBrick, brick->nearbyBricks()) {
            nearbyBrick->setType("ExplodingBrick");
        }
    }
}
