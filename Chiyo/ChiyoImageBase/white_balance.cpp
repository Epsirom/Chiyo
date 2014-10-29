#include "white_balance.h"
#include <vector>
#include <cmath>
#include "chiyoimagebase.h"
#define tr QObject::tr

// http://www.cnblogs.com/Imageshop/archive/2013/04/20/3032062.html

inline int __sign(int x)
{
    return x < 0 ? -1 : (x > 0 ? 1 : 0);
}

inline int __signed_char(int x)
{
    return x - 128;
}

typedef struct ReferencePoint
{
    int y;
    cv::Vec3b bgr;
} ReferencePoint;

void get_white_references(cv::Mat &ycrcb, cv::Mat &bgr,
                          int row_from, int row_to, int col_from, int col_to,
                          std::vector<ReferencePoint> &refs, int &y_max)
{
    double sum_cr = 0, sum_cb = 0;
    int sum_pixels = (row_to - row_from) * (col_to - col_from);
    for (int row = row_from; row < row_to; ++row)
    {
        for (int col = col_from; col < col_to; ++col)
        {
            auto &pixel = ycrcb.at<cv::Vec3b>(row, col);
            sum_cr += __signed_char(pixel[1]);
            sum_cb += __signed_char(pixel[2]);
            if (y_max < pixel[0])
            {
                y_max = pixel[0];
            }
        }
    }
    double mr = sum_cr / sum_pixels, mb = sum_cb / sum_pixels;
    sum_cr = 0;
    sum_cb = 0;
    for (int row = row_from; row < row_to; ++row)
    {
        for (int col = col_from; col < col_to; ++col)
        {
            auto &pixel = ycrcb.at<cv::Vec3b>(row, col);
            sum_cr += fabs(__signed_char(pixel[1]) - mr);
            sum_cb += fabs(__signed_char(pixel[2]) - mb);
        }
    }
    double db = sum_cr / sum_pixels, dr = sum_cb / sum_pixels;
    int smb = __sign(mb), smr = __sign(mr);
    for (int row = row_from; row < row_to; ++row)
    {
        for (int col = col_from; col < col_to; ++col)
        {
            auto &pixel = ycrcb.at<cv::Vec3b>(row, col);
            if (fabs(__signed_char(pixel[2]) - (mb + db * smb)) < 1.5 * db &&
                    fabs(__signed_char(pixel[1]) - (1.5 * mr + dr * smr)) < 1.5 * dr)
            {
                ReferencePoint p;
                p.y = pixel[0];
                p.bgr = bgr.at<cv::Vec3b>(row, col);
                refs.push_back(p);
            }
        }
    }
}

bool sort_ReferencePoint(ReferencePoint &p1, ReferencePoint &p2)
{
    return p1.y > p2.y;
}

cv::Mat auto_adjust_white_balance(cv::Mat img)
{
    int square_size = std::max(img.rows, img.cols) / 4;
    cv::Mat result = img.clone();
    cv::Mat img_bgr(img.size(), CV_8UC3);
    cv::Mat img_ycrcb(img.size(), CV_8UC3);
    cv::cvtColor(img, img_bgr, CV_BGRA2BGR);
    cv::cvtColor(img_bgr, img_ycrcb, CV_BGR2YCrCb);

    double t = (double)cv::getTickCount();

    std::vector<ReferencePoint> refs;
    int y_max = 0;
    for (int row = 0; row < img_ycrcb.rows; row += square_size)
    {
        for (int col = 0; col < img_ycrcb.cols; col += square_size)
        {
            get_white_references(img_ycrcb, img_bgr,
                                 row, std::min(row + square_size, img_ycrcb.rows),
                                 col, std::min(col + square_size, img_ycrcb.cols),
                                 refs, y_max);
        }
    }
    std::sort(refs.begin(), refs.end(), sort_ReferencePoint);
    int refs_count = refs.size();
    refs_count = refs_count < 2 ? refs_count : refs_count / 10 + 1;
    int sum_r = 0, sum_g = 0, sum_b = 0;
    for (int i = 0; i < refs_count; ++i)
    {
        ReferencePoint &p = refs[i];
        sum_b += p.bgr[0];
        sum_g += p.bgr[1];
        sum_r += p.bgr[2];
    }
    double gain_r = y_max / (double(sum_r) / refs_count),
            gain_g = y_max / (double(sum_g) / refs_count),
            gain_b = y_max / (double(sum_b) / refs_count);
    for (int row = 0; row < result.rows; ++row)
    {
        for (int col = 0; col < result.cols; ++col)
        {
            auto &pixel = result.at<cv::Vec4b>(row, col);
            pixel[0] = std::min(255, int(pixel[0] * gain_b));
            pixel[1] = std::min(255, int(pixel[1] * gain_g));
            pixel[2] = std::min(255, int(pixel[2] * gain_r));
        }
    }

    logInfo(tr("White balance adjusted in %1 s.")
            .arg(((double)cv::getTickCount() - t) / cv::getTickFrequency()));

    return result;
}
