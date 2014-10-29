#ifndef VIGNETTE_H
#define VIGNETTE_H

#include <opencv2/opencv.hpp>
#include <QDialog>
#include "chiyoimagebase.h"

namespace Ui {
class Vignette;
}

class Vignette : public QDialog
{
    Q_OBJECT

public:
    explicit Vignette(QWidget *parent = 0);
    void initialize(cv::Mat source);
    ~Vignette();
    cv::Mat getResult();

private:
    cv::Mat getMask();

private slots:
    void on_radius_valueChanged(int value);
    void on_power_valueChanged(int value);
    void on_center_row_valueChanged(int arg1);
    void on_center_col_valueChanged(int arg1);

    void on_Vignette_accepted();

    void on_Vignette_rejected();

private:
    Ui::Vignette *ui;
    double _center_row;
    double _center_col;
    double _radius;
    double _power;
    cv::Mat _source;
};

#endif // VIGNETTE_H
