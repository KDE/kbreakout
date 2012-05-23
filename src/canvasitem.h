#ifndef CANVASITEM_H
#define CANVASITEM_H

#include <QDeclarativeItem>
#include <KGameRenderer>

class CanvasItem : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(QString spriteKey READ spriteKey WRITE setSpriteKey NOTIFY spriteKeyChanged)
    Q_PROPERTY(bool valid READ isValid)

public:
    CanvasItem(QDeclarativeItem *parent = 0);

    static void setRenderer(KGameRenderer*);

    QString spriteKey() const;
    void setSpriteKey(const QString &spriteKey);

    bool isValid() const;

    void setImplicitSize();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget=0);

signals:
    void spriteKeyChanged();

private:
    QString m_key;
    static KGameRenderer *m_renderer;

};

#endif
