#ifndef LOMO_H
#define LOMO_H

#include <QDialog>
#include <opencv2/opencv.hpp>
#include "chiyoimagebase.h"

typedef cv::Mat (*LomoFunc)(const cv::Mat & src);

cv::Mat lomo_b_channel(const cv::Mat & src);
cv::Mat lomo_g_channel(const cv::Mat & src);
cv::Mat lomo_r_channel(const cv::Mat & src);
cv::Mat lomo_auto(const cv::Mat & src, int channel);

namespace Ui {
class Lomo;
}

class Lomo : public QDialog
{
    Q_OBJECT

public:
    explicit Lomo(QWidget *parent = 0);
    ~Lomo();

private slots:
    void on_Lomo_accepted();

    void on_Lomo_rejected();

private:
    Ui::Lomo *ui;
};

#endif // LOMO_H
