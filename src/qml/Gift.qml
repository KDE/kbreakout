/*
    SPDX-FileCopyrightText: 2012 Viranch Mehta <viranch.mehta@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

import QtQuick 2.3
import org.kde.games.core 0.1 as KgCore
import "globals.js" as Globals

CanvasItem {
    id: gift

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
