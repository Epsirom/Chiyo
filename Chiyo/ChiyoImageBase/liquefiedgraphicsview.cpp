#include "liquefiedgraphicsview.h"
#include <QMouseEvent>
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>

LiquefiedGraphicsView::LiquefiedGraphicsView(QWidget *parent) :
    QGraphicsView(parent),
    current(),
    radius(10)
{
    setMouseTracking(true);
    this->setBackgroundBrush(QBrush(QColor(100, 100, 100)));
    QGraphicsScene * scn = new QGraphicsScene(this);
    this->setScene(scn);
    pix = scn->addPixmap(QPixmap());
    circle = scn->addEllipse(0, 0, radius * 2, radius * 2);
    circle->hide();
}

void LiquefiedGraphicsView::setImage(cv::Mat mat)
{
    current = mat.clone();
    syncImage();
}

void LiquefiedGraphicsView::syncImage()
{
    QPixmap img = cvMatToQPixmap(current);
    pix->setPixmap(img);
    scene()->setSceneRect(img.rect());
}

void LiquefiedGraphicsView::setRadius(int r)
{
    if (r <= 0)
        return;
    radius = r;
    circle->setRect(circle->rect().x(), circle->rect().y(), r * 2, r * 2);
}

void LiquefiedGraphicsView::mousePressEvent(QMouseEvent *e)
{
    last_pos = mapToScene(e->pos());
}

void LiquefiedGraphicsView::mouseMoveEvent(QMouseEvent *e)
{
    QPointF new_pos = mapToScene(e->pos());
    if (e->buttons() & Qt::LeftButton)
    {
        liquefy(last_pos, new_pos);
        last_pos = new_pos;
    }
    circle->setPos(new_pos - QPointF(radius + 1, radius + 1));
}

void LiquefiedGraphicsView::leaveEvent(QEvent *e)
{
    circle->hide();
}

void LiquefiedGraphicsView::enterEvent(QEvent *e)
{
    circle->show();
}

cv::Mat LiquefiedGraphicsView::getCurrent()
{
    return current;
}

double pointSqr(QPointF p)
{
    return p.x() * p.x() + p.y() * p.y();
}

QPointF liquefy_get_u(QPointF c, QPointF m, QPointF x, int r2)
{
    double x_c_2 = pointSqr(x - c);
    QPointF m_c = m - c;
    return x - doubleSqr((r2 - x_c_2) / (r2 - x_c_2 + pointSqr(m_c))) * (m_c);
}

cv::Vec4b mat_at_pointf(cv::Mat mat, QPointF p)
{
    return mat.at<cv::Vec4b>(p.y(), p.x());
    /*
    double x = p.x(), y = p.y();
    int ix = int(x), iy = int(y);
    double dx = x - ix, dy = y - iy;
    auto & i1 = mat.at<cv::Vec4b>(iy, ix);
    auto & i2 = mat.at<cv::Vec4b>(iy + 1, ix);
    auto & i3 = mat.at<cv::Vec4b>(iy, ix + 1);
    auto & i4 = mat.at<cv::Vec4b>(iy + 1, ix + 1);
    cv::Vec4b result;
    for (int i = 0; i < 3; ++i)
    {
        result[i] = (1 - dy) * ((1 - dx) * i1[i] + dx * i3[i]) + dy * ((1 - dx) * i2[i] + dx * i4[i]);
    }
    result[3] = 255;
    return result;
    */
}

void LiquefiedGraphicsView::liquefy(QPointF c, QPointF m)
{
    if (c == m)
        return;
    cv::Mat src = current.clone();
    int r2 = intSqr(radius);
    for (int row = intMax(0, c.y() - radius); row < intMin(current.rows, c.y() + radius); ++row)
    {
        for (int col = intMax(0, c.x() - radius); col < intMin(current.cols, c.x() + radius); ++col)
        {
            if (intSqr(row - c.y()) + intSqr(col - c.x()) <= r2)
            {
                QPointF u = liquefy_get_u(c, m, QPointF(col, row), r2);
                current.at<cv::Vec4b>(row, col) = mat_at_pointf(src, u);
            }
        }
    }
    syncImage();
}
