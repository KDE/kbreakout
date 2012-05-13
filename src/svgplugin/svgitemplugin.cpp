#include "svgitemplugin.h"
#include "svgitem.h"

#include <QDeclarativeImageProvider>

void SvgItemPlugin::registerTypes(const char *uri)
{
    Q_UNUSED(uri);
}

void SvgItemPlugin::initializeEngine(QDeclarativeEngine *engine, const char *uri)
{
    Q_UNUSED(uri);
    engine->addImageProvider("svgitem", new SvgItem);
}
