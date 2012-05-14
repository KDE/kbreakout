#include "canvaswidget.h"
#include "svgitem.h"
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

    setResizeMode(QDeclarativeView::SizeRootObjectToView);
    QString path = KStandardDirs::locate("appdata", "qml/main.qml");
    setSource(QUrl::fromLocalFile(path));

    connect(m_renderer->themeProvider(), SIGNAL(currentThemeChanged(const KgTheme*)), rootObject(), SLOT(reloadSprites()));
}
