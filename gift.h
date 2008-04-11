#ifndef GIFT_H
#define GIFT_H

#include "item.h"

class StringList;
class GameEngine;

class Gift : public Item
{
public:
    /**
     * This is the function that should be used to create new gifts
     */
    // TODO: see if I should overload new instead
    static Gift *newGift(const QString &type);
    
    void move();
    void execute(GameEngine *engine);
    
private:
    Gift(const QString &type);
    
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
