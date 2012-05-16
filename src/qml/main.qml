import QtQuick 1.1
import SvgLibrary 1.0

Item {
    id: canvas

    CanvasItem {
        id: ball
        spriteKey: "PlainBall"
        anchors.centerIn: parent
    }

    Rectangle {
        id: black
        anchors.fill: ball
        color: "black"
        opacity: 0.3
    }

    MouseArea {
        anchors.fill: parent
        onClicked: print(black.width, black.height);
    }
}
