import QtQuick 1.1
import SvgLibrary 1.0
import "globals.js" as Globals
import "logic.js" as Logic

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

    function loadLine(line, lineNumber) {
        var start = (lineNumber-1)*Globals.WIDTH;
        brickString = brickString.slice(0, start) + line + brickString.slice(start+line.length);
    }

    function loadGift(gift, times, pos) {
        Logic.putGift(gift, times, pos);
    }

    property string brickString
    
    Component.onCompleted: {
        var emptyBricks="";
        for(var i=0; i<Globals.WIDTH*Globals.HEIGHT; i++)
            emptyBricks += "-";
        brickString = emptyBricks;
    }

    Grid {
        id: bricks
        columns: Globals.WIDTH
        anchors.fill: bgOverlay
        property alias items: brickItems
        
        Repeater {
            id: brickItems
            model: brickString.split("")
            Brick {
                type: Logic.getTypeFromChar(modelData)
                width: bgOverlay.width/20
            }
        }
    }
}
