#include <cmath>

#include <QTimer>

#include <KConfig>
#include <KStandardDirs>
#include <kconfiggroup.h>
#include <kdebug.h>
#include "gameengine.h"

GameEngine::GameEngine()
{
    gameTimer.setInterval(UPDATE_INTERVAL);
    connect(&gameTimer, SIGNAL(timeout()), SLOT(step()));
    
    elapsedTimeTimer.setInterval(1000);
    connect(&elapsedTimeTimer, SIGNAL(timeout()), SLOT(increaseElapsedTime()));
}

GameEngine::~GameEngine()
{
    deleteAllObjects();
}

void GameEngine::start(QString l)
{
    levelSet = l;
    for (int i = 0; i < INITIAL_LIVES; ++i){
        m_lives.append(new Life);
    }
    level = 1;
    elapsedTime = 0;
    setScore(0);
    
    loadLevel();
    qsrand(666);
    
    elapsedTimeTimer.start();
    gameTimer.start();
}

void GameEngine::pause()
{
    elapsedTimeTimer.stop();
    gameTimer.stop();
}

void GameEngine::resume()
{
    elapsedTimeTimer.start();
    
    // only restart the gameTimer if there are objects moving
    bool movingObjects = false;
    foreach(Ball *ball, m_balls) {
        if (!ball->toBeFired) {
            movingObjects = true;
            continue;
        }
    }
    if (movingObjects) gameTimer.start();
}

void GameEngine::togglePause()
{
    if (elapsedTimeTimer.isActive()) pause();
    else resume();
}

void GameEngine::moveBar(int newPos) 
{
    // width of the game
    int width = BRICK_WIDTH * WIDTH;
    // width of the bar
    int w = m_bar.getRect().width();
    int y = m_bar.getRect().y();
    int x = newPos - w/2;
    
    if (x < 0) x = 0;
    else if (x > width - w) x = width - w;
    
    m_bar.moveTo(x, y);
    
    // move attached balls if needed
    foreach(Ball *ball, m_balls) {
        if (!ball->toBeFired) continue;
        // else
        int ballX = x + qRound(ball->barPosition * w);
        ball->moveTo(ballX, y - BALL_SIZE);
    }
    
    // TODO: maybe do some collision detection...
}

void GameEngine::moveBarLeft()
{
    moveBar(m_bar.getRect().left() + m_bar.getRect().width()/2 - BAR_MOVEMENT);
}

void GameEngine::moveBarRight()
{
    moveBar(m_bar.getRect().left() + m_bar.getRect().width()/2 + BAR_MOVEMENT);
}

void GameEngine::fire()
{
    foreach(Ball *ball, m_balls) {
        if (!ball->toBeFired) continue;
        // else
        ball->toBeFired = false;
        
        qreal ballCenter= ball->getRect().left() + (ball->getRect().width())/2;
        qreal barCenter = m_bar.getRect().left() + (m_bar.getRect().width())/2;
        qreal angle = M_PI / 3;
        angle *= (barCenter - ballCenter) / (m_bar.getRect().width()/2);
        angle += M_PI_2;
        
        ball->directionX =  cos(angle) * DEFAULT_SPEED;
        ball->directionY = -sin(angle) * DEFAULT_SPEED;
    }
    
    dScore = BRICK_SCORE;
    if (!gameTimer.isActive()) gameTimer.start();
}

// TODO: external level loarder?? (just an idea...)
void GameEngine::loadLevel()
{
    
    QString path = "levelsets/" + levelSet + ".desktop";
    path =  KStandardDirs::locate("appdata", path);
    KConfig file(path, KConfig::SimpleConfig);
    
    QString levelName("level" + QString::number(level));
    
    if (!file.hasGroup(levelName)) {
        if (level == 1) {
            kError() << "Invalid '" << levelName << "' in levelset " 
                     << levelSet << endl;
        } else {
            // TODO: shows the level max+1, is that ok??
            emit gameEnded(score, level, elapsedTime);
        }
        return;
    }
    
    KConfigGroup lvl = file.group(levelName);    
    int y = 1;
    QString key("line" + QString::number(y));
    
    deleteAllObjects();
    remainingBricks = 0;
    
    while(lvl.hasKey(key)) {
        // one line of bricks, to be converted
        QString line = lvl.readEntry(key.toAscii(), "error");
        if (line == "error") {
            kError() << "Something strange happened!!\n";
            return;
        }
        
        kDebug() << line << endl;
        
        if (line.size() > WIDTH) {
            kError() << "Invalid file: to many bricks\n";
        }
        
        // convert the string, each char represents a brick
        for (int x = 0; x < line.size(); ++x) {
            char type = line[x].toAscii();
            addBrick(type, x, y);
        }
        
        ++y;
        key = "line" + QString::number(y);
    }
    
    // add gifts
    
    // bricks whitout gifts
    QList<Brick *> bricksLeft = m_bricks;
    QMutableListIterator<Brick *> i(bricksLeft);
    while (i.hasNext()) {
        Brick *brick = i.next();
        if (brick->type() == "UnbreakableBrick")
            i.remove();
    }
    
    key = "GiftAddLife";
    for (int i = 0; i < giftTypesCount; ++i) {
        key = giftTypes[i];
        if (!lvl.hasKey(key)) continue;
        
        QString line = lvl.readEntry(key.toAscii(), "error");
        if (line == "error") {
            kError() << "Impossible Reading " << level << ":" << key << endl;
            return;
        }
        bool ok;
        int n = line.toInt(&ok);
        if (!ok) {
            kError() << levelName << ":" << key << " invalid number!!" << endl;
            continue;
        }
        if (bricksLeft.count() < n) {
            kError() << levelName << ": to many gifts!! " << endl;
            break;
        }
        for (int i = 0; i < n; ++i) {
            Gift *gift = new Gift;
            gift->setType(key);
            gift->hide();
            
            int index = qrand() % bricksLeft.count();
            bricksLeft.at(index)->gift = gift;
            bricksLeft.removeAt(index);
        }
    }
    
    m_balls.append(new Ball);
    moveBar(m_bar.getRect().x() + m_bar.getRect().width()/2);
    m_bar.reset();
}

void GameEngine::step()
{
    // needed to exit from the loop if the arrays that they cicle
    // change (items get deleted)    
    itemsGotDeleted = false;
    
    dScore *= 0.998;
    foreach (Ball *ball, m_balls) {
        if (ball->toBeFired) continue;
        
        ball->moveBy(ball->directionX, ball->directionY);
        // collision detection
        detectBallCollisions(ball);
        if (itemsGotDeleted) return;
    }
    
    QMutableListIterator<Gift *> i(m_gifts);
    while (i.hasNext()) {
        Gift *gift = i.next();
        if (!gift->isVisible()) continue;
        
        gift->moveBy(0, GIFT_SPEED);
        if (gift->getRect().bottom() > BRICK_HEIGHT * HEIGHT) {
            i.remove();
            delete gift;
        }
        else if (m_bar.getRect().intersects(gift->getRect())) {
            handleGift(gift);
            if (itemsGotDeleted) return;
            i.remove();
            delete gift;
        }
    }
}

void GameEngine::detectBallCollisions(Ball *ball)
{
    // never run this function more than two time recursively
    static bool firstTime = true;
    QRect rect = ball->getRect();
    
    // bounce a little early in some cases so the avarage position is centered
    rect.translate(qRound(ball->directionX/2), qRound(ball->directionY/2));
    
    // bounce against the wall
    if (rect.left() < 0 && ball->directionX < 0)
        ball->directionX *= -1;
    else if (rect.right() + 1 > BRICK_WIDTH * WIDTH
             && ball->directionX > 0)
        ball->directionX *= -1;
    else if (rect.top() < 0 && ball->directionY < 0)
        ball->directionY *= -1;
    else if (rect.bottom()+1 > ( HEIGHT) * BRICK_HEIGHT 
             && ball->directionY > 0) {
        // delete a ball
        m_balls.removeAll(ball);
        delete ball;
        itemsGotDeleted = true;
        // TODO: go on moving the other balls
        if (m_balls.isEmpty()) {
            addScore(LOSE_LIFE_SCORE);
        
            QTimer::singleShot(1000, this, SLOT(handleDeath()));
            gameTimer.stop();
        
            deleteMovingObjects();
        }
        return;
    }
    // bounce against the bar
    else if (m_bar.getRect().intersects(rect) && ball->directionY > 0) {
        qreal speed = sqrt(pow(ball->directionX, 2)+pow(ball->directionY, 2));
        
        qreal ballCenter = ball->getRect().left() + (ball->getRect().width()) / 2;
        qreal barCenter  = m_bar.getRect().left() + (m_bar.getRect().width()) / 2;
        qreal angle = M_PI / 3;
        angle *= (barCenter - ballCenter) / (m_bar.getRect().width()/2);
        angle += M_PI_2;
        
        if (angle > 0 && angle < M_PI) {
            ball->directionX =  cos(angle) * speed;
            ball->directionY = -sin(angle) * speed;
        }
    }
    // bounce against the bricks (and optionally break them)
    else {
        handleBrickCollisions(ball);
    }
    
    // never run this function more than two time recursively
    if (firstTime) {
        // add some randomness to the mix...
        // (the ball will slowly go faster...)
        ball->directionX *= 1 + 0.01 * (qrand() % 256 == 0);
        ball->directionY *= 1 + 0.01 * (qrand() % 256 == 0);
        if (ball->directionX > BRICK_HEIGHT)
            ball->directionX = BRICK_HEIGHT;
        if (ball->directionY > BRICK_HEIGHT)
            ball->directionY = BRICK_HEIGHT;
        firstTime = false;
        // check if there is another collision
        detectBallCollisions(ball);
    } else {
        firstTime = true;
        return;
    }
}

void GameEngine::addBrick(char type, int x, int y) 
{
    if (type == '-') return;
    
    ++remainingBricks;
    Brick *brick = new Brick;
    brick->moveTo(x*BRICK_WIDTH, (y-1)*BRICK_HEIGHT);
    
    switch (type) {
    case '1': brick->setType("PlainBrick1"); break;
    case '2': brick->setType("PlainBrick2"); break;
    case '3': brick->setType("PlainBrick3"); break;
    case '4': brick->setType("PlainBrick4"); break;
    case '5': brick->setType("PlainBrick5"); break;
    case '6': brick->setType("PlainBrick6"); break;
    case 'u': 
        brick->setType("UnbreakableBrick");
        --remainingBricks;
        break;
    case 'h': 
        brick->setType("HiddenBrick");
        brick->hide();
        --remainingBricks;
        break;
    case 'm': 
    {
        brick->setType("MultipleBrick3");        
        break;
    }
    default:
        kError() << "Invalid File: unknown character '" 
                    << type << "'\n";
        brick->setType("PlainBrick1");
    }
    m_bricks.append(brick);
}


void GameEngine::handleDeath()
{
    deleteMovingObjects();
    m_bar.reset();
    if (m_lives.isEmpty()) {
        emit gameEnded(score, level, elapsedTime);
    } else {
        delete m_lives.takeLast();
        Ball *ball = new Ball;
        m_balls.append(ball);
        moveBar(m_bar.getRect().x() + m_bar.getRect().width()/2);
    }
}


void GameEngine::handleBrickCollisions(Ball *ball)
{
    // TODO: rect->left() and -> bottom() may be one pixel wrong
    QRect rect = ball->getRect();

    QMutableListIterator<Brick *> i(m_bricks);
    while (i.hasNext()) {
        Brick *brick = i.next();
        QRect brickRect = brick->getRect();
        if (!brickRect.intersects(rect)) continue;
        // else: the ball has hit the brick
        
        if (ball->type() != "UnstoppableBall"
                && ball->type() != "UstoppableBurningBall") {
            // find out in which direction to bounce
            int top = brickRect.top() - rect.top();
            int bottom = rect.bottom() - brickRect.bottom();
            int left = brickRect.left() - rect.left();
            int right = rect.right() - brickRect.right();
            int max = qMax(qMax(top, bottom), qMax(left, right));
            
            // bounce
            // TODO: check this stuff
            if (max == top && ball->directionY > 0)
                ball->directionY *= -1;
            else if (max == bottom && ball->directionY < 0)
                ball->directionY *= -1;
            else if (max == left && ball->directionX > 0)
                ball->directionX *= -1;
            else if (max == right && ball->directionX < 0)
                ball->directionX *= -1;
            else
                break;
            
            if (brick->type() == "HiddenBrick" && !brick->isVisible()) {
                brick->show();
                ++remainingBricks;
                return;
            }
            if (brick->type() == "UnbreakableBrick") return;
            if (brick->type() == "MultipleBrick3") {
                brick->setType("MultipleBrick2");
                addScore(qRound(dScore));
                dScore = BRICK_SCORE;
                return;
            }
            if (brick->type() == "MultipleBrick2") {
                brick->setType("MultipleBrick1");
                addScore(qRound(dScore));
                dScore = BRICK_SCORE;
                return;
            }
        }
        // delete the brick
        i.remove();
        
        // FIXME: fix!!!!!!!
        if (not (brick->type() == "HiddenBrick" && not brick->isVisible()
            || brick->type() == "UnbreakableBrick"))
                --remainingBricks;
        
        if (brick->gift != 0) {
            brick->gift->moveTo(brick->getRect().left(), 
                                brick->getRect().top());
            brick->gift->show();
            m_gifts.append(brick->gift);
        }
        
        delete brick;
        
        addScore(qRound(dScore));
        dScore = BRICK_SCORE;
        if (remainingBricks == 0) {
            loadNextLevel();
            return;
        }
    }
}

void GameEngine::handleGift(Gift *gift)
{
    // TODO: put in a virtual function of gift (???)
    addScore(GIFT_SCORE);
    
    if (gift->type() == "Gift100Points") {
        addScore(100 - GIFT_SCORE);
    } 
    else if (gift->type() == "Gift200Points") {
        addScore(200 - GIFT_SCORE);
    } 
    else if (gift->type() == "Gift500Points") {
        addScore(500 - GIFT_SCORE);
    } 
    else if (gift->type() == "Gift1000Points") {
        addScore(1000 - GIFT_SCORE);
    } 
    else if (gift->type() == "GiftAddLife") {
        if (m_lives.count() < MAX_LIVES) m_lives.append(new Life);
    } 
    else if (gift->type() == "GiftLoseLife") {
        handleDeath();
    } 
    else if (gift->type() == "GiftNextLevel") {
        // assign points for each remaining brick
        foreach (Brick *brick, m_bricks) {
            // don't assign points for Unbreakable Bricks
            if (brick->type() == "UnbreakableBrick") continue;
            
            addScore(AUTOBRICK_SCORE);
            
            // add extra points for Multiple Bricks
            if (brick->type() == "MultipleBrick3")
                addScore(AUTOBRICK_SCORE*2);
            if (brick->type() == "MultipleBrick2")
                addScore(AUTOBRICK_SCORE);
        }
        loadNextLevel();
    } 
    else if (gift->type() == "GiftMagicEye") {
        // make all hidden bricks visible
        foreach (Brick *brick, m_bricks) {
            if (!brick->isVisible()) {
                brick->show();
                ++remainingBricks;
            }
        }
    }
    else if (gift->type() == "GiftMagicWand") {
        foreach (Brick *brick, m_bricks) {
            // make Unbreakbable Bricks Breakable
            if (brick->type() == "UnbreakableBrick") {
                brick->setType("BreakableBrick");
                ++remainingBricks;
            }
            
            // Make Multiple Bricks single
            if (brick->type() == "MultipleBrick3") {
                brick->setType("MultipleBrick1");
                addScore(AUTOBRICK_SCORE * 2);
            }
            if (brick->type() == "MultipleBrick2") {
                brick->setType("MultipleBrick1");
                addScore(AUTOBRICK_SCORE);
            }
        }
        // TODO: make multiple bricks single
    }
    else if (gift->type() == "GiftSplitBall") {
        // TODO: better copy (type, speed, etc...)
        QList<Ball *> newBalls;
        foreach(Ball *ball, m_balls) {
            Ball *newBall = new Ball;
            // give it a nice direction...
            newBall->directionX = ball->directionX;
            newBall->directionY = ball->directionY;
            if (ball->directionY > 0)
                newBall->directionY *= -1;
            else
                newBall->directionX *= -1;
          
            newBall->toBeFired = false;
            newBall->moveTo(ball->pos());
            newBalls.append(newBall);
        }
        m_balls += newBalls;
    }
    else if (gift->type() == "GiftAddBall") {
        m_balls.append(new Ball);
    } 
    else if (gift->type() == "GiftUnstoppableBall") {
        foreach(Ball *ball, m_balls)
            ball->setType("UnstoppableBall");
    }
    else if (gift->type() == "GiftDecreaseBallSpeed") {
        foreach(Ball *ball, m_balls)
            ball->decreaseSpeed();
    }
    else if (gift->type() == "GiftIncreaseBallSpeed") {
        foreach(Ball *ball, m_balls)
            ball->increaseSpeed();
    }
    else if (gift->type() == "GiftEnlargeBar") {
        m_bar.enlarge();
    }
    else if (gift->type() == "GiftShrinkBar") {
        m_bar.shrink();
    }
    else {
        kError() << "Unrecognized gift!!!\n";
    }
}

//======= convenience functions =================//
void GameEngine::loadNextLevel()
{
    ++level;
    deleteMovingObjects();
    QTimer::singleShot(200, this, SLOT(loadLevel()));
    addScore(LEVEL_SCORE);
}

inline void GameEngine::addScore(int points)
{
    score += points;
    scoreCanvas.setScore(score);
}

inline void GameEngine::setScore(int newScore)
{
    score = newScore;
    scoreCanvas.setScore(score);
}

// TODO: check (in debugger?) why this function is called so much...
inline void GameEngine::deleteMovingObjects()
{
    kDebug() << "Deleting objects...\n";
    itemsGotDeleted = true;
    while (!m_balls.isEmpty()) delete m_balls.takeFirst();
    
    QMutableListIterator<Gift *> i(m_gifts);
    while (i.hasNext()) {
        Gift *gift = i.next(); 
        if (gift->isVisible())
            delete gift;
            i.remove();
    }
}

inline void GameEngine::deleteAllObjects()
{
    deleteMovingObjects();
    while (!m_bricks.isEmpty()) delete m_bricks.takeFirst();
    while (!m_gifts.isEmpty()) delete m_gifts.takeFirst();
}

#include "gameengine.moc"
