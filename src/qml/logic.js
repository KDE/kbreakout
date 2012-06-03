var ballSpeed = 1.8;
var ballComponent = Qt.createComponent("Ball.qml");
var balls = new Array;
var dScore;

function getTypeFromChar(type) 
{
    switch (type) {
    case '-': return "";
    case '1': return "PlainBrick1";
    case '2': return "PlainBrick2";
    case '3': return "PlainBrick3";
    case '4': return "PlainBrick4";
    case '5': return "PlainBrick5";
    case '6': return "PlainBrick6";
    case 'm': return "MultipleBrick3";
    case 'x': return "ExplodingBrick";
    case 'u': return "UnbreakableBrick";
    case 'h': return "HiddenBrick";
    default:
        print("Invalid File: unknown character '", type, "'");
        return "PlainBrick1";
    }
}

function indexForPos(position) {
    var pos = position.split(",");
    var row = parseInt(pos[0]) - 1;
    var column = parseInt(pos[1]) - 1;
    return row*bricks.columns + column;
}

function putGiftOnRandomBrick(gift, except) {
    var index;
    var brick;
    do {
        index = Math.round(Math.random()*bricks.items.count);
        brick = bricks.items.itemAt(index);
    } while (index==except || brick==null || brick.type=="" || brick.hasGift());
    brick.giftType = gift;
}

function giftlessBricks() {
    var count = 0;
    for (var i=0; i<bricks.items.count; i++) {
        var brick = bricks.items.itemAt(i);
        if (brick!=null && brick.type!="" && !brick.hasGift()) count++;
    }
}

function putGift(gift, times, pos) {
    if (pos!="") {
        // Put gift at given position
        var index = indexForPos(pos);
        var giftBrick = bricks.items.itemAt(index);
        if (giftBrick == null || giftBrick.type == "") {
            print("error:", "Can't place gift at position (", pos, "). There is no brick.");
        }
        else {
            if (giftBrick.hasGift()) {
                // Brick already has a gift -> move this gift to a random position
                putGiftOnRandomBrick(gift, index);
            }
            giftBrick.giftType = gift;
        }
    }
    else {
        // Distribute gifts randomly
        var bricksLeft = giftlessBricks();
        if (bricksLeft < times) {
            print("error:", "Too many gifts of type", gift);
            return;
        }
        for (var i=0; i<times; i++) {
            putGiftOnRandomBrick(gift, -1);
        }
    }
}

function createBall() {
    var ball = ballComponent.createObject(canvas);
    balls.push(ball);
}

function startGame() {
    createBall();
    elapsedTimeTimer.start();
    gameTimer.start();
}

function timerTimeout() {
    dScore *= Globals.SCORE_AUTO_DECREASE;
    for (var i in balls) {
        var ball = balls[i];
        if (ball.toBeFired) continue;

        ball.x += m_scale * ball.directionX * ballSpeed;
        ball.y += m_scale * ball.directionY * ballSpeed;
    }
}

function fireBall() {
    for (var i in balls) {
        var ball = balls[i];
        if (!ball.toBeFired) continue;

        ball.toBeFired = false;
        var ballCenter = ball.x + ball.width/2;
        var barCenter = bar.x + bar.width/2;
        var angle = (Math.PI/3) * (barCenter-ballCenter)/(bar.width/2) + Math.PI/2;

        ball.directionX =  Math.cos(angle) * Globals.BALL_SPEED;
        ball.directionY = -Math.sin(angle) * Globals.BALL_SPEED;
    }

    dScore = Globals.BRICK_SCORE;
}

function moveBar(x) {
    if (x < bgOverlay.x) {
        x = bgOverlay.x;
    } else if (x+bar.width > bgOverlay.x+bgOverlay.width) {
        x = bgOverlay.x + bgOverlay.width - bar.width;
    }

    bar.x = x;
}
