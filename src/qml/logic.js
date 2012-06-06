/*
    Copyright 2012 Viranch Mehta <viranch.mehta@gmail.com>
  
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.
   
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
   
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

var speed;
var ballComponent = Qt.createComponent("Ball.qml");
var balls = new Array;
var dScore;
var tick = 0;
var repaintInterval;
var randomCounter = 0;
var itemsGotDeleted;
var gameOver = false;
var gameWon = false;

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

function startLevel() {
    showMessage("Level "+level);
    hideLater(messageBox, 2000);
    resumeGame();
}

// TODO: loadNextLevel

// similar to GameEngine::loadLevel() in terms of setting state variables
function resumeGame() {
    createBall();
    speed = 1.8;
    repaintInterval = 1;
    elapsedTimeTimer.start();
    gameTimer.interval = Globals.REPAINT_INTERVAL;
    gameTimer.start();
    showInfoMessage("Press Space to fire the ball");
}

function timerTimeout() {
    // needed to exit from the loop if the arrays that they cycle
    // change (items get deleted)
    itemsGotDeleted = false;

    dScore *= Globals.SCORE_AUTO_DECREASE;
    for (var i in balls) {
        var ball = balls[i];
        if (ball.toBeFired) continue;

        ball.x += m_scale * ball.directionX * speed;
        ball.y += m_scale * ball.directionY * speed;

        // collision detection
        firstTime = true;
        detectBallCollisions(ball);
        if (itemsGotDeleted) {
            return;
        }
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
    messageBox.updateFontSize();
}

function showInfoMessage(text) {
    infoMessage.text = text;
    infoMessage.updateFontSize();
}

function hideMessage() {
    messageBox.text = "";
}

function hideInfoMessage() {
    infoMessage.text = "";
}

function hideLater(target, interval) {
    hideTimer.target = target;
    hideTimer.interval = interval;
    hideTimer.start();
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

function createRect(object) {
    var globalPos = object.parent.mapToItem(null, object.x, object.y);
    return {
        left: globalPos.x,
        top: globalPos.y,
        right: globalPos.x + object.width,
        bottom: globalPos.y + object.height
    };
}

function intersects(r1, r2) {
    return !(r2.left > r1.right ||
             r2.right < r1.left ||
             r2.top > r1.bottom ||
             r2.bottom < r1.top);
}

function intersectArea(r1, r2) {
    return (Math.min(r1.right, r2.right) - Math.max(r1.left, r2.left)) *
           (Math.min(r1.bottom, r2.bottom) - Math.max(r1.top, r2.top));
}

// never run this function more than two times recursively
var firstTime = true;
function detectBallCollisions(ball) {
    // bounce a little early in some cases so the average position is centered
    var rect = createRect(ball);
    rect.left += ball.directionX/2;
    rect.top += ball.directionY/2;
    var x = rect.left;
    var y = rect.top;

    var barRect = createRect(bar);

    // bounce against the wall
    if (x < bgOverlay.x && ball.directionX < 0) {
        ball.directionX *= -1;
    } else if (x+ball.width > bgOverlay.x+bgOverlay.width
                && ball.directionX > 0) {
        ball.directionX *= -1;
    } else if (y < bgOverlay.y && ball.directionY < 0) {
        ball.directionY *= -1;
    } else if (y+ball.height > bgOverlay.y+bgOverlay.height
                && ball.directionY > 0) {
        // delete the ball
        balls.splice(balls.indexOf(ball), 1);
        ball.destroy();
        itemsGotDeleted = true;
        if (balls.length == 0) {
            showMessage("Oops! You have lost the ball!");
            handleDeathTimer.start();
            gameTimer.stop();

            //TODO: delete moving objects
        }
        return;
    }
    // bounce against the bar
    else if (intersects(rect, barRect) && ball.directionY > 0) {
        
        var ballCenter = ball.x + ball.width/2;
        var barCenter = bar.x + bar.width/2;

        if (ballCenter > bar.x &&
                ballCenter < bar.x+bar.width) {
            // the bar has been hit

            if (bar.type() == "StickyBar") {
                ball.toBeFired = true;

                diff = ball.x - bar.x;

                ball.barPosition = diff / bar.width;
                //TODO: update attached balls
            }

            var angle = (Math.PI/3) * (barCenter-ballCenter)/(bar.width/2) + Math.PI/2;
            var speed = Math.sqrt(Math.pow(ball.directionX, 2) +
                                Math.pow(ball.directionY, 2));

            ball.directionX =  Math.cos(angle) * speed;
            ball.directionY = -Math.sin(angle) * speed;
        }
    } else { // bounce against the bricks (and optionally break them)
        handleBrickCollisions(ball);
    }

    // never run this function more than two times recursively
    if (firstTime) {
        firstTime = false;
        if (!itemsGotDeleted) {
            detectBallCollisions(ball);
        }
    } else {
        firstTime = true;
        return;
    }
}

function handleDeath() {
    hideMessage();
    // TODO: delete moving objects
    // TODO: bar.reset
    if (lives == 0) {
        gameOver = true;
        showMessage("Game Over!");
        elapsedTimeTimer.stop();
        moveBarTimer.stop();
        canvas.gameEnded(score, level, elapsedTimeTimer.elapsedTime);
    } else {
        lives--;
        resumeGame();
    }
}

function handleBrickCollisions(ball) {
    if (itemsGotDeleted) {
        return;
    }
    var rect = createRect(ball);
    var intersectingBricks = new Array;

    for (var i=0; i<brickItems.count; i++) {
        var brick = brickItems.itemAt(i);
        if (brick.type == "") continue;
        var brickRect = createRect(brick);

        if (intersects(brickRect, rect)) {
            intersectingBricks.push(brick);
        }
    }

    if (intersectingBricks.length != 0) {
        collideWithBricks(ball, intersectingBricks);
    }
}

function collideWithBricks(ball, bricks) {
    if (bricks.length == 2) {
        collideWithTwoBricks(ball, bricks);
    } else {
        collideWithBrick(ball, bricks[0]);
    }
}

function collideWithTwoBricks(ball, bricks) {
    var ballRect = createRect(ball);
    var r1 = createRect(bricks[0]);
    var r2 = createRect(bricks[1]);

    var area1 = intersectArea(ball, r1);
    var area2 = intersectArea(ball, r2);

    if (area1 > area2) {
        // the area of intersection with the first brick is bigger
        collideWithBrick(ball, bricsk[0]);
    } else {
        collideWithBrick(ball, bricks[1]);
    }
}

function collideWithBrick(ball, brick) {
    if (ball.type() == "UnstoppableBall") {
        forcedHit(brick);
        return; // don't bounce
    }
    if (ball.type() == "UnstoppableBurningBall") {
        explode(brick);
        return; // don't bounce
    }

    // calculate bounce
    var brickRect = createRect(brick);
    var ballRect = createRect(ball);

    var top = Math.round(ballRect.bottom - brickRect.top);
    var bottom = Math.round(brickRect.bottom - ballRect.top);
    var left = Math.round(ballRect.right - brickRect.left);
    var right = Math.round(brickRect.right - ballRect.left);
    var min = Math.min(Math.min(top, bottom), Math.min(left, right));

    // bounce
    if (min == top && ball.directionY > 0) {
        ball.directionY *= -1;
        ball.y -= top;
    } else if (min == bottom && ball.directionY < 0) {
        ball.directionY *= -1;
        ball.y += bottom;
    } else if (min == left && ball.directionX > 0) {
        ball.directionX *= -1;
        ball.x -= left;
    } else if (min == right && ball.directionX < 0) {
        ball.directionX *= -1;
        ball.x += right;
    } else {
        return; // already bounced
    }

    if (ball.type() == "BurningBall") {
        explode(brick);
    } else {
        hit(brick);
    }
}

var remainingBricks=0;

function addBrickScore() {
    score += Math.round(dScore);
    dScore = Globals.BRICK_SCORE;
}

function forcedHit(brick) {
    if (brick.type == "") return;

    if (brick.type == "ExplodingBrick") {
        explode(brick);
    } else {
        handleDeletion(brick);
    }

    brick.hide();
}

function hit(brick) {
    if (brick.type == "HiddenBrick" && brick.spriteKey=="") {
        brick.spriteKey = brick.type;
        ++remainingBricks;
    } else if (brick.type == "MultipleBrick3") {
        brick.type = "MultipleBrick2";
        addBrickScore();
    } else if (brick.type == "MultipleBrick2") {
        brick.type = "MultipleBrick1";
        addBrickScore();
    } else if (brick.type == "ExplodingBrick") {
        explode(brick);
    } else if (brick.type != "UnbreakableBrick") {
        forcedHit(brick);
    }
}

function explode(brick) {
    if (brick.type == "") return;

    burn(brick);
    burnNearbyBricksLater(brick);
}

function burnNearbyBricksLater(brick) {
    burnBricksTimer.target = brick;
    burnBricksTimer.start();
}

function burnNearbyBricks(brick) {
    var bricks = nearbyBricks(brick);
    for (var i in bricks) {
        burn(brickItems.itemAt(bricks[i]));
    }
}

function burn(brick) {
    if (brick.type == "") return;

    if (brick.type == "ExplodingBrick") {
        // make sure it doesn't explode twice
        brick.type = "BurningBrick";
        explode(brick);
    } else {
        handleDeletion(brick);
    }

    brick.type = "BurningBrick";
    //brick.spriteKey = brick.type; // make sure hidden bricks are shown
    brick.hideTimer.start();
}

function handleDeletion(brick) {
    var brickType = brick.type;
    brick.type = "";
    if (brick.hasGift()) {
        // TODO: showGift
    }

    --remainingBricks;
    addBrickScore();

    // these two kind of bricks aren't counted in remainingBricks
    if (brickType == "HiddenBrick" || brickType == "UnbreakableBrick") {
        ++remainingBricks;
        return; // never need to load the next level
    }

    if (remainingBricks == 0) {
        showMessage("Level complete!");
    }
}

function nearbyBricks(brick) {
    var i = brick.index;
    var bricks = new Array;
    var points = new Array(i-1, i+1, i-Globals.WIDTH, i+Globals.WIDTH);
    for (var j in points) {
        if (brickItems.itemAt(j).type != "") {
            bricks.push(j);
        }
    }
    return bricks;
}
