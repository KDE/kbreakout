import QtQuick 1.1
import org.kde.games.core 0.1 as GamesCore

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
