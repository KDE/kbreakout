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

    function moveBy(dx) {
        var xPos = posX + dx;
        if (xPos < 0) {
            xPos = 0;
        } else if (xPos*m_scale + width > bgOverlay.width) {
            xPos = (bgOverlay.width - bar.width)/m_scale;
        }

        posX = xPos;
    }

    function reset() {
        var oldWidth = width;
        width = m_scale * Globals.DEFAULT_BAR_WIDTH;
        type = "PlainBar";
        moveBy( ((oldWidth-width)/2) / m_scale );
    }

    function enlarge() {
        var oldWidth = width;
        width = Math.round((width/m_scale) * Globals.RESIZE_BAR_RATIO) * m_scale;
        if (width > Globals.MAX_BAR_WIDTH*m_scale) {
            width = Globals.MAX_BAR_WIDTH*m_scale;
        }
        moveBy( ((oldWidth-width)/2) / m_scale );
    }

    function shrink() {
        var oldWidth = width;
        width = Math.round((width/m_scale) / Globals.RESIZE_BAR_RATIO) * m_scale;
        if (width < Globals.MIN_BAR_WIDTH*m_scale) {
            width = Globals.MIN_BAR_WIDTH*m_scale;
        }
        moveBy( ((oldWidth-width)/2) / m_scale );
    }
}
