#include "vignette.h"
#include "ui_vignette.h"
#include <cmath>
#include <QDebug>

Vignette::Vignette(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Vignette),
    _center_col(0),
    _center_row(0),
    _radius(1.0),
    _power(0.8)
{
    ui->setupUi(this);
    initialize(getImage());
}

Vignette::~Vignette()
{
    delete ui;
}

void Vignette::initialize(cv::Mat source)
{
    _source = source;
    auto source_size = source.size();
    _center_col = source_size.width / 2.0;
    _center_row = source_size.height / 2.0;
    ui->center_col->setMaximum(source_size.width);
    ui->center_col->setMinimum(0);
    ui->center_row->setMaximum(source_size.height);
    ui->center_row->setMinimum(0);
    ui->center_col->setValue(_center_col);
    ui->center_row->setValue(_center_row);
    setImage(getResult());
}

void Vignette::on_radius_valueChanged(int value)
{
    _radius = value / 100.0;
    ui->radius_text->setText(QString("%1").arg(_radius));
    setImage(getResult());
}

void Vignette::on_power_valueChanged(int value)
{
    _power = value / 100.0;
    ui->power_text->setText(QString("%1").arg(_power));
    setImage(getResult());
}

void Vignette::on_center_row_valueChanged(int arg1)
{
    _center_row = arg1;
    ui->center_row_text->setText(QString("%1").arg(_center_row));
    setImage(getResult());
}

void Vignette::on_center_col_valueChanged(int arg1)
{
    _center_col = arg1;
    ui->center_col_text->setText(QString("%1").arg(_center_col));
    setImage(getResult());
}

void Vignette::on_Vignette_accepted()
{
    setImage(getResult());
    logInfo(tr("Vignitte applied."));
}

void Vignette::on_Vignette_rejected()
{
    setImage(_source);
    logInfo(tr("Vignitte canceled."));
}

cv::Mat Vignette::getResult()
{
    cv::Mat mask = getMask();
    //cv::imshow("mask", mask);
    //cv::imshow("source_before", _source);
    cv::Mat result = _source.clone();

    for (int row = 0; row < result.rows; ++row)
    {
        for (int col = 0; col < result.cols; ++col)
        {
            auto &pixel = result.at<cv::Vec4b>(row, col);
            auto mask_value = mask.at<double>(row, col);
            pixel[0] *= mask_value;
            pixel[1] *= mask_value;
            pixel[2] *= mask_value;
        }
    }
    //cv::imshow("result", result);
    //cv::imshow("source_after", _source);

    return result;
}

inline double __sqr(double x)
{
    return x * x;
}

inline double __square_distance(double x1, double y1, double x2, double y2)
{
    return __sqr(x1 - x2) + __sqr(y1 - y2);
}

inline double __max(double a, double b)
{
    return a < b ? b : a;
}

inline double __get_vignette_radius(double x, double y, int width, int height)
{
    double result = __square_distance(x, y, 0, 0);
    result = __max(result, __square_distance(x, y, width, 0));
    result = __max(result, __square_distance(x, y, 0, height));
    return __max(result, __square_distance(x, y, width, height));
}

cv::Mat Vignette::getMask()
{
    cv::Mat mask(_source.size(), CV_64F);
    double vignette_radius = _radius * __get_vignette_radius(_center_row, _center_col, mask.rows, mask.cols);
    for (int row = 0; row < mask.rows; ++row)
    {
        for (int col = 0; col < mask.cols; ++col)
        {
            double ratio = sqrt(__square_distance(_center_row, _center_col, row, col) / vignette_radius);
            mask.at<double>(row, col) = __sqr(__sqr(cos(ratio * _power)));
        }
    }
    return mask;
}
