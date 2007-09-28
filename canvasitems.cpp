#include <cmath>

#include <KDebug>

#include "canvasitems.h"

Ball::Ball()
{   
    width = BALL_SIZE;
    height = BALL_SIZE;
    elementId = "PlainBall";
    
    toBeFired = true;
    barPosition = 0.6;
    
    loadSprite();
}

void Ball::decreaseSpeed()
{
    directionX /= CHANGE_BALL_SPEED_RATIO;
    directionY /= CHANGE_BALL_SPEED_RATIO;
}

void Ball::increaseSpeed()
{
    // FIXME
}

Brick::Brick()
{
    width = BRICK_WIDTH;
    height = BRICK_HEIGHT;
    elementId = "Unset";
    
    gift = 0;
}

Gift::Gift()
{
    width = GIFT_WIDTH;
    height = GIFT_HEIGHT;
    elementId = "Unset";
}

Bar::Bar()
{
    width = DEFAULT_BAR_WIDTH;
    height = BRICK_HEIGHT;
    elementId = "PlainBar";
    
    moveTo(300, (HEIGHT-1)*BRICK_HEIGHT);

    //loadSprite();
}

void Bar::reset()
{
    int oldWidth = width;
    width = DEFAULT_BAR_WIDTH;
    loadSprite();
    
    // move the bar to retain it's center
    int d = (oldWidth - width)/2;
    
    moveBy(d, 0);
}

void Bar::enlarge()
{
    int oldWidth = width;
    width = qRound(width*RESIZE_BAR_RATIO);
    if (width > MAX_BAR_WIDTH) width = MAX_BAR_WIDTH;
    loadSprite();
    
    // move the bar to retain it's center
    int d = (oldWidth - width)/2;
    
    moveBy(d, 0);
}

void Bar::shrink()
{
    int oldWidth = width;
    kDebug() << "old width = " << width << endl;
    width = qRound(width/RESIZE_BAR_RATIO);
    if (width < MIN_BAR_WIDTH) width = MIN_BAR_WIDTH;
    loadSprite();
    
    // move the bar to retain it's center
    int d = (oldWidth - width)/2;
    
    moveBy(d, 0);
}

int Life::count = 0;

Life::Life()
{
    ++count;
    
    width = (BRICK_WIDTH)/2;
    height = (BRICK_HEIGHT)/2;
    elementId = "PlainBar";
    
    int x = -5 + qRound( BRICK_WIDTH * WIDTH 
                        - (0.6 * BRICK_WIDTH * count) );
    int y = qRound(-0.75*BRICK_HEIGHT);
     
    moveTo(x, y);
    
    loadSprite();
}

Life::~Life()
{
    --count;
}

Background::Background()
{
    int w = BRICK_WIDTH * WIDTH;
    int h = BRICK_HEIGHT * HEIGHT;
    //qreal border = BACKGROUND_BORDER * w;
    // BORDER_RATIO is to make it a little bit bigger...
    // TODO: not sure (souldn't it be 2??? but this seems to work better...)
    width = w + 1;
    height = h + 1;
    elementId = "BackgroundOverlay";
    moveTo(-1, -1);
        
    loadSprite();
}

Score::Score()
{
    height = BRICK_HEIGHT;
    width =  BRICK_WIDTH * (WIDTH/2);
    moveTo(BRICK_WIDTH/6, -BRICK_HEIGHT);
    
    elementId = "NoId";
    
    setScore(0);
}

void Score::loadSprite()
{
    updateScale();
    
    QPixmap pixmap(qRound(m_scale*width), qRound(m_scale*height));
    pixmap.fill(Qt::transparent);
    QPainter p(&pixmap);
    p.setPen(QColor(255,255,255,150));
    p.setFont(QFont("Arial", qRound(m_scale*height * 0.8)));
    p.drawText(QPoint(0, qRound(m_scale*height * 0.8)), scoreString);
    setPixmap(pixmap);
    
    updatePosition(); //TODO: needed?????
}

void Score::setScore(int newScore)
{
    scoreString = QString("%L1").arg(newScore);
    //scoreString = QString("%1").arg(0.01*newScore, 0, 'f', 2);
    loadSprite();
}
