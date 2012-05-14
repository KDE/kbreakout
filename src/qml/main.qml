import QtQuick 1.1

Item {
    Rectangle {
        width: 300
        height: 200
        anchors.centerIn: parent
        color: "green"
        MouseArea {
            anchors.fill: parent
            onClicked: parent.opacity-=0.1
        }
    }

    function reloadSprites() { print("theme changed"); }

    CanvasItem {
        id: ball
        spriteKey: "PlainBall"
    }
    Rectangle {
        width: ball.width+10
        height: ball.height+10
        anchors { top: ball.bottom; left: ball.right }
        color: "black"
        opacity: 0.6
    }
}
