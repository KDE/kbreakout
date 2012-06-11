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
    id: brick
    property int row
    property int column
    property int blockWidth: giftType!="" && spriteKey==giftType ? Globals.GIFT_WIDTH : Globals.BRICK_WIDTH
    property int blockHeight: giftType!="" && spriteKey==giftType ? Globals.GIFT_HEIGHT : Globals.BRICK_HEIGHT

    objectName: hidden || giftVisible ? "" : "brick"

    width: m_scale * blockWidth
    height: m_scale * blockHeight

    x: m_scale * (column*Globals.BRICK_WIDTH)
    y: m_scale * (row*Globals.BRICK_HEIGHT)

    Behavior on y { SpringAnimation { spring: 2; damping: 0.2 } }

    property string type
    property string giftType

    onTypeChanged: show();

    property bool hidden: spriteKey==""
    function hide() { spriteKey = ""; }
    function show() {
        if (type != "HiddenBrick") {
            spriteKey = type;
        }
    }
    function forceShow() {
        spriteKey = type;
    }

    property bool giftVisible: spriteKey==giftType
    function showGift() { spriteKey = giftType; }
    function hasGift() { return giftType!=""; }

    Timer {
        id: hideTimer
        interval: Globals.BURNING_INTERVAL
        onTriggered: hide();
    }

    function hideLater() {
        hideTimer.start();
    }
}
