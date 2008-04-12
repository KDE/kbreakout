#ifndef BRICK_H
#define BRICK_H

#include "item.h"

class Gift;
class GameEngine;

class Brick : public Item
{
    Q_OBJECT
public:
    Brick(GameEngine *game, char typeChar, int posX, int posY);
    
    Gift *gift; // TODO: make private
    
    void setDeleted();
    void hit();
    void explode();
    
    bool isDeleted() {return m_deleted;}
    
    // a ball collided against the brick
    // can acces the private GameEngine members so be carefull!!! (TODO: (??))
    //virtual void handleCollision();

public slots:
    void burn();
    void hide();

private:
    void handleDeletion();
    void setTypeFromChar(char type);
    QList<Brick *> nearbyBricks();

    GameEngine *m_game;
    bool m_deleted;
};

#endif //BRICK_H
