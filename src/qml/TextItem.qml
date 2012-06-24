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

    onTextChanged: fontTimerTrigger.restart();

    // assign largest font size and start the timer to update
    // font size to fit text
    function updateFontSize() {
        fontSize = 72/m_scale;
        fontTimer.start();
    }
 
    // do not update font size more frequently than once in 100ms
    Timer {
        id: fontTimerTrigger
        interval: 100
        onTriggered: {
            if (!fontTimer.running)
                updateFontSize();
        }
    }

    // check font size every 1ms; if text does not fit, decrease the font size
    // stop the timer, if text fits
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
                    opacity = 1;
                } else {
                    fontSize = size;
                }
            } else {
                stop();
                opacity = 1;
            }
        }
    }
}
