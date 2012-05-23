import QtQuick 1.1
import SvgLibrary 1.0
import "globals.js" as Globals

Item {
    id: canvas
    property int m_scale: updateScale();
    
    // needs revision, returns very inaccurate scale
    function updateScale() {
        var scaleX = width/(Globals.BRICK_WIDTH*Globals.WIDTH);
        var scaleY = height/(Globals.BRICK_HEIGHT*Globals.HEIGHT);
        var scale = Math.min(scaleX, scaleY);
        scale = Math.floor(scale*Globals.BRICK_HEIGHT)/Globals.BRICK_HEIGHT;
        if (scale <= 0) scale = 1.0 / Globals.BRICK_HEIGHT;
        return scale;
    }

    CanvasItem {
        id: background
        spriteKey: "Background"
        anchors.fill: parent
    }

    CanvasItem {
        id: bgOverlay
        spriteKey: "BackgroundOverlay"
        anchors.centerIn: parent
        
        width: canvas.m_scale * (Globals.BRICK_WIDTH*Globals.WIDTH + 1)
        height: canvas.m_scale * (Globals.BRICK_HEIGHT*Globals.HEIGHT + 1)
    }
}
