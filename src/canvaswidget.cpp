#include "canvaswidget.h"
#include "svgitem.h"
#include "globals.h"
#include "settings.h"

#include <KStandardDirs>

#include <QDeclarativeEngine>
#include <QKeyEvent>
#include <QCursor>
#include <QUrl>

#include <KDebug>

CanvasWidget::CanvasWidget(QWidget *parent) 
    : QDeclarativeView(parent)
{
    engine()->addImageProvider("svgitem", new SvgItem);

    QString path = KStandardDirs::locate("appdata", "qml/main.qml");
    setResizeMode(QDeclarativeView::SizeRootObjectToView);
    setSource(QUrl::fromLocalFile(path));
}
