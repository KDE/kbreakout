/*
    SPDX-FileCopyrightText: 2012 Viranch Mehta <viranch.mehta@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

import QtQuick 2.3
import org.kde.games.core 0.1 as KgCore
import "globals.js" as Globals

CanvasItem {
    id: bar
    spriteKey: "PlainBar"

    property real barWidth: Globals.DEFAULT_BAR_WIDTH
    width: m_scale * barWidth
    height: m_scale * Globals.BRICK_HEIGHT

    Behavior on width { NumberAnimation { duration: 50 } }

    property int direction: 0

    property string type
    onTypeChanged: spriteKey = type;

    // for preserving position relative
    // to bgOverlay when canvas is resized
    property real posX
    x: m_scale * posX

    function stopMoving() {
        moveTimer.stop();
    }

    Timer {
        id: moveTimer
        interval: Globals.DEFAULT_UPDATE_INTERVAL
        repeat: true
        running: direction!=0 && !paused
        onTriggered: moveBy(bar.direction*Globals.BAR_MOVEMENT);
    }

    function moveTo(newPos) {
        if (newPos < 0) {
            newPos = 0;
        } else if (newPos*m_scale + width > bgOverlay.width) {
            newPos = (bgOverlay.width - bar.width)/m_scale;
        }

        posX = newPos;
    }

    function moveBy(dx) {
        moveTo(posX + dx);
    }

    function reset() {
        var newWidth = Globals.DEFAULT_BAR_WIDTH;
        moveBy( (barWidth-newWidth)/2 );
        barWidth = newWidth;
        type = "PlainBar";
    }

    function enlarge() {
        var newWidth = Math.round(barWidth * Globals.RESIZE_BAR_RATIO);
        if (newWidth > Globals.MAX_BAR_WIDTH) {
            newWidth = Globals.MAX_BAR_WIDTH;
        }
        moveBy( (barWidth-newWidth)/2 );
        barWidth = newWidth;
    }

    function shrink() {
        var newWidth = Math.round(barWidth / Globals.RESIZE_BAR_RATIO);
        if (newWidth < Globals.MIN_BAR_WIDTH) {
            newWidth = Globals.MIN_BAR_WIDTH;
        }
        moveBy( (barWidth-newWidth)/2 );
        barWidth = newWidth;
    }
}
