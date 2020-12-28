/*
    SPDX-FileCopyrightText: 2012 Viranch Mehta <viranch.mehta@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

import QtQuick 2.3
import org.kde.games.core 0.1 as KgCore
import "globals.js" as Globals

CanvasItem {
    id: brick
    property int row
    property int column

    width: m_scale * Globals.BRICK_WIDTH
    height: m_scale * Globals.BRICK_HEIGHT

    x: m_scale * (column*Globals.BRICK_WIDTH)
    y: m_scale * (row*Globals.BRICK_HEIGHT)

    Behavior on y { SpringAnimation { spring: 2; damping: 0.2 } }

    property string type
    onTypeChanged: spriteKey = type;

    visible: type!="HiddenBrick"

    property string giftType
    property bool hasGift: giftType!=""
}
