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
    id: item
    spriteKey: "Display"
    property alias text: textItem.text
    property int fontSize

    Text {
        id: textItem
        anchors.centerIn: parent
        opacity: 220/256
        font { bold: true; family: "Helvetica"; pixelSize: fontSize }
        color: "white"
    }

    onWidthChanged: fontTimerTrigger.restart();
    onHeightChanged: fontTimerTrigger.restart();

    function updateFontSize() {
        fontSize = 72;
        fontTimer.start();
    }
 
    Timer {
        id: fontTimerTrigger
        interval: 100
        onTriggered: {
            if (!fontTimer.running)
                updateFontSize();
        }
    }

    Timer {
        id: fontTimer
        interval: 1
        repeat: true
        onTriggered: {
            var w = item.width*0.8;
            var h = item.height;
            if (textItem.width > w || textItem.height > h) {
                var size = Math.min(w * fontSize / textItem.width, h * fontSize / textItem.height);
                size = Math.floor(size);
                if (size == fontSize) {
                    stop();
                } else {
                    fontSize = size;
                }
            } else {
                stop();
            }
        }
    }
}
