#ifndef SVGPLUGIN_H
#define SVGPLUGIN_H

#include <QDeclarativeImageProvider>
#include <KGameRenderer>

class SvgItem : public QDeclarativeImageProvider
{
public:
    SvgItem();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

private:
    KGameRenderer *renderer;
};

#endif
