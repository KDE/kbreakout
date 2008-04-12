#ifndef BRICK_H
#define BRICK_H

#include "item.h"

class Gift;
class GameEngine;

class Brick : public Item
{
    Q_OBJECT
public:
    Brick(GameEngine *game);
    
    Gift *gift; // TODO: make private
    
    void setDeleted();
    bool isDeleted() {return m_deleted;}
    
    void burn();
    
    // a ball collided against the brick
    // can acces the private GameEngine members so be carefull!!! (TODO: (??))
    //virtual void handleCollision();

public slots:
    void hide();

private:
    void handleDeletion();

    GameEngine *m_game;
    bool m_deleted;
};

#endif //BRICK_H
