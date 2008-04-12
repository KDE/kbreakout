#ifndef CANVASITEMS_H
#define CANVASITEMS_H

#include <KDE/KGameCanvas>

#include "item.h"

class Bar : public Item
{
public:
    Bar();
    
    // resets to default values
    void reset();
    
    // used by GiftEnlargeBar and GiftShrinkBar
    void enlarge();
    void shrink();
};

class Life : public Item
{
public:
    Life();
    ~Life();
    
private:
    static int count;
};

class Background : public Item
{
public:
    Background();
};

class Score : public Item
{
public:
    Score();
    void setScore(int newScore);

private:
    QString scoreString;
    void loadSprite();
};

#endif // CANVASITEMS_H
