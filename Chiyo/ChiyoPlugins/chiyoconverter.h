#ifndef CHIYOCONVERTER_H
#define CHIYOCONVERTER_H

class QPixmap;
class QImage;
namespace cv
{
    class Mat;
}

namespace Chiyo
{
    namespace Converter
    {
        QImage cvMatToQImage( const cv::Mat &inMat );
        QPixmap cvMatToQPixmap( const cv::Mat &inMat );
        cv::Mat QImageToCvMat( const QImage &inImage, bool inCloneImageData = true );
        cv::Mat QPixmapToCvMat( const QPixmap &inPixmap, bool inCloneImageData = true );
    }
}

#endif // CHIYOCONVERTER_H
