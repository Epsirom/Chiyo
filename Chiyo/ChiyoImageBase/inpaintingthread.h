#ifndef INPAINTINGTHREAD_H
#define INPAINTINGTHREAD_H

#include <QThread>
#include "chiyoimagebase.h"
#include <opencv2/opencv.hpp>

#define PATCH_SIZE 7
#define PYRAMID_LEVELS 4
#define ANN_ITERATIONS 3

class InpaintingThread : public QThread
{
    Q_OBJECT
public:
    explicit InpaintingThread(QObject *parent = 0);
    void initialize(const cv::Mat& src, const cv::Mat& mask, int patchSize, int pyramidIterations, int ANNIterations);
    cv::Mat getResult();

    void erodeMask(cv::Mat& mask);

    void initMask(cv::Mat& mask);
    void initOffset(const cv::Mat& mask, cv::Mat& off);

    void applyOffset(cv::Mat& dst, const cv::Mat& src, const cv::Mat& mask, const cv::Mat& off);

    double dist(const cv::Mat& dst, const cv::Mat& src);
    cv::Mat getPatch(const cv::Mat& src, int row, int col);
    double getDelta(double* sim, int length);
    cv::Vec3b meanShift(double* w, cv::Vec3b* c, int len, int odd);
    cv::Vec3b generatePixel(const cv::Mat& dst, const cv::Mat& src, const cv::Mat& mask, cv::Mat& off, int row, int col, int odd);
    void generateImage(cv::Mat& dst, const cv::Mat& src, const cv::Mat& mask, cv::Mat& off, int odd);

    // patch match
    void randomSearch(const cv::Mat& dst, const cv::Mat& src, const cv::Mat& mask, cv::Mat& off, int row, int col);
    void propagation(const cv::Mat& dst, const cv::Mat& src, const cv::Mat& mask, cv::Mat& off, int row, int col, int odd);

    // Pyramid operations
    void pyramidDownMask(const cv::Mat& inMask, cv::Mat& outMask);
    void pyramidUpOffset(const cv::Mat& inOffset, const cv::Mat& inMask, cv::Mat& outOffset);

    // commond funcs
    double getWindowSize(cv::Vec3b* c, int len);

protected:
    void run();

signals:

public slots:

protected:
    cv::Mat dst, src, mask, offset;
    int patchSize, randomWindow, minWindow, pyramidIterations, ANNIterations;
    cv::RNG rng;

    double randomAttenuation, searchRowRatio;
};

#endif // INPAINTINGTHREAD_H
