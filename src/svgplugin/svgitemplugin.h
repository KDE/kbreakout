#ifndef SVGITEMPLUGIN_H
#define SVGITEMPLUGIN_H

#include <QDeclarativeExtensionPlugin>
#include <QDeclarativeEngine>

class SvgItemPlugin : public QDeclarativeExtensionPlugin
{
public:
    void registerTypes(const char *uri);

    void initializeEngine(QDeclarativeEngine *engine, const char *uri);
};

Q_EXPORT_PLUGIN2(svgitemplugin, SvgItemPlugin);

#endif // SVGITEMPLUGIN_H
