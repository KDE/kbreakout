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
    id: bar
    spriteKey: "PlainBar"
    width: m_scale * Globals.DEFAULT_BAR_WIDTH
    height: m_scale * Globals.BRICK_HEIGHT
    property int direction: 0

    function type() { return spriteKey; }

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
        onTriggered: {
            var xPos = bar.posX + (bar.direction*Globals.BAR_MOVEMENT);
            if (xPos < 0) {
                xPos = 0;
            } else if (xPos*m_scale+bar.width > bgOverlay.width) {
                xPos = (bgOverlay.width - bar.width)/m_scale;
            }

            bar.posX = xPos;
        }
    }
}
