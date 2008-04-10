#ifndef CONSTS_H
#define CONSTS_H

#include <QString>

const double SPEED_FACTOR = 1.0;
const double DEFAULT_SPEED = 2;
const int GIFT_SPEED = 1;
const int UPDATE_INTERVAL = 10;
const int USE_ANTIALIASING = true;

const int WIDTH = 20; // how many bricks the game is wide
const int HEIGHT = 24; // how many bricks the game is high
const int BRICK_WIDTH = 30;
const int BRICK_HEIGHT = 15;
const int BALL_SIZE = 10;
const int DEFAULT_BAR_WIDTH = 60;
const int MIN_BAR_WIDTH = 30;
const int MAX_BAR_WIDTH = 250;
const int GIFT_WIDTH = 25;
const int GIFT_HEIGHT = 18;
const qreal BACKGROUND_BORDER = 0.01;

// used to enlarge and shrink the bar
const qreal RESIZE_BAR_RATIO = 1.2;
const qreal CHANGE_BALL_SPEED_RATIO = 1.2;

// pixels to move the bar each "tick", when using the keyboard
const int BAR_MOVEMENT = 5;

// points when breaking a brick
// decreases over time since last brick was hit
const int BRICK_SCORE = 20;
// score when i brick gets "autoremoved" 
// (for example becouse of a gift, of fire)
const int AUTOBRICK_SCORE = BRICK_SCORE / 2;
// points avarded when passing a level
const int LEVEL_SCORE = 2000;
const int GIFT_SCORE = 50;
const int LOSE_LIFE_SCORE = -500;
const int INITIAL_LIVES = 2;
const int MAX_LIVES = 8;

// minimum with and height of the game widget (CanvasWidget)
const int MINIMUM_WIDTH = 700;
const int MINIMUM_HEIGHT = 400;

// IMPORTANT: keep giftTypesCount in sync!!!!!!!
// TODO: see if I can use somthing different
const QString giftTypes[] = {
    "Gift100Points",
    "Gift200Points",
    "Gift500Points",
    "Gift1000Points",
    "GiftAddLife",
    
    "GiftLoseLife", 
    "GiftNextLevel",
    "GiftSplitBall",
    "GiftAddBall",
    "GiftDecreaseBallSpeed",
    
    "GiftIncreaseBallSpeed",
    "GiftUnstoppableBall",
    "GiftBurningBall",
    "GiftMagicWand",
    "GiftMagicEye",
    
    "GiftEnlargeBar",
    "GiftShrinkBar"
};
    
const int giftTypesCount = 17;

#endif // CONSTS_H
