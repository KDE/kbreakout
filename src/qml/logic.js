var speed;
var ballComponent = Qt.createComponent("Ball.qml");
var balls = new Array;
var dScore;
var tick = 0;
var repaintInterval;
var randomCounter = 0;

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
    speed = 1.8;
    repaintInterval = 1;
    elapsedTimeTimer.start();
    gameTimer.start();
    showInfoMessage("Press Space to fire the ball");
}

function timerTimeout() {
    dScore *= Globals.SCORE_AUTO_DECREASE;
    for (var i in balls) {
        var ball = balls[i];
        if (ball.toBeFired) continue;

        ball.x += m_scale * ball.directionX * speed;
        ball.y += m_scale * ball.directionY * speed;
    }

    tick = (tick+1) % repaintInterval;

    if (tick==0) {
        updateBallDirection();
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
    hideInfoMessage();

    randomCounter = 0;
}

function moveBar(x) {
    if (x < bgOverlay.x) {
        x = bgOverlay.x;
    } else if (x+bar.width > bgOverlay.x+bgOverlay.width) {
        x = bgOverlay.x + bgOverlay.width - bar.width;
    }

    bar.x = x;
}

function setGamePaused(paused) {
    canvas.paused = paused;
    if (paused) {
        showMessage("Game Paused!");
        elapsedTimeTimer.stop();
        gameTimer.stop();
    } else {
        elapsedTimeTimer.start();
        gameTimer.start();
        hideMessage();
    }
}

function showMessage(text) {
    messageBox.text = text;
}

function showInfoMessage(text) {
    infoMessage.text = text;
}

function hideMessage() {
    messageBox.text = "";
}

function hideInfoMessage() {
    infoMessage.text = "";
}

function updateBallDirection() {
    // avoid infinite loops of the ball
    ++randomCounter;
    if (randomCounter == 1024) {
        randomCounter = 0;
        for (var i in balls) {
            var ball = balls[i];
            if (Math.floor(Math.random()*10) % 2) {
                ball.directionX += 0.002;
            } else {
                ball.directionY += 0.002;
            }
        }

        // increase the speed a little
        // if there is at least one ball moving
        // and the game isn't paused
        var ballMoving = false;
        for (var i in balls) {
            var ball = balls[i];
            if (ball.toBeFired) continue;
            ballMoving = true;
            break;
        }
        if (ballMoving && !paused) {
            changeSpeed(Globals.AUTO_SPEED_INCREASE);
        }
    }
}

function changeSpeed(ratio) {
    speed *= ratio;
    if (speed > 2.0) {
        // make sure the minimum update interval is respected
        if (gameTimer.interval < Globals.MINIMUM_UPDATE_INTERVAL*2) {
            speed = 2.0;
            return;
        }
        // else

        // half the speed
        speed /= 2.0;
        // and double the number of ticks of the timer per time unit
        gameTimer.interval /= 2;
        repaintInterval *= 2;
        gameTimer.restart();
    }
    if (speed < 1.0) {
        if (gameTimer.interval >= Globals.REPAINT_INTERVAL) {
            if (speed < Globals.MINIMUM_SPEED) {
                speed = Globals.MINIMUM_SPEED;
            }
            return;
        }
        // else

        // double the speed
        speed *= 2.0;
        // and half the number of ticks of the timer per time unit
        gameTimer.interval *= 2;
        repaintInterval /= 2;
        gameTimer.restart();
    }
}
