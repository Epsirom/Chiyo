#ifndef SATURATION_H
#define SATURATION_H

#include <QDialog>
#include <opencv2/opencv.hpp>
#include "chiyoimagebase.h"

namespace Ui {
class Saturation;
}

class Saturation : public QDialog
{
    Q_OBJECT

public:
    explicit Saturation(QWidget *parent = 0);
    void initialize(cv::Mat source);
    ~Saturation();
    cv::Mat getResult();

public slots:
    void applyResult();

private slots:
    void on_increment_valueChanged(int value);

    void on_Saturation_accepted();

    void on_Saturation_rejected();

private:
    Ui::Saturation *ui;

    cv::Mat _source;
    double _increment;
};

#endif // SATURATION_H
