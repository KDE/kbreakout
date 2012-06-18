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

var ballComponent = Qt.createComponent("Ball.qml");
var balls = new Array;
var brickComponent = Qt.createComponent("Brick.qml");
var bricks = new Array;
var giftComponent = Qt.createComponent("Gift.qml");
var gifts = new Array;
// score to add if you hit a brick
// decreases over time since last hit
var dScore;
// count of remaining bricks
// (not counting the unbreakable ones)
var remainingBricks=0;
var tick = 0;
var repaintInterval;
var randomCounter = 0;
// is set to true when deleteMovingObjects() is called
var itemsGotDeleted;
var singleShotComponent = Qt.createComponent("Singleshot.qml");

function remove(array, object) {
    array.splice(array.indexOf(object), 1);
}

function singleShot(interval, slot, target) {
    var timer = singleShotComponent.createObject(canvas);
    timer.interval = interval;
    timer.target = target;
    timer.timeout.connect(slot);
    timer.start();
}

function scoreString(score) {
    var absScore = Math.abs(score);
    var str = absScore.toString().split("");

    // insert spaces every 3 characters
    var start = str.length%3;
    if (start==0) start=3;
    for (; start<str.length; start+=4) {
        str.splice(start, 0, " ");
    }
    
    // join the array
    var displayString = score<0 ? "-" : "";
    for (var i in str) {
        displayString += str[i];
    }
    return displayString;
}

function reset() {
    level = 0;
    score = 0;
    resetBricks();
    bar.reset();
    gameOver = false;
    gameWon = false;
    deleteMovingObjects();
    paused = false;
    lives = Globals.INITIAL_LIVES;
    mouseArea.enabled = true;
}

function resetBricks() {
    for (var i in bricks) {
        bricks[i].destroy();
    }
    bricks = new Array;
    remainingBricks = 0;
}

function getTypeFromChar(type) 
{
    switch (type) {
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
    var column = parseInt(pos[0]) - 1;
    var row = parseInt(pos[1]) - 1;
    return index(row, column);
}

function index(row, column) {
    for (var i in bricks) {
        var brick = bricks[i];
        if (brick.row==row && brick.column==column) {
            return i;
        }
    }
    return -1;
}

function putGiftOnRandomBrick(gift, except) {
    var index;
    var brick;
    do {
        index = Math.round(Math.random()*bricks.length);
        brick = bricks[index];
    } while (index==except || brick==null || brick.hasGift);
    brick.giftType = gift;
}

function giftlessBricks() {
    var count = 0;
    for (var i=0; i<bricks.length; i++) {
        var brick = bricks[i];
        if (!brick.hasGift) count++;
    }
}

function createBrick(row, column, type) {
    var brick = brickComponent.createObject(bgOverlay);
    brick.row = row;
    brick.column = column;
    brick.type = type;
    bricks.push(brick);
}

function showLine(line, number) {
    for (var i=0; i<line.length; i++) {
        var c = line[i];
        if (c == '-') continue;
        createBrick(number-1, i, getTypeFromChar(c));
        if (c!='h' && c!='u') ++remainingBricks;
    }
}

function putGift(gift, times, pos) {
    if (pos!="") {
        // Put gift at given position
        var index = indexForPos(pos);
        if (index == -1) {
            print("error:", "Can't place gift at position (", pos, "). There is no brick.");
        }
        else {
            var giftBrick = bricks[index];
            if (giftBrick.hasGift) {
                // Brick already has a gift -> move this gift to a random position
                putGiftOnRandomBrick(giftBrick.giftType, index);
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
    var ball = ballComponent.createObject(bgOverlay);
    ball.posX = (bar.x + ball.barPosition*bar.width)/m_scale;
    ball.posY = (bar.y - ball.height)/m_scale;
    balls.push(ball);
}

function startLevel() {
    if (bricks.length==0) {
        if (level == 0) {
            // No level in the levelset
            print("Invalid levelset");
        } else {
            // No more levels: game won
            gameWon = true;
            score += Globals.GAME_WON_SCORE + lives*Globals.LIFE_SCORE;
            showMessage(i18n("Well done! You won the game"));
            singleShot(10, endGame, null);
            deleteMovingObjects();
        }
        return;
    }
    ++level;
    showMessage(i18n("Level %1", level));
    hideLater(messageBox, 2000);
    resumeGame();
}

function loadNextLevel() {
    // assign points for each remaining brick
    for (var i=0; i<bricks.length; i++) {
        var type = bricks[i].type;
        // don't assign points for unbreakable bricks
        if (type == "UnbreakableBrick") {
            continue;
        }
        if (type == "") {
            continue;
        }
        score += Globals.AUTOBRICK_SCORE;

        // add extra points for multiple bricks
        if (type == "MultipleBrick3") {
            score += Globals.AUTOBRICK_SCORE*2;
        } else if (type == "MultipleBrick2") {
            score += Globals.AUTOBRICK_SCORE;
        }
    }
    deleteMovingObjects();
    score += Globals.LEVEL_SCORE;
    resetBricks();
    canvas.levelComplete();
}

function resumeGame() {
    createBall();
    bar.reset();
    speed = 1.8;
    repaintInterval = 1;
    elapsedTimeTimer.restart();
    gameTimer.interval = Globals.REPAINT_INTERVAL;
    gameTimer.restart();
    showFireBallMessage();
}

function endGame() {
    canvas.gameEnded(score, (gameWon ? -1 : level), elapsedTimeTimer.elapsedTime);
}

function detectCollisions() {
    // needed to exit from the loop if the arrays that they cycle
    // change (items get deleted)
    itemsGotDeleted = false;

    dScore *= Globals.SCORE_AUTO_DECREASE;
    for (var i in balls) {
        var ball = balls[i];
        if (ball.toBeFired) {
            continue;
        }

        // collision detection
        firstTime = true;
        detectBallCollisions(ball);
        if (itemsGotDeleted) {
            return;
        }
    }

    for (var i=0; i<gifts.length; i++) {
        var gift = gifts[i];
        var giftRect = createRect(gift);
        var barRect = createRect(bar);
        if (gift.y+gift.height > bgOverlay.height) {
            remove(gifts, gift);
            gift.destroy();
            i--;
        } else if (intersects(giftRect, barRect)) {
            executeGift(gift.type);
            if (itemsGotDeleted) {
                return;
            }
            remove(gifts, gift);
            gift.destroy();
            i--;
        }
    }

    tick = (tick+1) % repaintInterval;

    if (tick==0) {
        updateBallDirection();
    }
}

function fireBall() {
    if (paused) {
        print("trying to fire while game is paused!!!");
        return;
    }

    for (var i in balls) {
        var ball = balls[i];
        if (!ball.toBeFired) continue;

        var ballCenter = ball.x + ball.width/2;
        var barCenter = bar.x + bar.width/2;
        var angle = (Math.PI/3) * (barCenter-ballCenter)/(bar.width/2) + Math.PI/2;

        ball.directionX =  Math.cos(angle) * Globals.BALL_SPEED;
        ball.directionY = -Math.sin(angle) * Globals.BALL_SPEED;
        ball.toBeFired = false;
    }

    dScore = Globals.BRICK_SCORE;
    hideFireBallMessage();

    randomCounter = 0;
}

function setGamePaused(paused) {
    if (gameOver || gameWon || canvas.paused==paused) return;
    canvas.paused = paused;
    if (paused) {
        showMessage(i18n("Game Paused!"));
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

function showFireBallMessage(text) {
    fireBallMessage.opacity = 1;
}

function hideMessage() {
    messageBox.text = "";
}

function hideFireBallMessage() {
    fireBallMessage.opacity = 0;
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
            if (!balls[i].toBeFired) {
                ballMoving = true;
                break;
            }
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

function deleteMovingObjects() {
    itemsGotDeleted = true;
    var times = balls.length
    for (var i=0; i<times; i++) {
        balls.shift().destroy();
    }
    
    times = gifts.length
    for (var i=0; i<times; i++) {
        gifts.shift().destroy();
    }
}

function createRect(object) {
    var globalPos = object.parent.mapToItem(bgOverlay, object.x, object.y);
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
    if (x < 0 && ball.directionX < 0) {
        ball.directionX *= -1;
    } else if (x+ball.width > bgOverlay.width
                && ball.directionX > 0) {
        ball.directionX *= -1;
    } else if (y < 0 && ball.directionY < 0) {
        ball.directionY *= -1;
    } else if (y+ball.height > bgOverlay.height
                && ball.directionY > 0) {
        // delete the ball
        remove(balls, ball);
        ball.destroy();
        itemsGotDeleted = true;
        if (balls.length == 0) {
            showMessage(i18n("Oops! You have lost the ball!"));
            handleDeathTimer.start();
            gameTimer.stop();

            deleteMovingObjects();
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

            if (bar.type == "StickyBar") {
                ball.toBeFired = true;

                var diff = ball.x - bar.x;

                ball.barPosition = diff / bar.width;
                ball.posY = (bar.y-ball.height)/m_scale;
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
        // check if there is another collision
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
    deleteMovingObjects();
    bar.reset();
    if (lives == 0) {
        gameOver = true;
        showMessage(i18n("Game Over!"));
        elapsedTimeTimer.stop();
        bar.stopMoving();
        singleShot(10, endGame, null);
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

    for (var i=0; i<bricks.length; i++) {
        var brick = bricks[i];
        if (brick == null) continue;
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
    if (ball.type == "UnstoppableBall") {
        forcedHit(brick);
        return; // don't bounce
    }
    if (ball.type == "UnstoppableBurningBall") {
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
        ball.posY -= top/m_scale;
    } else if (min == bottom && ball.directionY < 0) {
        ball.directionY *= -1;
        ball.posY += bottom/m_scale;
    } else if (min == left && ball.directionX > 0) {
        ball.directionX *= -1;
        ball.posX -= left/m_scale;
    } else if (min == right && ball.directionX < 0) {
        ball.directionX *= -1;
        ball.posX += right/m_scale;
    } else {
        return; // already bounced
    }

    if (ball.type == "BurningBall") {
        explode(brick);
    } else {
        hit(brick);
    }
}

function addBrickScore() {
    score += Math.round(dScore);
    dScore = Globals.BRICK_SCORE;
}

function forcedHit(brick) {
    if (brick == null) return;

    if (brick.type == "ExplodingBrick") {
        explode(brick);
    } else {
        handleDeletion(brick);
    }
}

function hit(brick) {
    if (brick.type == "HiddenBrick" && !brick.visible) {
        brick.visible = true;
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
        handleDeletion(brick);
    }
}

function explode(brick) {
    if (brick == null) return;

    burn(brick);
    singleShot(Globals.BURNING_SPEED, burnNearbyBricks, createRect(brick));
}

function burnNearbyBricks(brickRect) {
    var nearby = nearbyBricks(brickRect);
    for (var i in nearby) {
        burn(nearby[i]);
    }
}

function burn(brick) {
    if (brick == null) return;

    if (brick.type == "ExplodingBrick") {
        // make sure it doesn't explode twice
        brick.type = "BurningBrick";
        explode(brick);
    } else {
        singleShot(Globals.BURNING_SPEED, deleteBrick, new Array(brick.x, brick.y));
    }

    brick.type = "BurningBrick";
}

// wrapper function to get the brick from position
// and then passing it to handleDeletion
function deleteBrick(pos) {
    var brick = bgOverlay.childAt(pos[0], pos[1]);
    if (brick!=null && bricks.indexOf(brick)>=0) {
        handleDeletion(brick);
    }
}

function createGiftAt(brick) {
    var gift = giftComponent.createObject(bgOverlay);
    gift.type = brick.giftType;
    gift.setPosition(brick.x/m_scale, brick.y/m_scale);
    gift.falling = true;
    gifts.push(gift);
}

function handleDeletion(brick) {
    remove(bricks, brick);
    if (brick.hasGift) {
        createGiftAt(brick);
    }

    // take the properties we want later before destroying the brick
    var brickType = brick.type;
    var brickVisible = brick.visible;
    brick.destroy();

    --remainingBricks;
    addBrickScore();

    // these two kind of bricks aren't counted in remainingBricks
    if ( (brickType == "HiddenBrick" && !brickVisible)
            || brickType == "UnbreakableBrick") {
        ++remainingBricks;
        return; // never need to load the next level
    }

    if (remainingBricks == 0) {
        loadNextLevel();
    }
}

function nearbyBricks(brickRect) {
    var result = new Array;

    // coordinates of the center of the brick
    var x = (brickRect.left + brickRect.right)/2;
    var y = (brickRect.top + brickRect.bottom)/2;
    var width = (brickRect.right - brickRect.left);
    var height = (brickRect.bottom - brickRect.top);

    // points to the left, right, top and bottom of the brick
    var points = new Array;
    points.push([x-width, y]);
    points.push([x+width, y]);
    points.push([x, y-height]);
    points.push([x, y+height]);

    for (var j in points) {
        var px = points[j][0];
        var py = points[j][1];
        var b = bgOverlay.childAt(px, py);
        if (b != null && bricks.indexOf(b)>=0) result.push(b);
    }
    return result;
}

function executeGift(type) {
    score += Globals.GIFT_SCORE;
    
    if (type == "Gift100Points") {
        score += 100 - Globals.GIFT_SCORE;
    } 
    else if (type == "Gift200Points") {
        score += 200 - Globals.GIFT_SCORE;
    } 
    else if (type == "Gift500Points") {
        score += 500 - Globals.GIFT_SCORE;
    } 
    else if (type == "Gift1000Points") {
        score += 1000 - Globals.GIFT_SCORE;
    } 
    else if (type == "GiftAddLife") {
        if (lives < Globals.MAXIMUM_LIVES) {
            lives++;
        }
    } 
    else if (type == "GiftLoseLife") {
        handleDeath();
    } 
    else if (type == "GiftNextLevel") {
        loadNextLevel();
    }
    else if (type == "GiftMagicEye") {
        giftMagicEye();
    }
    else if (type == "GiftMagicWand") {
        giftMagicWand();
    }
    else if (type == "GiftSplitBall") {
        giftSplitBall();
    }
    else if (type == "GiftAddBall") {
        createBall();
    } 
    else if (type == "GiftUnstoppableBall") {
        giftUnstoppableBall();
    }
    else if (type == "GiftBurningBall") {
        giftBurningBall();
    }
    else if (type == "GiftDecreaseSpeed") {
        changeSpeed(1.0/Globals.CHANGE_SPEED_RATIO);
    }
    else if (type == "GiftIncreaseSpeed") {
        changeSpeed(Globals.CHANGE_SPEED_RATIO);
    }
    else if (type == "GiftEnlargeBar") {
        bar.enlarge();
    }
    else if (type == "GiftShrinkBar") {
        bar.shrink();
    }
    else if (type == "GiftStickyBar") {
        bar.type = "StickyBar";
    }
    else if (type == "GiftMoreExplosion") {
        giftMoreExplosion();
    }
    else {
        print("Unrecognized gift type!!!", type);
    }
}

function giftMagicEye() {
    // make all hidden bricks visible
    for (var i in bricks) {
        var brick = bricks[i];
        if (!brick.visible) {
            brick.visible = true;
            ++remainingBricks;
        }
    }
}

function giftMagicWand() {
    for (var i in bricks) {
        var brick = bricks[i];

        // make unbreakable bricks breakable
        if (brick.type == "UnbreakableBrick") {
            brick.type = "BreakableBrick";
            ++remainingBricks;
        }

        // make multiple bricks single
        if (brick.type == "MultipleBrick3") {
            brick.type = "MultipleBrick1";
            score += Globals.AUTO_BRICK_SCORE * 2;
        } else if (brick.type == "MultipleBrick2") {
            brick.type = "MultipleBrick1";
            score += Globals.AUTO_BRICK_SCORE;
        }
    }
}

function giftSplitBall() {
    var newBalls = new Array;
    for (var i in balls) {
        var ball = balls[i];
        var newBall = ballComponent.createObject(bgOverlay);

        // give it a nice direction
        newBall.directionX = ball.directionX;
        newBall.directionY = ball.directionY;
        if (ball.directionY > 0)
            newBall.directionY *= -1;
        else
            newBall.directionX *= -1;

        newBall.toBeFired = ball.toBeFired;
        newBall.spriteKey = ball.spriteKey;
        newBall.posX = ball.posX;
        newBall.posY = ball.posY;
        newBalls.push(newBall);
    }
    balls = balls.concat(newBalls);
}

function giftUnstoppableBall() {
    for (var i in balls) {
        var ball = balls[i];
        if (ball.type == "BurningBall") {
            ball.type = "UnstoppableBurningBall";
        } else if (ball.type != "UnstoppableBurningBall") {
            ball.type = "UnstoppableBall";
        }
    }
}

function giftBurningBall() {
    for (var i in balls) {
        var ball = balls[i];
        if (ball.type == "UnstoppableBall") {
            ball.type = "UnstoppableBurningBall";
        } else if (ball.type != "UnstoppableBurningBall") {
            ball.type = "BurningBall";
        }
    }
}

function giftMoreExplosion() {
    var explodingBricks = new Array;
    for (var i in bricks) {
        var brick = bricks[i];
        if (brick.type == "ExplodingBrick") {
            explodingBricks.push(brick);
        }
    }

    for (var i in explodingBricks) {
        var nearby = nearbyBricks(createRect(explodingBricks[i]));
        for (var j in nearby) {
            var nearbyBrick = nearby[j];
            if (nearbyBrick.type == "UnbreakableBrick") {
                ++remainingBricks;
            }
            if (nearbyBrick.type == "HiddenBrick" && !nearbyBrick.visible) {
                nearbyBrick.visible = true;
                ++remainingBricks;
            }

            nearbyBrick.type = "ExplodingBrick";
        }
    }
}
