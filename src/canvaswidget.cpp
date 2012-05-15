#include "canvaswidget.h"
#include "svgitem.h"
#include "themedimage.h"
#include "globals.h"
#include "settings.h"

#include <KStandardDirs>
#include <KgThemeProvider>

#include <QDeclarativeEngine>
#include <QGraphicsObject>

CanvasWidget::CanvasWidget(KGameRenderer *renderer, QWidget *parent) :
    QDeclarativeView(parent),
    m_renderer(renderer)
{
    engine()->addImageProvider("svgitem", new SvgItem(m_renderer));
    ThemedImage::setRenderer(m_renderer);
    qmlRegisterType<ThemedImage>("SvgLibrary", 1, 0, "ThemedImage");

    setResizeMode(QDeclarativeView::SizeRootObjectToView);
    QString path = KStandardDirs::locate("appdata", "qml/main.qml");
    setSource(QUrl::fromLocalFile(path));
}
