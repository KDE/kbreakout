#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include <QDeclarativeView>
#include <KGameRenderer>

class CanvasWidget : public QDeclarativeView
{
    Q_OBJECT
public:
    CanvasWidget(KGameRenderer *renderer, QWidget *parent=0);

signals:

public slots:
    void fire();

private slots:
    void showLine(QString line, int lineNumber);
    void putGift(QString gift, int times, QString pos);
    void updateBarDirection();
    void startGame();

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    KGameRenderer *m_renderer;

    //used when moving the bar with the keys
    int m_barDirection;

    // used to track which direction keys are pressed between key events
    bool m_rightPressed;
    bool m_leftPressed;

};

#endif //CANVASWIDGET_H
