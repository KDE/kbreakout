import QtQuick 1.1
import SvgLibrary 1.0

Item {
    id: canvas

    CanvasItem {
        id: background
        spriteKey: "Background"
        anchors.fill: parent

        CanvasItem {
            id: ball
            spriteKey: "PlainBall"
            anchors {
                horizontalCenter: parent.horizontalCenter
                bottom: parent.bottom
                bottomMargin: 20
            }
        }
    }
}
