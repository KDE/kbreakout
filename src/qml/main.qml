/*
    SPDX-FileCopyrightText: 2012 Viranch Mehta <viranch.mehta@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

import QtQuick 2.3
import org.kde.games.core 0.1 as KgCore
import "globals.js" as Globals
import "logic.js" as Logic

Item {
    id: canvas

    signal levelComplete()
    signal gameEnded(int score, int level, int elapsedTime)
    signal mousePressed()

    property real speed
    property bool paused: false
    property bool gameOver: false
    property bool gameWon: false
    readonly property Item jailItem: bgOverlay
    readonly property bool ballMoving: !paused && fireBallMessage.opacity !== 1

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

    property int barCenter: mapFromItem(bgOverlay, Math.round(bar.x + bar.width/2), 0).x

    CanvasItem {
        id: bgOverlay
        spriteKey: "BackgroundOverlay"
        anchors {
            left: container.left
            bottom: container.bottom
        }
        width: m_scale * (Globals.BRICK_WIDTH*Globals.WIDTH + 1)
        height: m_scale * (Globals.BRICK_HEIGHT*Globals.HEIGHT + 1)

        Bar {
            id: bar
            anchors {
                bottom: parent.bottom
            }
            posX: 300
        }
    }

    Rectangle {
        id: pauseOverlay
        anchors.fill: parent
        color: "#646464"
        opacity: paused ? 0.6 : 0
        z: 1 // to show above all the elements (except messageBox)

        Behavior on opacity { NumberAnimation { duration: 100 } }
    }
    function setGamePaused(paused) {
        Logic.setGamePaused(paused);
    }

    TextItem {
        id: messageBox
        anchors.centerIn: bgOverlay
        z: 2 // to make it display above the pause overlay
        width: m_scale * Globals.BRICK_WIDTH*9
        height: m_scale * Globals.BRICK_HEIGHT*5
        opacity: 0

        Behavior on opacity { NumberAnimation { duration: 100 } }
    }

    property string fireShortcut: "Space"
    TextItem {
        id: fireBallMessage
        anchors {
            horizontalCenter: bgOverlay.horizontalCenter
            bottom: bgOverlay.bottom
            bottomMargin: (bgOverlay.height-height)/4
        }
        width: m_scale * Globals.BRICK_WIDTH*9
        height: m_scale * Globals.BRICK_HEIGHT*2
        text: i18n("Press %1 to fire the ball", fireShortcut)
        opacity: 0

        Behavior on opacity { NumberAnimation { duration: 100 } }
    }

    property int score
    TextItem {
        id: scoreDisplay
        width: m_scale * (Globals.BRICK_WIDTH*Globals.WIDTH)/6
        height: m_scale * Globals.BRICK_HEIGHT*1.5
        anchors {
            left: bgOverlay.left
            bottom: bgOverlay.top
            bottomMargin: height/5
        }
        text: Logic.scoreString(parent.score)
    }

    property int level
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
        text: i18n("Level %1", parent.level)
    }

    property int lives
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

    function reset() {
        Logic.reset();
    }

    function loadLine(line, lineNumber) {
        Logic.showLine(line, lineNumber);
    }

    function loadGift(gift, times, pos) {
        Logic.putGift(gift, times, pos);
    }

    function updateBarDirection(direction) {
        bar.direction = direction;
    }

    function startGame() {
        Logic.startLevel();
    }

    Timer {
        id: gameTimer
        interval: Globals.REPAINT_INTERVAL
        repeat: true
        onTriggered: {
            for (x = 0; x < Logic.substeps; ++x) {
                Logic.moveBalls()
                Logic.detectCollisions()
            }
        }
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

    // hides the current showed message by target
    // unless the game is paused, won or game over
    Timer {
        id: hideTimer
        property variant target
        onTriggered: {
            if (paused) return;
            target.opacity = 0;
        }
    }

    function fire() {
        Logic.fireBall();
    }

    function cheatAddLife() {
        Logic.addLife();
    }

    function cheatSkipLevel() {
        Logic.loadNextLevel();
    }

    Timer {
        id: burnBricksTimer
        property variant target
        interval: Globals.BURNING_SPEED
        onTriggered: Logic.burnNearbyBricks(target);
    }

    MouseArea {
        id: mouseArea
        enabled: false
        anchors.fill: parent
        hoverEnabled: true
        onPositionChanged: mouse => {
            if (paused) return;

            // avoids accidentally moving the mouse while playing using the keys
            if (bar.direction != 0) return;

            var barX = canvas.mapToItem(bgOverlay, mouse.x, 0).x - bar.width/2;
            bar.moveTo(barX/m_scale);
        }
        onClicked: canvas.mousePressed()
    }

    Item {
        id: keyEventHandler
        focus: true
        anchors.fill: parent
        Keys.onPressed: event => {
            if (event.key == Qt.Key_Right) {
                updateBarDirection(1);
            } else if (event.key == Qt.Key_Left) {
                updateBarDirection(-1);
            }
        }

        Keys.onReleased: event => {
            if (event.key == Qt.Key_Right || event.key == Qt.Key_Left) {
                updateBarDirection(0);
            }
        }
    }
}
