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
import "logic.js" as Logic

Item {
    id: canvas

    signal levelComplete()
    signal gameEnded(int score, int level, int elapsedTime)
    
    onWidthChanged: updateGeometry();
    onHeightChanged: updateGeometry();

    function updateGeometry() {
        var bw = Globals.BRICK_WIDTH*Globals.WIDTH + 1;
        // bh = (overlayHeight = BRICK_HEIGHT*HEIGHT+1)
        //     +(headerItemsHeight = BRICK_HEIGHT*1.5)
        //     +(margin = headerItemHeight*0.2)
        var bh = Globals.BRICK_HEIGHT*Globals.HEIGHT + 1 + Globals.BRICK_HEIGHT*1.5*1.2;

        var w = canvas.height*bw/bh;
        var h = canvas.width*bh/bw;

        if (w > canvas.width) {
            container.width = canvas.width-20;
            container.height = container.width*bh/bw;
        } else if (h > canvas.height) {
            container.height = canvas.height-50;
            container.width = container.height*bw/bh;
        } else {
            // Never happens, don't know what to do in case it does!
        }
    }

    property real m_scale: container.width/(Globals.BRICK_WIDTH*Globals.WIDTH)

    CanvasItem {
        id: background
        spriteKey: "Background"
        anchors.fill: parent
    }

    Item {
        id: container
        anchors.centerIn: parent
    }

    CanvasItem {
        id: bgOverlay
        spriteKey: "BackgroundOverlay"
        anchors {
            left: container.left
            bottom: container.bottom
        }
        width: m_scale * (Globals.BRICK_WIDTH*Globals.WIDTH + 1)
        height: m_scale * (Globals.BRICK_HEIGHT*Globals.HEIGHT + 1)
    }

    property bool paused: false
    Rectangle {
        anchors.fill: parent
        color: "#646464"
        opacity: paused ? 0.6 : 0
        z: 1

        Behavior on opacity { NumberAnimation { duration: 100 } }
    }
    function setGamePaused(paused) {
        Logic.setGamePaused(paused);
    }

    TextItem {
        id: messageBox
        anchors.centerIn: bgOverlay
        z: 2
        width: m_scale * Globals.BRICK_WIDTH*9
        height: m_scale * Globals.BRICK_HEIGHT*5
        opacity: text!=""

        Behavior on opacity { NumberAnimation { duration: 100 } }
    }

    TextItem {
        id: infoMessage
        anchors {
            horizontalCenter: bgOverlay.horizontalCenter
            bottom: bgOverlay.bottom
            bottomMargin: (bgOverlay.height-height)/4
        }
        width: m_scale * Globals.BRICK_WIDTH*9
        height: m_scale * Globals.BRICK_HEIGHT*2
        opacity: text!=""

        Behavior on opacity { NumberAnimation { duration: 100 } }
    }

    property int score: 0
    TextItem {
        id: scoreDisplay
        width: m_scale * (Globals.BRICK_WIDTH*Globals.WIDTH)/6
        height: m_scale * Globals.BRICK_HEIGHT*1.5
        anchors {
            left: bgOverlay.left
            bottom: bgOverlay.top
            bottomMargin: height/5
        }
        text: parent.score
    }

    property int level: 0
    TextItem {
        id: levelDisplay
        width: m_scale * (Globals.BRICK_WIDTH*Globals.WIDTH)/5
        height: m_scale * Globals.BRICK_HEIGHT*1.5
        anchors {
            left: scoreDisplay.right
            leftMargin: width-scoreDisplay.width
            bottom: bgOverlay.top
            bottomMargin: height/5
        }
        text: "Level "+parent.level
    }

    property int lives: Globals.INITIAL_LIVES
    Row {
        id: lifeBars
        spacing: m_scale * Globals.BRICK_WIDTH*0.23
        anchors {
            right: bgOverlay.right
            rightMargin: 20
            bottom: bgOverlay.top
            bottomMargin: levelDisplay.height/5
        }

        Repeater {
            model: canvas.lives
            Bar {
                width: m_scale * Globals.BRICK_WIDTH/1.3
                height: m_scale * Globals.BRICK_HEIGHT/1.3
            }
        }
    }

    function loadLine(line, lineNumber) {
        var start = (lineNumber-1)*Globals.WIDTH;
        brickString = brickString.slice(0, start) + line + brickString.slice(start+line.length);
        for(var i=0; i<line.length; i++) {
            var c = line[i];
            if (c!='-' && c!='u' && c!='h') Logic.remainingBricks++;
        }
    }

    function loadGift(gift, times, pos) {
        Logic.putGift(gift, times, pos);
    }

    property string brickString
    
    Component.onCompleted: {
        Logic.resetBricks();
    }

    Grid {
        id: bricks
        columns: Globals.WIDTH
        rows: Globals.HEIGHT
        anchors.fill: bgOverlay
        property alias items: brickItems
        
        Repeater {
            id: brickItems
            model: brickString.split("")
            Brick {
                type: Logic.getTypeFromChar(modelData)
            }
        }
    }

    Bar {
        id: bar
        anchors {
            bottom: bgOverlay.bottom
        }
        x: m_scale * 300
        property int direction: 0
    }

    Timer {
        id: moveBarTimer
        interval: Globals.DEFAULT_UPDATE_INTERVAL
        repeat: true
        onTriggered: {
            Logic.moveBar(bar.x + (m_scale * bar.direction*Globals.BAR_MOVEMENT));
        }
    }

    function updateBarDirection(direction) {
        if (paused)
            return;
        if (direction==0) {
            moveBarTimer.stop();
        } else {
            bar.direction = direction;
            if (!moveBarTimer.running)
                moveBarTimer.start();
        }
    }

    function startGame() {
        Logic.startLevel();
    }

    Timer {
        id: gameTimer
        interval: Globals.REPAINT_INTERVAL
        repeat: true
        onTriggered: Logic.timerTimeout();
    }

    Timer {
        id: elapsedTimeTimer
        interval: 1000
        repeat: true
        property int elapsedTime: 0
        onTriggered: elapsedTime++;
    }

    Timer {
        id: handleDeathTimer
        interval: 1000
        onTriggered: Logic.handleDeath()
    }

    Timer {
        id: hideTimer
        property variant target
        onTriggered: target.text = "";
    }

    function fire() {
        if (paused) {
            Logic.setGamePaused(false);
        } else {
            Logic.fireBall();
        }
    }

    Timer {
        id: burnBricksTimer
        property variant target
        interval: Globals.BURNING_SPEED
        onTriggered: Logic.burnNearbyBricks(target);
    }
}
