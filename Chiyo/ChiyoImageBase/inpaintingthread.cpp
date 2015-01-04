#include "inpaintingthread.h"
#include <QDebug>
#include <ctime>

InpaintingThread::InpaintingThread(QObject *parent) :
    QThread(parent),
    rng((unsigned)time(NULL))
{
}

void InpaintingThread::run()
{
    cv::Mat origin = getImage();
    cv::Mat src = cv::Mat(origin.size(), CV_8UC3);
    cv::cvtColor(origin, src, CV_BGRA2BGR);
    cv::Mat mask = cv::Mat(src.size(), CV_8UC1, cv::Scalar::all(0));

    for (int row = 0; row < mask.rows; ++row)
    {
        for (int col = 0; col < mask.cols; ++col)
        {
            cv::Vec3b& pt = src.at<cv::Vec3b>(row, col);
            if (pt == cv::Vec3b(0, 0, 255))
            {
                mask.at<uchar>(row, col) = 255;
            }
            else
            {
                mask.at<uchar>(row, col) = 128;
            }
        }
    }

    initialize(src, mask, PATCH_SIZE, PYRAMID_LEVELS, ANN_ITERATIONS);
    setImage(getResult());
}

void InpaintingThread::erodeMask(cv::Mat &mask)
{
    cv::Mat m = mask.clone();
    int erodeRadius = patchSize * 1.5;
    for (int row = 0; row < mask.rows; ++row)
    {
        for (int col = 0; col < mask.cols; ++col)
        {
            if (m.at<uchar>(row, col) == 255)
            {
                int rowFrom = intMax(0, row - erodeRadius);
                int rowTo = intMin(m.rows, row + erodeRadius);
                int colFrom = intMax(0, col - erodeRadius);
                int colTo = intMin(m.cols, col + erodeRadius);

                for (int i = rowFrom; i < rowTo; ++i)
                {
                    for (int j = colFrom; j < colTo; ++j)
                    {
                        mask.at<uchar>(i, j) = 255;
                    }
                }
            }
        }
    }
}

void InpaintingThread::initialize(const cv::Mat &src, const cv::Mat &mask, int patchSize, int pyramidIterations, int ANNIterations)
{
    this->src = src.clone();
    this->dst = src.clone();
    this->mask = mask.clone();
    this->offset = cv::Mat::zeros(mask.size(), CV_32SC2);
    this->patchSize = patchSize;
    this->pyramidIterations = pyramidIterations;
    this->ANNIterations = ANNIterations;
    this->randomWindow = -1;
    this->minWindow = 7 * patchSize;
    this->randomAttenuation = 0.5;
    this->searchRowRatio = 0.5;

    erodeMask(this->mask);
}

cv::Mat InpaintingThread::getResult()
{
    cv::Mat* py_src = new cv::Mat[pyramidIterations + 1];
    cv::Mat* py_dst = new cv::Mat[pyramidIterations + 1];
    cv::Mat* py_mask_ = new cv::Mat[pyramidIterations + 1];
    cv::Mat* py_mask = new cv::Mat[pyramidIterations + 1];
    cv::Mat* py_offset = new cv::Mat[pyramidIterations + 1];

    py_src[0] = src;
    py_dst[0] = dst;
    py_mask_[0] = mask;
    py_offset[0] = offset;

    py_mask[0] = py_mask_[0].clone();
    initMask(py_mask[0]);

    //cv::imshow(tr("mask%1").arg(0).toStdString(), py_mask[0]);

    int pyramid;

    for (pyramid = 0; pyramid < pyramidIterations; ++pyramid)
    {
        if (py_dst[pyramid].rows <= minWindow || py_dst[pyramid].cols <= minWindow)
        {
            break;
        }
        cv::pyrDown(py_src[pyramid], py_src[pyramid + 1]);
        cv::pyrDown(py_dst[pyramid], py_dst[pyramid + 1]);
        pyramidDownMask(py_mask_[pyramid], py_mask_[pyramid + 1]);
        py_mask[pyramid + 1] = py_mask_[pyramid + 1].clone();
        initMask(py_mask[pyramid + 1]);
        //cv::imshow(tr("mask%1").arg(pyramid + 1).toStdString(), py_mask[pyramid + 1]);
    }

    initOffset(py_mask[pyramid], py_offset[pyramid]);

    while (pyramid >= 0)
    {
        logInfo(tr("Pyramid %1").arg(pyramid));
        //qDebug() << tr("Pyramid up %1").arg(pyramid);
        randomWindow = intMax(py_src[pyramid].rows, py_src[pyramid].cols);

        for (int i = 0; i < ANNIterations + pyramid * 5; ++i)
        {
            //logInfo(tr("Iteration %1").arg(i));
            //qDebug() << tr("Iteration %1").arg(i);
            //cv::imshow("mask", py_mask[pyramid]);
            applyOffset(py_dst[pyramid], py_src[pyramid], py_mask[pyramid], py_offset[pyramid]);
            //cv::imshow("dst", py_dst[pyramid]);
            generateImage(py_dst[pyramid], py_src[pyramid], py_mask[pyramid], py_offset[pyramid], i);
        }

        if (pyramid > 0)
        {
            pyramidUpOffset(py_offset[pyramid], py_mask[pyramid - 1], py_offset[pyramid - 1]);
        }

        --pyramid;
    }

    dst = py_dst[0];

    delete [] py_src;
    delete [] py_dst;
    delete [] py_mask;
    delete [] py_offset;

    return dst;
}

void InpaintingThread::initMask(cv::Mat &mask)
{
    for (int row = 0; row < mask.rows; ++row)
    {
        for (int col = 0; col < mask.cols; ++col)
        {
            if (mask.at<uchar>(row, col) == 128)
            {
                cv::Mat patch = getPatch(mask, row, col);
                if (patch.rows * patch.cols != patchSize * patchSize)
                {
                    mask.at<uchar>(row, col) = 0;
                }
            }
        }
    }
}

void InpaintingThread::applyOffset(cv::Mat &dst, const cv::Mat &src, const cv::Mat &mask, const cv::Mat &off)
{
    for (int row = 0; row < dst.rows; ++row)
    {
        for (int col = 0; col < dst.cols; ++col)
        {
            if (mask.at<uchar>(row, col) == 255)
            {
                const cv::Vec2i& offset = off.at<cv::Vec2i>(row, col);
                dst.at<cv::Vec3b>(row, col) = src.at<cv::Vec3b>(row + offset[0], col + offset[1]);
            }
        }
    }
}

cv::Mat InpaintingThread::getPatch(const cv::Mat &src, int row, int col)
{
    int row_from = intMax(0, row - patchSize / 2);
    int row_to = intMin(src.rows - 1, row + patchSize / 2);
    int col_from = intMax(0, col - patchSize / 2);
    int col_to = intMin(src.cols - 1, col + patchSize / 2);
    return src(cv::Range(row_from, row_to + 1), cv::Range(col_from, col_to + 1));
}

void InpaintingThread::pyramidDownMask(const cv::Mat &inMask, cv::Mat &outMask)
{
    int outRows = (inMask.rows + 1) / 2;
    int outCols = (inMask.cols + 1) / 2;
    outMask = cv::Mat(outRows, outCols, inMask.type());
    for (int row = 0; row < outRows; ++row)
    {
        for (int col = 0; col < outCols; ++col)
        {
            auto& outPixel = outMask.at<uchar>(row, col);
            int r2 = row * 2, c2 = col * 2;
            // inMask window
            uchar in00 = inMask.at<uchar>(r2, c2),
                    in10 = inMask.at<uchar>(r2 + 1, c2),
                    in01 = inMask.at<uchar>(r2, c2 + 1),
                    in11 = inMask.at<uchar>(r2 + 1, c2 + 1);
            if (r2 + 1 < outRows && c2 + 1 < outCols)
            {
                if (in00 == 255 || in10 == 255 || in01 == 255 || in11 == 255)
                {
                    outPixel = 255;
                }
                else if (in00 == 0 || in10 == 0 || in01 == 0 || in11 == 0)
                {
                    outPixel = 0;
                }
                else
                {
                    outPixel = 128;
                }
            }
            else if (r2 + 1 < outRows && c2 + 1 == outCols)
            {
                if (in00 == 255 || in10 == 255)
                {
                    outPixel = 255;
                }
                else if (in00 == 0 || in10 == 0)
                {
                    outPixel = 0;
                }
                else
                {
                    outPixel = 128;
                }
            }
            else if (r2 + 1 == outRows && c2 + 1 < outCols)
            {
                if (in00 == 255 || in01 == 255)
                {
                    outPixel = 255;
                }
                else if (in00 == 0 || in01 == 0)
                {
                    outPixel = 0;
                }
                else
                {
                    outPixel = 128;
                }
            }
            else
            {
                outPixel = in00;
            }
        }
    }
}

void InpaintingThread::initOffset(const cv::Mat &mask, cv::Mat &off)
{
    off = cv::Mat(mask.size(), CV_32SC2, cv::Scalar::all(0));

    for (int row = 0; row < mask.rows; ++row)
    {
        for (int col = 0; col < mask.cols; ++col)
        {
            if (mask.at<uchar>(row, col) == 128)
            {
                off.at<cv::Vec2i>(row, col) = cv::Vec2i(0, 0);
            }
            else
            {
                int r_row, r_col;
                do {
                    r_row = rng.uniform(0, mask.rows) - row;
                    r_col = rng.uniform(0, mask.cols) - col;
                } while (mask.at<uchar>(r_row + row, r_col + col) != 128 || intAbs(r_row) >= searchRowRatio * mask.rows);
                off.at<cv::Vec2i>(row, col) = cv::Vec2i(r_row, r_col);
            }
        }
    }
}

void InpaintingThread::generateImage(cv::Mat &dst, const cv::Mat &src, const cv::Mat &mask, cv::Mat &off, int odd)
{
    int row_from = 0;
    int row_to = dst.rows - 1;
    int col_from = 0;
    int col_to = dst.cols - 1;
    int step = 1;
    if (odd & 1)
    {
        row_from = row_to;
        row_to = 0;
        col_from = col_to;
        col_to = 0;
        step = -1;
    }
    for (int row = row_from; step > 0 ? row <= row_to : row >= row_to; row += step)
    {
        for (int col = col_from; step > 0 ? col <= col_to : col >= col_to; col += step)
        {
            if (mask.at<uchar>(row, col) == 255)
            {
                dst.at<cv::Vec3b>(row, col) = generatePixel(dst, src, mask, off, row, col, odd);
            }
        }
    }
}

cv::Vec3b InpaintingThread::generatePixel(const cv::Mat &dst, const cv::Mat &src, const cv::Mat &mask, cv::Mat &off, int row, int col, int odd)
{
    cv::Mat patch = getPatch(dst, row, col);
    int windowLength = patch.rows * patch.cols;
    double* a = new double[windowLength];
    double* dists = new double[windowLength];
    double* w = new double[windowLength];
    cv::Vec3b* pixels = new cv::Vec3b[windowLength];
    int currentLen = 0;
    double ratio = doubleMin(1.0, sqrt(1.0 * dst.rows * dst.rows + dst.cols * dst.cols) / 150);

    int it_from = 0, it_to = patchSize - 1, it_step = 1;
    if (odd & 1)
    {
        it_from = patchSize - 1;
        it_to = 0;
        it_step = -1;
    }

    int patchRadius = patchSize / 2;

    for (int i = it_from; (it_step > 0) ? (i <= it_to) : (i >= it_to); i += it_step)
    {
        for (int j = it_from; (it_step > 0) ? (j <= it_to) : (j >= it_to); j += it_step)
        {
            int rr = row - patchRadius + i;
            int cc = col - patchRadius + j;
            if (rr < patchRadius || cc < patchRadius || rr > dst.rows - 1 - patchRadius || cc > dst.cols - 1 - patchRadius || mask.at<uchar>(rr, cc) == 0)
            {
                continue;
            }
            propagation(dst, src, mask, off, rr, cc, odd);
            randomSearch(dst, src, mask, off, rr, cc);

            cv::Vec2i offCenter = cv::Vec2i(rr, cc) + off.at<cv::Vec2i>(rr, cc);
            cv::Mat srcPatch = getPatch(src, offCenter[0], offCenter[1]);
            cv::Mat dstPatch = getPatch(dst, rr, cc);

            pixels[currentLen] = srcPatch.at<cv::Vec3b>(patchSize - 1 - i, patchSize - 1 - j);
            a[currentLen] = sqrt(doubleSqr(rr - offCenter[0]) + doubleSqr(cc - offCenter[1])) / ratio;
            dists[currentLen] = dist(dstPatch, srcPatch);
            ++currentLen;
        }
    }

    double delta = getDelta(dists, currentLen);
    if (delta == 0)
    {
        delta = 0.5;    // Avoid divided by zero.
    }
    for (int i = 0; i < currentLen; ++i)
    {
        dists[i] = exp(-dists[i] / (2 * delta));
        a[i] = pow(1.3, -a[i]);
        w[i] = dists[i] * a[i];
    }

    cv::Vec3b result = meanShift(w, pixels, currentLen, odd);
    delete [] a;
    delete [] dists;
    //delete [] w;
    //delete [] c;
    return result;
}

cv::Vec3b InpaintingThread::meanShift(double *w, cv::Vec3b *c, int len, int odd)
{
    double delta = getWindowSize(c, len);
    cv::Vec3b center = odd & 1 ? c[len - 1] : c[0];

    double* w1 = w;
    cv::Vec3b* c1 = c;
    int len1 = len;

    while (true)
    {
        double* w2 = new double[len1];
        cv::Vec3b* c2 = new cv::Vec3b[len1];
        int len2 = 0;

        for (int i = 0; i < len1; ++i)
        {
            double tmp = 0;
            for (int channel = 0; channel < 3; ++channel)
            {
                tmp += doubleSqr(c1[i][channel] - center[channel]);
            }
            if (tmp < delta)
            {
                w2[len2] = w1[i];
                c2[len2] = c1[i];
                ++len2;
            }
        }
        if (len2 == 0)
            break;
        double avg[3] = {0};
        for (int i = 0; i < len2; ++i)
        {
            for (int channel = 0; channel < 3; ++channel)
            {
                avg[channel] += (c2[i][channel] - center[channel]);
            }
        }
        cv::Vec3b center2;
        double tmp_ = 0;
        for (int channel = 0; channel < 3; ++channel)
        {
            avg[channel] /= len2;
            center2[channel] = avg[channel] + center[channel];
            tmp_ += doubleSqr(center2[channel] - center[channel]);
        }

        delete [] c1;
        delete [] w1;

        c1 = c2;
        w1 = w2;
        len1 = len2;

        if (tmp_ < 1)
        {
            break;
        }
        else
        {
            center = center2;
        }
    }

    double sum[3] = {0};
    double weight = 0;
    for (int i = 0; i < len1; ++i)
    {
        weight += w1[i];
        for (int channel = 0; channel < 3; ++channel)
        {
            sum[channel] += double(c1[i][channel]) * w1[i];
        }
    }
    delete [] w1;
    delete [] c1;
    cv::Vec3b result;
    for (int channel = 0; channel < 3; ++channel)
    {
        result[channel] = sum[channel] / weight;
    }
    return result;
}

double InpaintingThread::getWindowSize(cv::Vec3b *c, int len)
{
    double avg[3] = {0};
    for (int i = 0; i < len; ++i)
    {
        for (int channel = 0; channel < 3; ++channel)
        {
            avg[channel] += c[i][channel];
        }
    }
    for (int channel = 0; channel < 3; ++channel)
    {
        avg[channel] /= len;
    }
    double delta = 0;
    for (int i = 0; i < len; ++i)
    {
        for (int channel = 0; channel < 3; ++channel)
        {
            delta += doubleSqr(avg[channel] - c[i][channel]);
        }
    }
    return delta;
}

void InpaintingThread::pyramidUpOffset(const cv::Mat &inOffset, const cv::Mat &inMask, cv::Mat &outOffset)
{
    int outRows = inMask.rows;
    int outCols = inMask.cols;

    outOffset = cv::Mat(outRows, outCols, inOffset.type());

    for (int row = 0; row < inOffset.rows; ++row)
    {
        for (int col = 0; col < inOffset.cols; ++col)
        {
            auto& inPixel = inOffset.at<cv::Vec2i>(row, col);
            cv::Vec2i in2 = inPixel * 2;
            int r2 = row * 2, c2 = col * 2;
            // inMask window
            cv::Vec2i &out00 = outOffset.at<cv::Vec2i>(r2, c2),
                    &out10 = outOffset.at<cv::Vec2i>(r2 + 1, c2),
                    &out01 = outOffset.at<cv::Vec2i>(r2, c2 + 1),
                    &out11 = outOffset.at<cv::Vec2i>(r2 + 1, c2 + 1);
            if (r2 + 1 < outRows && c2 + 1 < outCols)
            {
                out00 = in2;
                out10 = in2;
                out01 = in2;
                out11 = in2;
            }
            else if (r2 + 1 < outRows && c2 + 1 == outCols)
            {
                out00 = in2;
                out10 = in2;
            }
            else if (r2 + 1 == outRows && c2 + 1 < outCols)
            {
                out00 = in2;
                out01 = in2;
            }
            else
            {
                out00 = in2;
            }
        }
    }
}

void InpaintingThread::propagation(const cv::Mat &dst, const cv::Mat &src, const cv::Mat &mask, cv::Mat &off, int row, int col, int odd)
{
    cv::Mat dstPatch = getPatch(dst, row, col);
    cv::Vec2i& offset = off.at<cv::Vec2i>(row, col);
    cv::Mat srcPatch = getPatch(src, row + offset[0], col + offset[1]);
    double patchDist = dist(dstPatch, srcPatch);
    static int dir[][2] = {
        {1, 0},
        {0, 1},
        {-1, 0},
        {0, -1}
    };
    int dir_start = odd & 1 ? 0 : 2;
    for (int i = 0; i < 2; ++i)
    {
        int dir_idx = dir_start + i;
        cv::Vec2i& cmpOffset = off.at<cv::Vec2i>(row + dir[dir_idx][0], col + dir[dir_idx][1]);
        cv::Mat cmpPatch = getPatch(src, row + cmpOffset[0] + dir[dir_idx][0], col + cmpOffset[1] + dir[dir_idx][1]);
        double cmpDist = dist(dstPatch, cmpPatch);
        if (cmpDist < patchDist)
        {
            patchDist = cmpDist;
            offset[0] = cmpOffset[0] + dir[dir_idx][0];
            offset[1] = cmpOffset[1] + dir[dir_idx][1];
        }
    }
}

void InpaintingThread::randomSearch(const cv::Mat &dst, const cv::Mat &src, const cv::Mat &mask, cv::Mat &off, int row, int col)
{
    cv::Mat dstPatch = getPatch(dst, row, col);
    cv::Vec2i& offset = off.at<cv::Vec2i>(row, col);
    cv::Mat srcPatch = getPatch(src, row + offset[0], col + offset[1]);
    double patchDist = dist(dstPatch, srcPatch);

    int attenuate = 0;

    while (true)
    {
        double trow = randomWindow * pow(randomAttenuation, attenuate);
        double tcol = trow * rng.uniform(-1.0, 1.0);
        trow *= rng.uniform(-1.0, 1.0);

        if (doubleSqr(trow) + doubleSqr(tcol) < 1)
        {
            break;
        }

        int cmpRow = offset[0] + row + trow;
        int cmpCol = offset[1] + col + tcol;
        if (cmpRow >= 0 && cmpRow < off.rows
                && cmpCol >= 0 && cmpCol < off.cols
                && mask.at<uchar>(cmpRow, cmpCol) == 128
                && intAbs(cmpRow - row) < searchRowRatio * mask.rows
                && intAbs(cmpCol - col) < searchRowRatio * mask.cols)
        {
            cv::Mat cmpPatch = getPatch(dst, cmpRow, cmpCol);
            double cmpDist = dist(dstPatch, cmpPatch);
            if (cmpDist < patchDist)
            {
                patchDist = cmpDist;
                offset[0] = cmpRow - row;
                offset[1] = cmpCol - col;
            }
        }

        ++attenuate;
    }
}

double InpaintingThread::dist(const cv::Mat &dst, const cv::Mat &src)
{
    double d = 0;

    for (int row = 0; row < dst.rows; ++row)
    {
        for (int col = 0; col < dst.cols; ++col)
        {
            const cv::Vec3b& dstPixel = dst.at<cv::Vec3b>(row, col);
            const cv::Vec3b& srcPixel = src.at<cv::Vec3b>(row, col);
            for (int channel = 0; channel < 3; ++channel)
            {
                d += doubleSqr(dstPixel[channel] - srcPixel[channel]);
            }
        }
    }

    return d;
}

double InpaintingThread::getDelta(double *dists, int length)
{
    double* d = new double[length];
    for (int i = 0; i < length; ++i)
    {
        d[i] = dists[i];
    }

    // bubble sort
    for (int i = 0; i < length - 1; ++i)
    {
        for (int j = 0; j < length - 1 - i; ++j)
        {
            if (d[j] > d[j + 1])
            {
                double tmp = d[j];
                d[j] = d[j + 1];
                d[j + 1] = tmp;
            }
        }
    }

    double result = d[int(length * 0.75)];
    delete [] d;
    return result;
}
