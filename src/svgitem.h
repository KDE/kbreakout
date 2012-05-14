#ifndef SVGPLUGIN_H
#define SVGPLUGIN_H

#include <QDeclarativeImageProvider>
#include <KGameRenderer>

class SvgItem : public QDeclarativeImageProvider
{
public:
    SvgItem(KGameRenderer *renderer);

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

private:
    KGameRenderer *m_renderer;
};

#endif
