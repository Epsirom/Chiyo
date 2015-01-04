#include "saturation.h"
#include "ui_saturation.h"

Saturation::Saturation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Saturation)
{
    ui->setupUi(this);
    //QPushButton* applyButton = ui->buttonBox->button(QDialogButtonBox::Apply);
    //connect(applyButton, &QPushButton::clicked, this, &Saturation::applyResult);
    initialize(getImage());
}

void Saturation::initialize(cv::Mat source)
{
    _source = source.clone();
    _increment = 0;
}

Saturation::~Saturation()
{
    delete ui;
}

void Saturation::applyResult()
{
    setImage(getResult());
}

void Saturation::on_increment_valueChanged(int value)
{
    _increment = double(value) / 100.0;
    ui->increment_text->setText(QString("%1%").arg(value));
    applyResult();
}

void Saturation::on_Saturation_accepted()
{
    double t = (double)cv::getTickCount();
    applyResult();
    logInfo(tr("Saturation adjustment applied with increment %1, in %3 s.")
            .arg(_increment)
            .arg(((double)cv::getTickCount() - t) / cv::getTickFrequency()));
}

void Saturation::on_Saturation_rejected()
{
    setImage(_source);
    logInfo(tr("Saturation canceled."));
}

cv::Mat Saturation::getResult()
{
    cv::Mat result = _source.clone();
    for (int row = 0; row < result.rows; ++row)
    {
        for (int col = 0; col < result.cols; ++col)
        {
            cv::Vec4b & inst = result.at<cv::Vec4b>(row, col);
            int rgbMax = inst[0], rgbMin = inst[0];
            for (int i = 1; i < 3; ++i)
            {
                if (inst[i] < rgbMin)
                {
                    rgbMin = inst[i];
                }
                if (inst[i] > rgbMax)
                {
                    rgbMax = inst[i];
                }
            }
            double delta = (rgbMax - rgbMin) / 255.0;
            if (delta == 0)
                continue;
            double value = (rgbMax + rgbMin) / 255.0;
            double L = value / 2;
            double S = L < 0.5 ? delta / value : delta / (2 - value);
            if (_increment >= 0)
            {
                double alpha = (_increment + S >= 1) ? S : 1 - _increment;
                alpha = 1 / alpha - 1;
                for (int i = 0; i < 3; ++i)
                {
                    inst[i] = inst[i] + (inst[i] - L * 255) * alpha;
                }
            }
            else
            {
                double alpha = _increment;
                for (int i = 0; i < 3; ++i)
                {
                    inst[i] = L * 255 + (inst[i] - L * 255) * (1 + alpha);
                }
            }
        }
    }
    return result;
}
