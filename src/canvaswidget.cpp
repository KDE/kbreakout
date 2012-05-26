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

void CanvasWidget::showLine(int lineNumber, QString line)
{
    QMetaObject::invokeMethod(rootObject(), "loadLine",
                              Q_ARG(QVariant, lineNumber),
                              Q_ARG(QVariant, line));
}

