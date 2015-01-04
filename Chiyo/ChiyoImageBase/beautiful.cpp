#include "beautiful.h"
#include <cstring>
#include <vector>
#include "chiyoimagebase.h"
#include <QObject>
#include <algorithm>
#include "gaussianblur.h"
#include <QCoreApplication>

#define tr QObject::tr

using std::string;

std::vector<cv::Rect> detectFaces(cv::Mat & frame_gray, bool & isOK);
std::vector<cv::Rect> detectEyes(cv::Mat & frame_gray, cv::Rect & face, bool & isOK);

typedef struct RectInfo
{
    double center_x, center_y;
    double half_width_sqr, half_height_sqr;
} RectInfo;

RectInfo getRectInfo(cv::Rect & rect)
{
    RectInfo info;
    info.center_x = rect.x + rect.width * 0.5;
    info.center_y = rect.y + rect.height * 0.5;
    info.half_width_sqr = doubleSqr(rect.width * 0.5);
    info.half_height_sqr = doubleSqr(rect.height * 0.5);
    return info;
}

bool isInnerEllipse(double dx, double dy, RectInfo & fi)
{
    return (doubleSqr(dx - fi.center_x) / fi.half_width_sqr + doubleSqr(dy - fi.center_y) / fi.half_height_sqr < 1);
}

cv::Vec4b & bilateralFilter(const cv::Mat & src, cv::Mat & dst, int row, int col)
{
    static int radius = 5;
    static double sigma_s = 10, sigma_r = 15;
    double channel_sum[3] = {0};
    double coe_sum = 0;
    const cv::Vec4b & px = src.at<const cv::Vec4b>(row, col);
    for (int r = std::max<int>(0, row - radius); r <= std::min<int>(src.rows - 1, row + radius); ++r)
    {
        for (int c = std::max<int>(0, col - radius); c <= std::min<int>(src.cols - 1, col + radius); ++c)
        {
            const cv::Vec4b & p = src.at<const cv::Vec4b>(r, c);
            double color_dist = 0;
            for (int i = 0; i < 3; ++i)
            {
                color_dist += doubleSqr(px[i] - p[i]);
            }
            double coe = __gaussian(r - row, c - col, sigma_s) * gaussian_func(color_dist, sigma_r);
            coe_sum += coe;
            for (int i = 0; i < 3; ++i)
            {
                channel_sum[i] += coe * p[i];
            }
        }
    }
    cv::Vec4b & pt = dst.at<cv::Vec4b>(row, col);
    for (int i = 0; i < 3; ++i)
    {
        pt[i] = channel_sum[i] / coe_sum;
    }
    return pt;
}

cv::Mat auto_beautiful_face(const cv::Mat &src)
{
    static double channel_ratio[3] = {0.1, 0.4, 0.5};
    static double white_ratio = 0.2;
    double t = (double)cv::getTickCount();
    cv::Mat result = src.clone();
    cv::Mat gray;
    cv::cvtColor( result, gray, CV_BGR2GRAY );
    bool isOK = false;
    std::vector<cv::Rect> faces = detectFaces(gray, isOK);
    if (!isOK) {
        return src;
    }
    if (faces.size() == 0)
    {
        logWarning(tr("No faces found."));
    }
    for (cv::Rect & face : faces)
    {
        face.x = std::max<int>(0, face.x - face.width * 0.1);
        face.y = std::max<int>(0, face.y - face.height * 0.1);
        face.width = std::min<int>(src.cols - face.x, face.width * 1.2);
        face.height = std::min<int>(src.rows - face.y, face.height * 1.2);
        std::vector<cv::Rect> eyes;
        if (false)
            eyes = detectEyes(gray, face, isOK);
        if (!isOK) {
            return src;
        }
        RectInfo fi = getRectInfo(face);
        double channels_sum[3] = {0};
        int face_pixels = 0;
        for (int dx = face.x; dx < face.x + face.width; ++dx)
        {
            for (int dy = face.y; dy < face.y + face.height; ++dy)
            {
                if (isInnerEllipse(dx, dy, fi))
                {
                    ++face_pixels;
                    bool isInnerEyeRegion = false;
                    for (cv::Rect & eye : eyes)
                    {
                        RectInfo ei = getRectInfo(eye);
                        if (isInnerEllipse(dx, dy, ei))
                        {
                            isInnerEyeRegion = true;
                            break;
                        }
                    }
                    if (!isInnerEyeRegion)
                    {
                        cv::Vec4b & pt = bilateralFilter(src, result, dy, dx);
                        for (int i = 0; i < 3; ++i)
                        {
                            channels_sum[i] += pt[i];
                        }
                    }
                }
            }
        }
        for (int i = 0; i < 3; ++i)
        {
            channels_sum[i] /= face_pixels;
        }
        for (int dx = face.x; dx < face.x + face.width; ++dx)
        {
            for (int dy = face.y; dy < face.y + face.height; ++dy)
            {
                if (isInnerEllipse(dx, dy, fi))
                {
                    cv::Vec4b & pt = result.at<cv::Vec4b>(dy, dx);
                    double dist = 0;
                    for (int i = 0; i < 3; ++i)
                    {
                        dist += channel_ratio[i] * doubleSqr(pt[i] - channels_sum[i]);
                    }
                    double scale = 1 + white_ratio * exp(-sqrt(dist));
                    for (int i = 0; i < 3; ++i)
                    {
                        double tmp = pt[i] * scale;
                        pt[i] = tmp > 255 ? 255 : tmp;
                    }
                }
            }
        }
    }

    logInfo(tr("Face beautified in %1 s.")
            .arg(((double)cv::getTickCount() - t) / cv::getTickFrequency()));

    return result;
}

std::vector<cv::Rect> detectFaces(cv::Mat & frame_gray, bool & isOK)
{
    static string face_cascade_name = (QCoreApplication::applicationDirPath() + "/../Data/haarcascade_frontalface_alt.xml").toStdString();
    static cv::CascadeClassifier face_cascade;
    static bool is_loaded = false;
    if (!is_loaded)
    {
        if (!face_cascade.load(face_cascade_name))
        {
            logError(tr("Load face cascade failed."));
            isOK = false;
            return std::vector<cv::Rect>();
        }
        is_loaded = true;
    }
    std::vector<cv::Rect> faces;
    equalizeHist(frame_gray, frame_gray);
    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
    isOK = true;
    return faces;
}

std::vector<cv::Rect> detectEyes(cv::Mat & frame_gray, cv::Rect & face, bool &isOK)
{
    static string eye_cascade_name = (QCoreApplication::applicationDirPath() + "/../Data/haarcascade_eye_tree_eyeglasses.xml").toStdString();
    static cv::CascadeClassifier eye_cascade;
    static bool is_loaded = false;
    if (!is_loaded)
    {
        if (!eye_cascade.load(eye_cascade_name))
        {
            logError(tr("Load faces cascade failed."));
            isOK = false;
            return std::vector<cv::Rect>();
        }
        is_loaded = true;
    }
    cv::Mat faceROI = frame_gray(face);
    std::vector<cv::Rect> eyes;
    eye_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
    isOK = true;
    return eyes;
}
