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
    id: ball
    spriteKey: "PlainBall"

    property string type
    onTypeChanged: spriteKey = type;

    property real directionX
    property real directionY

    property bool toBeFired: true
    property real barPosition: 0.6

    width: m_scale * Globals.BALL_SIZE
    height: m_scale * Globals.BALL_SIZE

    // for preserving position relative
    // to bgOverlay when canvas is resized
    property real posX
    property real posY
    x: m_scale * posX
    y: m_scale * posY

    Timer {
        interval: gameTimer.interval
        running: gameTimer.running
        repeat: true
        onTriggered: {
            if (toBeFired) {
                posX = (bar.x + barPosition*bar.width)/m_scale;
            } else {
                posX += directionX * speed;
                posY += directionY * speed;
            }
        }
    }
}
