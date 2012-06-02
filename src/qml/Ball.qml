import QtQuick 1.1
import SvgLibrary 1.0
import "globals.js" as Globals

CanvasItem {
    id: ball
    spriteKey: "PlainBall"

    property real directionX
    property real directionY

    property bool toBeFired: true
    property real barPosition: 0.6

    width: m_scale * Globals.BALL_SIZE
    height: width

    x: bar.x + barPosition*bar.width
    y: bar.y - height
}
