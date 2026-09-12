/*
    SPDX-FileCopyrightText: 2012 Viranch Mehta <viranch.mehta@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

import QtQuick 2.3
import org.kde.games.core 0.1 as KgCore
import "globals.js" as Globals
import QtMultimedia

CanvasItem {
    id: ball
    spriteKey: "PlainBall"

    property string type
    onTypeChanged: spriteKey = type;

    property real directionX
    property real directionY

    property bool toBeFired: true
    property real barPosition: 0.6

    property bool wallCollision: false
    property bool barCollision: false

    width: m_scale * Globals.BALL_SIZE
    height: m_scale * Globals.BALL_SIZE

    // for preserving position relative
    // to bgOverlay when canvas is resized
    property real posX
    property real posY
    x: m_scale * posX
    y: m_scale * posY

    SoundEffect {
        id: wallCollisionSound
        source: "../sounds/ball-bounce-wall.wav"
    }

    SoundEffect {
        id: barCollisionSound
        source: "../sounds/ball-bounce.wav"
    }

    onWallCollisionChanged: { if (wallCollision) wallCollisionSound.play() }
    onBarCollisionChanged: { if (barCollision) barCollisionSound.play() }
}
