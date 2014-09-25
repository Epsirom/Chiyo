#include "chiyographics.h"

#include <qDebug>

ChiyoGraphics::ChiyoGraphics(QWidget *parent) :
    QGraphicsView(parent)
{
    this->setBackgroundBrush(QBrush(QColor(100, 100, 100)));
    QGraphicsScene * scn = new QGraphicsScene(this);
    this->setScene(scn);
    QPixmap pix(":/default_image");
    setImage(pix);

    this->grabGesture(Qt::PinchGesture);

}

void ChiyoGraphics::setImage(QPixmap &img)
{
    scene()->clear();
    scene()->addPixmap(img);
    scene()->setSceneRect(img.rect());
}

void ChiyoGraphics::setScale(qreal sx, qreal sy)
{
    QTransform t = transform();
    setTransform(QTransform(sx, t.m12(), t.m13(), t.m21(), sy, t.m23(), t.m31(), t.m32(), t.m33()));
}

bool ChiyoGraphics::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
    {
        QGestureEvent * gevent = static_cast<QGestureEvent*>(event);
        if (QGesture *pinch = gevent->gesture(Qt::PinchGesture))
        {
            gevent->accept();
            pinchTriggered(static_cast<QPinchGesture *>(pinch));
        }
    }
    return QGraphicsView::event(event);
}

bool ChiyoGraphics::pinchTriggered(QPinchGesture *g)
{
    if (g->scaleFactor() == 1)
    {
        return false;
    }
    scale(g->scaleFactor(), g->scaleFactor());
    g->setScaleFactor(1.);
    return true;
}
