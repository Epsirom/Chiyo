#ifndef LIQUEFIEDGRAPHICSVIEW_H
#define LIQUEFIEDGRAPHICSVIEW_H

#include <QGraphicsView>
#include <opencv2/opencv.hpp>
#include "chiyoimagebase.h"

class LiquefiedGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit LiquefiedGraphicsView(QWidget *parent = 0);
    void setImage(cv::Mat mat);

    cv::Mat getCurrent();

    void liquefy(QPointF c, QPointF m);

    void syncImage();

    void setRadius(int r);

protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void leaveEvent(QEvent* e);
    void enterEvent(QEvent* e);

signals:

public slots:

private:
    cv::Mat current;
    int radius;

    QPointF last_pos;

    QGraphicsEllipseItem* circle;
    QGraphicsPixmapItem* pix;
};

#endif // LIQUEFIEDGRAPHICSVIEW_H
