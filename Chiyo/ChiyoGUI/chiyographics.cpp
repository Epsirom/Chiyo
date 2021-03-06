#include "chiyographics.h"

#include <qDebug>
#include <QGraphicsItem>

ChiyoGraphics::ChiyoGraphics(QWidget *parent) :
    QGraphicsView(parent)
{
    this->setBackgroundBrush(QBrush(QColor(100, 100, 100)));
    QGraphicsScene * scn = new QGraphicsScene(this);
    this->setScene(scn);

    //QPixmap pix(":/default_image");
    //setImage(pix);

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
    emit pinchZoom(g->scaleFactor());
    g->setScaleFactor(1.);
    return true;
}

qreal ChiyoGraphics::getFitScale()
{
    QRect myRect = rect();
    QGraphicsScene* myScene = scene();
    qreal wscale = qreal(myRect.width()) / myScene->width();
    qreal hscale = qreal(myRect.height()) / myScene->height();
    return wscale < hscale ? wscale : hscale;
}

QImage ChiyoGraphics::getImage()
{
    auto scn = scene();
    auto gitems = scn->items();
    QGraphicsPixmapItem *item = (QGraphicsPixmapItem*)gitems[0];
    return item->pixmap().toImage();

    /*
    scn->clearSelection();
    scn->setSceneRect(scn->itemsBoundingRect());

    QImage img(scn->sceneRect().size().toSize(), QImage::Format_RGB32);
    img.fill(QColor(255, 255, 255));

    QPainter painter(&img);
    scene()->render(&painter);
    return img;
    */
}
