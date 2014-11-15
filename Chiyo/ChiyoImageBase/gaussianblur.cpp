#include "gaussianblur.h"
#include "ui_gaussianblur.h"
#include <cmath>
#include <QPushButton>
#include <vector>

// http://www.cnblogs.com/hoodlum1980/archive/2008/03/03/1088567.html

GaussianBlur::GaussianBlur(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GaussianBlur)
{
    ui->setupUi(this);
    QPushButton* applyButton = ui->buttonBox->button(QDialogButtonBox::Apply);
    connect(applyButton, &QPushButton::clicked, this, &GaussianBlur::applyResult);
    initialize(getImage());
}

void GaussianBlur::initialize(cv::Mat source)
{
    _source = source.clone();
    _radius = ui->radius->value() / 100.0;
    applyResult();
}

GaussianBlur::~GaussianBlur()
{
    delete ui;
}

void GaussianBlur::applyResult()
{
    _kernel = getGaussianKernel();
    setImage(getResult());
}

void GaussianBlur::on_radius_valueChanged(int value)
{
    _radius = value / 100.0;
    ui->radius_text->setText(QString("%1").arg(_radius));
}

void GaussianBlur::on_GaussianBlur_accepted()
{
    double t = (double)cv::getTickCount();
    applyResult();
    logInfo(tr("Gaussian blur applied with radius %1 and kernel size %2, in %3 s.")
            .arg(_radius)
            .arg(_kernel.rows)
            .arg(((double)cv::getTickCount() - t) / cv::getTickFrequency()));
}

void GaussianBlur::on_GaussianBlur_rejected()
{
    setImage(_source);
    logInfo(tr("Gaussian blur canceled."));
}

double __gaussian(double x, double y, double sigma)
{
    return exp(-(x * x + y * y) / 2 / sigma / sigma);
}

void get_gaussian_queue(double sigma, std::vector<double> &queue, int max_queue_size)
{
    queue.clear();
    double max_g = __gaussian(0, 0, sigma);
    double min_g = max_g * 0.01;
    int x = 0;
    double tmp = max_g;
    while (x < max_queue_size && tmp > min_g)
    {
        queue.push_back(tmp);
        ++x;
        tmp = __gaussian(x, 0, sigma);
    }
}

cv::Mat GaussianBlur::getGaussianKernel()
{
    std::vector<double> gaussian_queue;
    get_gaussian_queue(_radius, gaussian_queue, std::min(_source.rows, _source.cols));

    int kernel_size = gaussian_queue.size();
    cv::Mat kernel(kernel_size, kernel_size, CV_64F);
    double sum_g = 0;

    double g = gaussian_queue[0];
    kernel.at<double>(0, 0) = g;
    sum_g = g;
    for (int col = 1; col < kernel_size; ++col)
    {
        g = gaussian_queue[col];
        kernel.at<double>(0, col) = g;
        kernel.at<double>(col, 0) = g;
        sum_g += (g * 4);
    }

    for (int row = 1; row < kernel_size; ++row)
    {
        for (int col = row; col < kernel_size; ++col)
        {
            g = __gaussian(row, col, _radius);
            kernel.at<double>(row, col) = g;
            kernel.at<double>(col, row) = g;
            int mul = 8 >> int(row == col);
            sum_g += (g * mul);
        }
    }
    for (int row = 0; row < kernel_size; ++row)
    {
        for (int col = row; col < kernel_size; ++col)
        {
            double tmp = kernel.at<double>(row, col) /= sum_g;
            kernel.at<double>(col, row) = tmp;
        }
    }

    return kernel;
}

cv::Mat GaussianBlur::getResult()
{
    cv::Mat result = _source.clone();
    int kradius = _kernel.rows - 1;
    for (int row = 0; row < result.rows; ++row)
    {
        for (int col = 0; col < result.cols; ++col)
        {
            for (int i = 0; i < 3; ++i)     // for R,G,B   3 channels
            {
                double sum_channel = 0;
                for (int wrow = row - kradius; wrow <= row + kradius; ++wrow)
                {
                    for (int wcol = col - kradius; wcol <= col + kradius; ++wcol)
                    {
                        int trow = abs(wrow), tcol = abs(wcol), krow = abs(wrow - row), kcol = abs(wcol - col);
                        trow = trow < result.rows ? trow : result.rows - 1;
                        tcol = tcol < result.cols ? tcol : result.cols - 1;
                        sum_channel += (_kernel.at<double>(krow, kcol) * _source.at<cv::Vec4b>(trow, tcol)[i]);
                    }
                }
                result.at<cv::Vec4b>(row, col)[i] = sum_channel;
            }
        }
    }
    return result;
}
