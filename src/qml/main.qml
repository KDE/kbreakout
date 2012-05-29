import QtQuick 1.1
import SvgLibrary 1.0
import "globals.js" as Globals
import "logic.js" as Logic

Item {
    id: canvas
    
    onWidthChanged: updateGeometry();
    onHeightChanged: updateGeometry();

    function updateGeometry() {
        var bw = Globals.BRICK_WIDTH*Globals.WIDTH + 1;
        // bh = (overlayHeight = BRICK_HEIGHT*HEIGHT+1)
        //     +(headerItemsHeight = BRICK_HEIGHT*1.5)
        //     +(margin = headerItemHeight*0.2)
        var bh = Globals.BRICK_HEIGHT*Globals.HEIGHT + 1 + Globals.BRICK_HEIGHT*1.5*1.2;

        var w = canvas.height*bw/bh;
        var h = canvas.width*bh/bw;

        if (w > canvas.width) {
            container.width = canvas.width-20;
            container.height = container.width*bh/bw;
        } else if (h > canvas.height) {
            container.height = canvas.height-50;
            container.width = container.height*bw/bh;
        } else {
            // Never happens, don't know what to do in case it does!
        }
    }

    CanvasItem {
        id: background
        spriteKey: "Background"
        anchors.fill: parent
    }

    Item {
        id: container
        anchors.centerIn: parent
    }

    CanvasItem {
        id: bgOverlay
        spriteKey: "BackgroundOverlay"
        anchors {
            left: container.left
            right: container.right
            bottom: container.bottom
        }
        height: width * (Globals.BRICK_HEIGHT*Globals.HEIGHT+1) / (Globals.BRICK_WIDTH*Globals.WIDTH+1)
    }

    property real scaledBrickWidth: bgOverlay.width/Globals.WIDTH
    property real scaledBrickHeight: scaledBrickWidth * Globals.BRICK_HEIGHT / Globals.BRICK_WIDTH

    property int score: 0
    TextItem {
        id: scoreDisplay
        width: bgOverlay.width/6
        anchors {
            left: bgOverlay.left
            bottom: bgOverlay.top
            bottomMargin: height/5
        }
        text: parent.score
    }

    property int level: 1
    TextItem {
        id: levelDisplay
        width: bgOverlay.width/5
        anchors {
            left: scoreDisplay.right
            leftMargin: width-scoreDisplay.width
            bottom: bgOverlay.top
            bottomMargin: height/5
        }
        text: "Level "+parent.level
    }

    property int lives: Globals.INITIAL_LIVES
    Row {
        id: lifeBars
        spacing: scaledBrickWidth*0.23
        anchors {
            right: bgOverlay.right
            rightMargin: 20
            bottom: bgOverlay.top
            bottomMargin: levelDisplay.height/5
        }

        Repeater {
            model: canvas.lives
            CanvasItem {
                spriteKey: "PlainBar"
                width: scaledBrickWidth/1.3
                height: scaledBrickHeight/1.3
            }
        }
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
                width: bgOverlay.width/Globals.WIDTH
            }
        }
    }
}
