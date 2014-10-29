#include "median.h"
#include "chiyoimagebase.h"

#include <cmath>

#include <vector>

#define tr QObject::tr

cv::Mat apply_median(cv::Mat img, int window_radius)
{
    cv::Mat result = img.clone();
    std::vector<int> window;

    double t = (double)cv::getTickCount();

    for (int row = 0; row < result.rows; ++row)
    {
        for (int col = 0; col < result.cols; ++col)
        {
            for (int i = 0; i < 3; ++i) // B,G,R   3 channels
            {
                int wrow_min = std::max(0, row - window_radius),
                        wrow_max = std::min(result.rows, row + window_radius + 1),
                        wcol_min = std::max(0, col - window_radius),
                        wcol_max = std::min(result.cols, col + window_radius + 1);
                window.clear();
                for (int wrow = wrow_min; wrow < wrow_max; ++wrow)
                {
                    for (int wcol = wcol_min; wcol < wcol_max; ++wcol)
                    {
                        window.push_back(img.at<cv::Vec4b>(wrow, wcol)[i]);
                    }
                }
                std::sort(window.begin(), window.end());
                result.at<cv::Vec4b>(row, col)[i] = window[window.size() / 2];
            }
        }
    }

    logInfo(tr("Median filter with window radius %1 in %2 s.").arg(window_radius)
            .arg(((double)cv::getTickCount() - t) / cv::getTickFrequency()));

    return result;
}
