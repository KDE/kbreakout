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
    gift = 0;
    width = BRICK_WIDTH;
    height = BRICK_HEIGHT;
    
    ++m_game->remainingBricks;
    moveTo(x*BRICK_WIDTH, (y-1)*BRICK_HEIGHT);
    setTypeFromChar(typeChar);
}

void Brick::setDeleted() 
{
    if (m_deleted) return;
    
    handleDeletion();
    hide();
}

void Brick::hit()
{
        if (type() == "HiddenBrick" && !isVisible()) {
            show();
            ++m_game->remainingBricks;
        } else if (type() == "MultipleBrick3") {
            setType("MultipleBrick2");
            // TODO: make a convenience function out of the following two
            m_game->addScore(qRound(m_game->dScore));
            m_game->dScore = BRICK_SCORE;
        } else if (type() == "MultipleBrick2") {
            setType("MultipleBrick1");
            m_game->addScore(qRound(m_game->dScore));
            m_game->dScore = BRICK_SCORE;
        } else if (type() != "UnbreakableBrick") {
            setDeleted();
        }
}

void Brick::explode()
{
    burn();
    foreach (Brick *b, nearbyBricks()) {
        b->burn();
    }
}

void Brick::burn()
{
    if (m_deleted) return;
    
    handleDeletion();
    setType("BurningBrick");
    show();
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
    case 'u': 
        setType("UnbreakableBrick");
        --m_game->remainingBricks;
        break;
    case 'h': 
        setType("HiddenBrick");
        hide();
        --m_game->remainingBricks;
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
    
    foreach(Brick *b, m_game->m_bricks) {
        foreach (QPoint p, nearbyPoints) {
            if (b->getRect().contains(p)) {
                result.append(b);
            }
        }
    }
    
    return result;
}
