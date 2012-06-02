import QtQuick 1.1
import SvgLibrary 1.0
import "globals.js" as Globals

CanvasItem {
    spriteKey: "PlainBar"
    width: m_scale * Globals.DEFAULT_BAR_WIDTH
    height: m_scale * Globals.BRICK_HEIGHT
}
