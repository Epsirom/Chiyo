#include "lomo.h"
#include "ui_lomo.h"
#include "gaussianblur.h"
#include "vignette.h"
#include <cmath>
#include <QDebug>

Lomo::Lomo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Lomo)
{
    ui->setupUi(this);
}

Lomo::~Lomo()
{
    delete ui;
}

void Lomo::on_Lomo_accepted()
{
    static LomoFunc lomos[] = {
        lomo_b_channel, lomo_g_channel, lomo_r_channel
    };
    setImage(lomos[ui->comboBox->currentIndex()](getImage()));
    logInfo(tr("Lomo!"));
}

void Lomo::on_Lomo_rejected()
{
    logInfo(tr("Lomo canceled."));
}

cv::Mat lomo_b_channel(const cv::Mat &src)
{
    return lomo_auto(src, 0);
}

cv::Mat lomo_g_channel(const cv::Mat &src)
{
    return lomo_auto(src, 1);
}

cv::Mat lomo_r_channel(const cv::Mat &src)
{
    return lomo_auto(src, 2);
}

cv::Mat lomo_auto(const cv::Mat & src, int channel)
{
    cv::Mat result = src.clone();
    for (int row = 0; row < result.rows; ++row)
    {
        for (int col = 0; col < result.cols; ++col)
        {
            auto& pixel = result.at<cv::Vec4b>(row, col);
            pixel[channel] = fmin(10 * sqrt(pixel[channel]), 255);
        }
    }
    GaussianBlur gaussian;
    gaussian.initialize(result);
    gaussian.setRadius(0.8);

    cv::Mat blurred = gaussian.getResult();

    Vignette vignette(blurred, 3);
    return vignette.getResult();
}
