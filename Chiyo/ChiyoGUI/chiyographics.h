#ifndef CHIYOGRAPHICS_H
#define CHIYOGRAPHICS_H

#include <QGesture>
#include <QGraphicsView>
#include <QImage>

class ChiyoGraphics : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ChiyoGraphics(QWidget *parent = 0);
    void setImage(QPixmap& img);
    void setScale(qreal sx, qreal sy);

    QImage getImage();

    qreal getFitScale();

protected:
    bool event(QEvent *event);
    bool pinchTriggered(QPinchGesture * g);

signals:
    void pinchZoom(qreal factor);

public slots:

};

#endif // CHIYOGRAPHICS_H
