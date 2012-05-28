#include "canvaswidget.h"
#include "canvasitem.h"
#include "globals.h"
#include "settings.h"

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
