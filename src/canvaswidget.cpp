#include "canvaswidget.h"
#include "canvasitem.h"
#include "globals.h"
#include "settings.h"

#include <QKeyEvent>

#include <KStandardDirs>

CanvasWidget::CanvasWidget(KGameRenderer *renderer, QWidget *parent) :
    QDeclarativeView(parent),
    m_renderer(renderer)
{
    CanvasItem::setRenderer(m_renderer);
    qmlRegisterType<CanvasItem>("SvgLibrary", 1, 0, "CanvasItem");

    setResizeMode(QDeclarativeView::SizeRootObjectToView);
    QString path = KStandardDirs::locate("appdata", "qml/main.qml");
    setSource(QUrl::fromLocalFile(path));
}

void CanvasWidget::showLine(QString line, int lineNumber)
{
    QMetaObject::invokeMethod(rootObject(), "loadLine",
                              Q_ARG(QVariant, line),
                              Q_ARG(QVariant, lineNumber));
}

void CanvasWidget::putGift(QString gift, int times, QString pos)
{
    QMetaObject::invokeMethod(rootObject(), "loadGift",
                              Q_ARG(QVariant, gift),
                              Q_ARG(QVariant, times),
                              Q_ARG(QVariant, pos));
}

void CanvasWidget::startGame()
{
    QMetaObject::invokeMethod(rootObject(), "startGame");
}

void CanvasWidget::fire()
{
    QMetaObject::invokeMethod(rootObject(), "fire");
}

void CanvasWidget::updateBarDirection()
{
    QMetaObject::invokeMethod(rootObject(), "updateBarDirection",
                              Q_ARG(QVariant, m_barDirection));
}

void CanvasWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        QDeclarativeView::keyPressEvent(event);
        return;
    }
    int key = event->key();
    switch (key) {
    case Qt::Key_Right:
        m_rightPressed = true;
        m_barDirection = 1;
        break;
    case Qt::Key_Left:
        m_leftPressed = true;
        m_barDirection = -1;
        break;
    default:
        QDeclarativeView::keyPressEvent(event);
        return;
    }

    updateBarDirection();
}

void CanvasWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        QDeclarativeView::keyReleaseEvent(event);
        return;
    }
    int key = event->key();
    switch (key) {
    case Qt::Key_Right:
        m_rightPressed = false;
        break;
    case Qt::Key_Left:
        m_leftPressed = false;
        break;
    default:
        QDeclarativeView::keyReleaseEvent(event);
        return;
    }

    if (!m_rightPressed && !m_leftPressed) {
        m_barDirection = 0;
    } else if (m_rightPressed && !m_leftPressed) {
        m_barDirection = 1;
    } else if (!m_rightPressed && m_leftPressed) {
        m_barDirection = -1;
    }

    updateBarDirection();
}
