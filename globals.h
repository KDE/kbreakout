/*
    Copyright 2007-2008 Fela Winkelmolen <fela.kde@gmail.com> 
  
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

#ifndef CONSTS_H
#define CONSTS_H

#include <QString>

// how many pixels it moves each step
// should be less than the half of BALL_SIZE
const int BALL_SPEED = 3;
const int MINIMUM_GIFT_SPEED = 1;
const int MAXIMUM_GIFT_SPEED = 3;
const int REPAINT_INTERVAL = 18;
// how ofter the position of the item is updated (but not repainted)
const int DEFAULT_UPDATE_INTERVAL = 13;
const int MAXIMUM_UPDATE_INTERVAL = REPAINT_INTERVAL;
const int MINIMUM_UPDATE_INTERVAL = 5;
// the lower this number the more the game becomes faster over time
const qreal UPDATE_INTERVAL_DECREASE = 0.99;

const int BURNING_INTERVAL = 200; // how long it burns
const int BURNING_SPEED = BURNING_INTERVAL/2; // lower is faster

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

// used to enlarge and shrink the bar
const qreal RESIZE_BAR_RATIO = 1.3;
const qreal CHANGE_SPEED_RATIO = 1.3;

// pixels to move the bar each "tick", when using the keyboard
const int BAR_MOVEMENT = 5;

// points when breaking a brick
// decreases over time since last brick was hit
const int BRICK_SCORE = 15;
// relative score w.r.t the old score each step
const qreal SCORE_AUTO_DECREASE = 0.998;
// score when i brick gets "autoremoved" 
// (for example becouse of a gift, of fire)
const int AUTOBRICK_SCORE = BRICK_SCORE / 2;
// points avarded when passing a level
const int LEVEL_SCORE = 300;
const int GIFT_SCORE = 30;
const int LOSE_LIFE_SCORE = -500;
const int INITIAL_LIVES = 2;
const int MAXIMUM_LIVES = 8;

// minimum with and height of the game widget (CanvasWidget)
const int DEFAULT_WIDTH = 700;
const int DEFAULT_HEIGHT = 400;

// IMPORTANT: keep GIFT_TYPES_COUNT in sync!!!!!!!
// TODO: see if I can use somthing different
const QString GIFT_TYPES[] = {
    "Gift100Points",
    "Gift200Points",
    "Gift500Points",
    "Gift1000Points",
    "GiftAddLife",
    
    "GiftLoseLife", 
    "GiftNextLevel",
    "GiftSplitBall",
    "GiftAddBall",
    "GiftDecreaseSpeed",
    
    "GiftIncreaseSpeed",
    "GiftUnstoppableBall",
    "GiftBurningBall",
    "GiftMagicWand",
    "GiftMagicEye",
    
    "GiftEnlargeBar",
    "GiftShrinkBar",
    "GiftStickyBar",
    "GiftMoreExplosion"
};

// IMPORTANT: keep in sync with the above
const int GIFT_TYPES_COUNT = 19;

#endif // CONSTS_H
