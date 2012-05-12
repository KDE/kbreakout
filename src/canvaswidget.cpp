#include "canvaswidget.h"
#include "globals.h"
#include "settings.h"

#include <KStandardDirs>

#include <QKeyEvent>
#include <QCursor>
#include <QUrl>

#include <KDebug>

CanvasWidget::CanvasWidget(QWidget *parent) 
    : QDeclarativeView(parent)
{
    QString path = KStandardDirs::locate("appdata", "qml/main.qml");
    setResizeMode(QDeclarativeView::SizeRootObjectToView);
    setSource(QUrl::fromLocalFile(path));
}
