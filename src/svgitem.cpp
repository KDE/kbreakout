#include <QDeclarativeImageProvider>
#include <QDeclarativeEngine>
#include <QDeclarativeExtensionPlugin>
#include <KGameRenderer>
#include <KgThemeProvider>

class SvgItem : public QDeclarativeImageProvider
{
public:
    SvgItem() : QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap)
    {
        KgThemeProvider *prov = new KgThemeProvider;
        prov->discoverThemes("appdata", QLatin1String("themes"));
        renderer = new KGameRenderer(prov);
    }

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
    {
        return renderer->spritePixmap(id, *size);
    }

private:
    KGameRenderer *renderer;
};

class SvgItemPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT
public:
    void registerTypes(const char *uri)
    {
        Q_UNUSED(uri);
    }

    void initializeEngine(QDeclarativeEngine *engine, const char *uri)
    {
        Q_UNUSED(uri);
        engine->addImageProvider("svgitem", new SvgItem);
    }
};

#include "svgitem.moc"

Q_EXPORT_PLUGIN(SvgItemPlugin);
