import QtQuick 1.1
import SvgLibrary 1.0

Item {
    ThemedImage {
        id: ball
        //anchors.centerIn: parent
        x:50; y:50
        spriteKey: "PlainBall"
        scale: 2
        //width: 30; height: 10
    }

    /*CanvasItem {
        id: ball
        spriteKey: "PlainBall"
    }*/

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
