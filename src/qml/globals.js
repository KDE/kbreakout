/*
    Copyright 2012 Viranch Mehta <viranch.mehta@gmail.com>
  
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

// how many pixels it moves each step
// should be less than the half of BALL_SIZE
var BALL_SPEED = 2;
var MINIMUM_SPEED = 1.2;
var MINIMUM_GIFT_SPEED = 1;
var MAXIMUM_GIFT_SPEED = 3;
var REPAINT_INTERVAL = 16; // should be a power of two
// how ofter the position of the item is updated (but not repainted)
var DEFAULT_UPDATE_INTERVAL = 13;
//var MAXIMUM_UPDATE_INTERVAL = REPAINT_INTERVAL;
var MINIMUM_UPDATE_INTERVAL = 8;
// the higher this number the more the game becomes faster over time
var AUTO_SPEED_INCREASE = 1.05;

var BURNING_INTERVAL = 200; // how long it burns
var BURNING_SPEED = Math.floor(BURNING_INTERVAL/2); // lower is faster

var WIDTH = 20; // how many bricks the game is wide
var HEIGHT = 24; // how many bricks the game is high
var BRICK_WIDTH = 30;
var BRICK_HEIGHT = 15;
var BALL_SIZE = 10;
var DEFAULT_BAR_WIDTH = 60;
var MIN_BAR_WIDTH = 30;
var MAX_BAR_WIDTH = 250;
var GIFT_WIDTH = 25;
var GIFT_HEIGHT = 18;

// used to enlarge and shrink the bar
var RESIZE_BAR_RATIO = 1.4;
var CHANGE_SPEED_RATIO = 1.3;

// pixels to move the bar each "tick", when using the keyboard
var BAR_MOVEMENT = 5;

// points when breaking a brick
// decreases over time since last brick was hit
var BRICK_SCORE = 15;
// relative score w.r.t the old score
var SCORE_AUTO_DECREASE = 0.998;
// score when i brick gets "autoremoved" 
// (for example becouse of a gift, of fire)
var AUTOBRICK_SCORE = Math.floor(BRICK_SCORE / 2);
// points avarded when passing a level
var LEVEL_SCORE = 300;
var GIFT_SCORE = 30;
var LOSE_LIFE_SCORE = 0; // disabled
// score added for each life when game is won
var LIFE_SCORE = 2000;
var GAME_WON_SCORE = 10000;
var INITIAL_LIVES = 2;
var MAXIMUM_LIVES = 10;

// minimum with and height of the game widget (CanvasWidget)
var DEFAULT_WIDTH = 750;
var DEFAULT_HEIGHT = 500;
