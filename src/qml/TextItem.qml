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

import QtQuick 2.3
import org.kde.games.core 0.1 as KgCore
import "globals.js" as Globals

CanvasItem {
    id: item
    spriteKey: "Display"
    property alias text: textItem.text
    property int fontSize

    Text {
        id: textItem
        anchors.centerIn: parent
        opacity: 220/256
        font { bold: true; family: "Helvetica"; pixelSize: fontSize*m_scale }
        color: "white"
    }

    onTextChanged: updateFontSize();
    Component.onCompleted: updateTimer.start();

    Timer {
        id: updateTimer
        interval: 1
        onTriggered: updateFontSize();
    }

    function updateFontSize() {
        opacity = 0;
        fontSize = 72/m_scale;

        var w = item.width*0.8;
        var h = item.height;
        while (textItem.width > w || textItem.height > h) {
            var size = Math.min(w * fontSize / textItem.width, h * fontSize / textItem.height);
            size = Math.floor(size);
            if (size == fontSize) {
                break;
            } else {
                fontSize = size;
            }
        }

        opacity = 1;
    }
}
