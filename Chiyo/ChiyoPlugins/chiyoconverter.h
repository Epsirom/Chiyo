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
        cv::Mat QImageToCvMat( const QImage &inImage );
        cv::Mat QPixmapToCvMat( const QPixmap &inPixmap );
    }
}

#endif // CHIYOCONVERTER_H
