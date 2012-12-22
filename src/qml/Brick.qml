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

KgCore.CanvasItem {
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
