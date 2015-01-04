#include "sharpen.h"
#include "gaussianblur.h"

#define tr QObject::tr

//http://www.cnblogs.com/Imageshop/archive/2013/05/19/3086388.html

cv::Mat auto_sharpen(cv::Mat img)
{
    cv::Mat result = img.clone();
    double t = (double)cv::getTickCount();

    int lambda = 50;
    int threshold = 0;
    static GaussianBlur gaussian;
    gaussian.setRadius(1);
    cv::Mat blurred = gaussian.getResult();
    for (int row = 0; row < result.rows; ++row)
    {
        for (int col = 0; col < result.cols; ++col)
        {
            auto &src = img.at<cv::Vec4b>(row, col);
            auto &b = blurred.at<cv::Vec4b>(row, col);
            auto &dst = result.at<cv::Vec4b>(row, col);
            for (int i = 0; i < 3; ++i)
            {
                int diff = src[i] - b[i];
                if (intAbs(diff) > threshold)
                {
                    diff = src[i] + lambda * diff / 100;
                    dst[i] = (uchar)(((((ushort)diff | ((short)(255 - diff) >> 15)) & ~diff >> 15)));
                }
            }
        }
    }

    logInfo(tr("Sharpen applied in %1 s.")
            .arg(((double)cv::getTickCount() - t) / cv::getTickFrequency()));

    return result;
}
