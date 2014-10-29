#ifndef GAUSSIANBLUR_H
#define GAUSSIANBLUR_H

#include <QDialog>
#include <opencv2/opencv.hpp>
#include "chiyoimagebase.h"

namespace Ui {
class GaussianBlur;
}

class GaussianBlur : public QDialog
{
    Q_OBJECT

public:
    explicit GaussianBlur(QWidget *parent = 0);
    void initialize(cv::Mat source);
    ~GaussianBlur();
    cv::Mat getResult();

public slots:
    void applyResult();

private:
    cv::Mat getGaussianKernel();

private slots:
    void on_radius_valueChanged(int value);

    void on_GaussianBlur_accepted();

    void on_GaussianBlur_rejected();

private:
    Ui::GaussianBlur *ui;

    double _radius;
    cv::Mat _kernel;
    cv::Mat _source;
};

#endif // GAUSSIANBLUR_H
