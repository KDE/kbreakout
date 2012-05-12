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
}
