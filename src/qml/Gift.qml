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

import QtQuick 1.1
import SvgLibrary 1.0
import "globals.js" as Globals

CanvasItem {
    id: gift
    //property alias type: spriteKey
    property string type
    onTypeChanged: spriteKey = type;

    width: m_scale * Globals.GIFT_WIDTH
    height: m_scale * Globals.GIFT_HEIGHT

    property real posX
    property real posY

    x: m_scale * posX
    y: m_scale * posY

    property bool falling: false
    property real giftSpeed: 0
    property int speedFactor: random(Globals.MINIMUM_GIFT_SPEED, Globals.MAXIMUM_GIFT_SPEED)

    function random(min, max) {
        return min + Math.floor(Math.random()*(max-min+1));
    }

    function setPosition(x, y) {
        posX = x + (Globals.BRICK_WIDTH-Globals.GIFT_WIDTH)/2;
        posY = y;
    }

    Timer {
        interval: gameTimer.interval
        running: gameTimer.running && falling
        repeat: true
        onTriggered: {
            var simpleSpeed = Math.sqrt(speed / interval);
            move(simpleSpeed, interval);
        }
    }

    function move(speed, updateInterval) {
        for(var i=0; i<updateInterval; i++) {
            var linearIncreaseFactor = 0.00008 * speed;
            var exponentialIncreaseFactor = 0.002;
            giftSpeed += speedFactor * linearIncreaseFactor;
            giftSpeed += giftSpeed * exponentialIncreaseFactor;
            posY += giftSpeed;
        }
    }
}
