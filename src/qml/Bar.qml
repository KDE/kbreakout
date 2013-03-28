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
