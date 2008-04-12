#include "brick.h"

#include "globals.h"
#include "gameengine.h"
#include "gift.h"

#include <QTimer>

Brick::Brick(GameEngine *gameEngine)
    : m_game(gameEngine),
      m_deleted(false)
{
    gift = 0;
    width = BRICK_WIDTH;
    height = BRICK_HEIGHT;
    elementId = "Unset";
}


void Brick::setDeleted() 
{
    if (m_deleted) return;
    
    handleDeletion();
    hide();
}

void Brick::burn()
{
    if (m_deleted) return;
    
    handleDeletion();
    setType("BurningBrick");
    QTimer::singleShot(200, this, SLOT(hide()));
}


void Brick::handleDeletion()
{
    m_deleted = true; 
    
    // these two kind of bricks aren't counted in the remainingBricks
    if (type() == "HiddenBrick" && !isVisible()) {
        ++m_game->remainingBricks;
    }
    if (type() == "UnbreakableBrick") {
        ++m_game->remainingBricks;
    }

    if (gift != 0) {
        gift->moveTo(getRect().left(), getRect().top());
        gift->show();
        m_game->m_gifts.append(gift);
    }
    
    --m_game->remainingBricks;
    
    m_game->addScore(qRound(m_game->dScore));
    m_game->dScore = BRICK_SCORE;
    
    if (m_game->remainingBricks == 0) {
        m_game->loadNextLevel();
        return;
    }
}

void Brick::hide()
{
    Item::hide();
}
