import QtQuick 1.1

Timer {
    signal timeout(variant target)

    property variant target

    onTriggered: {
        timeout(target);
        destroy(10);
    }
}
