import QtQuick 1.1
import SvgLibrary 1.0
import "globals.js" as Globals

CanvasItem {
    id: item
    spriteKey: "Display"
    height: bgOverlay.width * (Globals.BRICK_HEIGHT*1.3) / (Globals.BRICK_WIDTH*Globals.WIDTH)
    property alias text: textItem.text
    property int fontSize

    Text {
        id: textItem
        anchors.centerIn: parent
        opacity: 220/256
        font { bold: true; family: "Helvetica"; pixelSize: fontSize }
        color: "white"
    }

    onWidthChanged: updateFontSize();
    onHeightChanged: updateFontSize();

    function updateFontSize() {
        fontSize = 72;
        fontTimer.start();
    }

    Timer {
        id: fontTimer
        interval: 1
        repeat: true
        onTriggered: {
            if (textItem.width > item.width || textItem.height > item.height) {
                var size = Math.min(item.width * fontSize / textItem.width, item.height * fontSize / textItem.height);
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
