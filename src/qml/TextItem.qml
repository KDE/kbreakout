import QtQuick 1.1
import SvgLibrary 1.0
import "globals.js" as Globals

CanvasItem {
    id: item
    spriteKey: "Display"
    property alias text: textItem.text
    property int fontSize
    property int maxFontSize: 60

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
        fontSize = maxFontSize;
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
            if (textItem.width > item.width || textItem.height > item.height) {
                var size = Math.min(item.width * fontSize / textItem.width, item.height * fontSize / textItem.height);
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
