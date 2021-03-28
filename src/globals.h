/*
    SPDX-FileCopyrightText: 2007-2008 Fela Winkelmolen <fela.kde@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef GLOBALS_H
#define GLOBALS_H

// how many pixels it moves each step
// should be less than the half of BALL_SIZE
const int BALL_SPEED = 2;
const qreal MINIMUM_SPEED = 1.2;
const int MINIMUM_GIFT_SPEED = 1;
const int MAXIMUM_GIFT_SPEED = 3;
const int REPAINT_INTERVAL = 16; // should be a power of two
// how ofter the position of the item is updated (but not repainted)
const int DEFAULT_UPDATE_INTERVAL = 13;
//const int MAXIMUM_UPDATE_INTERVAL = REPAINT_INTERVAL;
// the higher this number the more the game becomes faster over time
const qreal AUTO_SPEED_INCREASE = 1.05;

const int BURNING_INTERVAL = 200; // how long it burns
const int BURNING_SPEED = BURNING_INTERVAL / 2; // lower is faster

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
const qreal RESIZE_BAR_RATIO = 1.4;
const qreal CHANGE_SPEED_RATIO = 1.3;

// pixels to move the bar each "tick", when using the keyboard
const int BAR_MOVEMENT = 5;

// points when breaking a brick
// decreases over time since last brick was hit
const int BRICK_SCORE = 15;
// relative score w.r.t the old score
const qreal SCORE_AUTO_DECREASE = 0.998;
// score when i brick gets "autoremoved"
// (for example becouse of a gift, of fire)
const int AUTOBRICK_SCORE = BRICK_SCORE / 2;
// points avarded when passing a level
const int LEVEL_SCORE = 300;
const int GIFT_SCORE = 30;
const int LOSE_LIFE_SCORE = 0; // disabled
// score added for each life when game is won
const int LIFE_SCORE = 2000;
const int GAME_WON_SCORE = 10000;
const int INITIAL_LIVES = 2;
const int MAXIMUM_LIVES = 10;

// minimum width and height of the game widget (CanvasWidget)
const int DEFAULT_WIDTH = 750;
const int DEFAULT_HEIGHT = 500;

#endif // GLOBALS_H
