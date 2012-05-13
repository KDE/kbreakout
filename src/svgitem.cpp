#include "svgitem.h"

#include <KgThemeProvider>

SvgItem::SvgItem() : QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap)
{
    KgThemeProvider *prov = new KgThemeProvider;
    prov->discoverThemes("appdata", QLatin1String("themes"));
    renderer = new KGameRenderer(prov);
}

QPixmap SvgItem::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    return renderer->spritePixmap(id, renderer->boundsOnSprite(id).size().toSize());
}
