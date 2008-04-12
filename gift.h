#ifndef GIFT_H
#define GIFT_H

#include "item.h"

class StringList;
class GameEngine;

class Gift : public Item
{
public:
    Gift(const QString &type);
    
    void move();
    void execute(GameEngine *engine);
    
private:
    // convenience functions
    void giftNextLevel();
    void giftMagicEye();
    void giftMagicWand();
    void giftSplitBall();
    void giftUnstoppableBall();
    void giftBurningBall();
    
    static QStringList giftTypes;
    
    GameEngine *engine;
    int speed;
};

#endif // GIFT_H
