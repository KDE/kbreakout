#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include <QDeclarativeView>
#include <KGameRenderer>

class CanvasWidget : public QDeclarativeView
{
    Q_OBJECT
public:
    CanvasWidget(KGameRenderer *renderer, QWidget *parent=0);

signals:

public slots:

private slots:

private:
    KGameRenderer *m_renderer;

};

#endif //CANVASWIDGET_H
