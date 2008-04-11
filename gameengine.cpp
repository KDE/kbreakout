#include <cmath>

#include <QTimer>

#include <KConfig>
#include <KStandardDirs>
#include <kconfiggroup.h>
#include <kdebug.h>

#include "gameengine.h"
#include "gift.h"

GameEngine::GameEngine()
    : updateInterval(DEFAULT_UPDATE_INTERVAL)
{
    gameTimer.setInterval(qRound(updateInterval));
    connect(&gameTimer, SIGNAL(timeout()), SLOT(step()));
    
    repaintTimer.setInterval(REPAINT_INTERVAL);
    connect(&repaintTimer, SIGNAL(timeout()), SLOT(repaintMovingObjects()));
    
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
    repaintTimer.start();
}

bool GameEngine::gameIsPaused()
{
    return !elapsedTimeTimer.isActive();
}

void GameEngine::pause()
{
    if (gameIsPaused()) return;
    
    elapsedTimeTimer.stop();
    gameTimer.stop();
    repaintTimer.stop();
    emit gamePaused();
}

void GameEngine::resume()
{
    if (!gameIsPaused()) return;
    
    elapsedTimeTimer.start();
    repaintTimer.start();
    
    // only restart the gameTimer if there are objects moving
    bool movingObjects = false;
    foreach(Ball *ball, m_balls) {
        if (!ball->toBeFired) {
            movingObjects = true;
            continue;
        }
    }
    if (movingObjects) {
        gameTimer.start();
    }
    
    int barPosition = m_bar.position().x() + m_bar.getRect().width()/2;
    emit gameResumed(barPosition);
}

void GameEngine::togglePause()
{
    if (gameIsPaused()) {
        resume();
    } else {
        pause();
    }
}

void GameEngine::moveBar(int newPos) 
{
    if (gameIsPaused()) return;
    
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
    if (gameIsPaused()) {
        resume();
        return;
    }

    foreach(Ball *ball, m_balls) {
        if (!ball->toBeFired) continue;
        // else
        ball->toBeFired = false;
        
        qreal ballCenter= ball->getRect().left() + (ball->getRect().width())/2;
        qreal barCenter = m_bar.getRect().left() + (m_bar.getRect().width())/2;
        qreal angle = M_PI / 3;
        angle *= (barCenter - ballCenter) / (m_bar.getRect().width()/2);
        angle += M_PI_2;
        
        ball->directionX =  cos(angle) * BALL_SPEED;
        ball->directionY = -sin(angle) * BALL_SPEED;
    }
    
    dScore = BRICK_SCORE;
    if (!gameTimer.isActive()) gameTimer.start();
}

// TODO: external level loarder???
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
        QString line = lvl.readEntry(key, "error");
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
    
    for (int i = 0; i < giftTypesCount; ++i) {
        key = giftTypes[i];
        if (!lvl.hasKey(key)) continue;
        
        QString line = lvl.readEntry(key, "error");
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
            Gift *gift = Gift::newGift(key); // key is the gift type
            gift->hide();
            
            int index = qrand() % bricksLeft.count();
            bricksLeft.at(index)->gift = gift;
            bricksLeft.removeAt(index);
        }
    }
    
    m_balls.append(new Ball);
    moveBar(m_bar.getRect().x() + m_bar.getRect().width()/2);
    m_bar.reset();
    setUpdateInterval(DEFAULT_UPDATE_INTERVAL);
}

void GameEngine::step()
{
    // needed to exit from the loop if the arrays that they cicle
    // change (items get deleted)    
    itemsGotDeleted = false;
    
    // TODO: don't use magick numbers
    dScore *= SCORE_AUTO_DECREASE;
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
        
        gift->move();
        if (gift->getRect().bottom() > BRICK_HEIGHT * HEIGHT) {
            i.remove();
            delete gift;
        }
        else if (m_bar.getRect().intersects(gift->getRect())) {
            gift->execute(this);
            if (itemsGotDeleted) return;
            i.remove();
            delete gift;
        }
    }
}

void GameEngine::repaintMovingObjects()
{
    m_bar.repaint();
    
    foreach (Ball *ball, m_balls) {
        ball->repaint();
    }
    
    foreach (Gift *gift, m_gifts) {
        if (!gift->isVisible()) continue;
        
        gift->repaint();
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
    if (rect.left() < 0 && ball->directionX < 0) {
        ball->directionX *= -1;
    } else if (rect.right() + 1 > BRICK_WIDTH * WIDTH
             && ball->directionX > 0) {
        ball->directionX *= -1;
    } else if (rect.top() < 0 && ball->directionY < 0) {
        ball->directionY *= -1;
    } else if (rect.bottom()+1 > ( HEIGHT) * BRICK_HEIGHT 
             && ball->directionY > 0) {
        // delete the ball
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
        if (qrand() % 512 == 0) {
            if (qrand() % 2) {
                ball->directionX += 0.01;
            } else {
                ball->directionY += 0.01;
            }
            
            // increase the speed a little
            setUpdateInterval(updateInterval*UPDATE_INTERVAL_DECREASE);
        }
        
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
        setUpdateInterval(DEFAULT_UPDATE_INTERVAL);
    }
}


void GameEngine::handleBrickCollisions(Ball *ball)
{
    // TODO: rect->left() and -> bottom() may be one pixel wrong
    QRect rect = ball->getRect();

    QSet<Brick *> bricksMarkedForDeletion;
    foreach(Brick *brick, m_bricks) {
        QRect brickRect = brick->getRect();
        if (!brickRect.intersects(rect)) continue;
        // else: the ball has hit the brick
        
        if (ball->type() != "UnstoppableBall"
                && ball->type() != "UnstoppableBurningBall") {
            // find out in which direction to bounce
            int top = brickRect.top() - rect.top();
            int bottom = rect.bottom() - brickRect.bottom();
            int left = brickRect.left() - rect.left();
            int right = rect.right() - brickRect.right();
            int max = qMax(qMax(top, bottom), qMax(left, right));
            
            // bounce
            // TODO: check this stuff
            if (max == top && ball->directionY > 0) {
                ball->directionY *= -1;
            } else if (max == bottom && ball->directionY < 0) {
                ball->directionY *= -1;
            } else if (max == left && ball->directionX > 0) {
                ball->directionX *= -1;
            } else if (max == right && ball->directionX < 0) {
                ball->directionX *= -1;
            } else {
                break;
            }
        }
        
        if (ball->type() == "BurningBall" 
                || ball->type() == "UnstoppableBurningBall") {
            bricksMarkedForDeletion.insert(brick);
            bricksMarkedForDeletion.unite(nearbyBricks(brick));
        } else if (ball->type() == "UnstoppableBall") {
            // regardless of the type of ball
            bricksMarkedForDeletion.insert(brick);
        } else if (brick->type() == "HiddenBrick" && !brick->isVisible()) {
            brick->show();
            ++remainingBricks;
        } else if (brick->type() == "MultipleBrick3") {
            brick->setType("MultipleBrick2");
            addScore(qRound(dScore));
            dScore = BRICK_SCORE;
        } else if (brick->type() == "MultipleBrick2") {
            brick->setType("MultipleBrick1");
            addScore(qRound(dScore));
            dScore = BRICK_SCORE;
        } else if (brick->type() != "UnbreakableBrick") {
            bricksMarkedForDeletion.insert(brick);
        }
    }
    
    // remove the bricks from m_bricks
    QMutableListIterator<Brick *> i(m_bricks);
    while (i.hasNext()) {
        Brick *brick = i.next();
        if (bricksMarkedForDeletion.contains(brick)) {
            i.remove();
        }
    }
    
    // really remove them
    foreach(Brick *brick, bricksMarkedForDeletion) {
        deleteBrick(brick);
    }
}

QSet<Brick *> GameEngine::nearbyBricks(Brick *brick)
{
    
    QSet<Brick *> result;
    QRect brickRect = brick->getRect();
    // coordinates of the center of the brick
    int x = brickRect.x() + BRICK_WIDTH / 2;
    int y = brickRect.y() + BRICK_HEIGHT / 2; 
    
    // points to the left, right, top and bottom of the brick
    QList<QPoint> nearbyPoints;
    nearbyPoints.append(QPoint(x - BRICK_WIDTH, y));
    nearbyPoints.append(QPoint(x + BRICK_WIDTH, y));
    nearbyPoints.append(QPoint(x, y - BRICK_HEIGHT));
    nearbyPoints.append(QPoint(x, y + BRICK_HEIGHT));
    
    foreach(Brick *b, m_bricks) {
        foreach (QPoint p, nearbyPoints) {
            if (b->getRect().contains(p)) {
                result.insert(b);
            }
        }
    }
    
    return result;
}

void GameEngine::deleteBrick(Brick *brick)
{
    // these two kind of bricks aren't counted in the remainingBricks
    if (brick->type() == "HiddenBrick" && !brick->isVisible()) {
        ++remainingBricks;
    }
    if (brick->type() == "UnbreakableBrick") {
        ++remainingBricks;
    }

    if (brick->gift != 0) {
        brick->gift->moveTo(brick->getRect().left(), 
                            brick->getRect().top());
        brick->gift->show();
        m_gifts.append(brick->gift);
    }
    
    delete brick;
    --remainingBricks;
    
    addScore(qRound(dScore));
    dScore = BRICK_SCORE;
    
    if (remainingBricks == 0) {
        loadNextLevel();
        return;
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

void GameEngine::setUpdateInterval(qreal newUpdateInterval)
{
    if (newUpdateInterval > MAXIMUM_UPDATE_INTERVAL) {
        newUpdateInterval = MAXIMUM_UPDATE_INTERVAL;
    }
    if (newUpdateInterval < MINIMUM_UPDATE_INTERVAL) {
        newUpdateInterval = MINIMUM_UPDATE_INTERVAL;
    }
    updateInterval = newUpdateInterval;
    
    int roundedValue = qRound(updateInterval);
    if (roundedValue != gameTimer.interval()) {
        gameTimer.setInterval(roundedValue);
    } 
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
