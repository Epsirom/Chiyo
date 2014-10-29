#include "chiyoplugins.h"

#include <QMessageBox>
#include "chiyopluginsloader.h"

#include "chiyoconverter.h"
#include <opencv2/opencv.hpp>

#include <QImage>

cv::Mat getCvMat()
{
    QImage img = ChiyoPlugins::instance()->get_qimage();
    if (img.isNull())
        return cv::Mat();
    return Chiyo::Converter::QImageToCvMat(img);
}

void setCvMat(cv::Mat mat)
{
    ChiyoPlugins::instance()->set_qimage(Chiyo::Converter::cvMatToQImage(mat));
}

ChiyoPlugins* ChiyoPlugins::instance()
{
    static ChiyoPlugins* _instance = 0;
    if (!_instance)
    {
        _instance = new ChiyoPlugins();
    }
    return _instance;
}

ChiyoPlugins::ChiyoPlugins()
{
    loader = new ChiyoPluginsLoader;
}

ChiyoPlugins::~ChiyoPlugins()
{
    delete loader;
}

void ChiyoPlugins::loadPlugins()
{
    loader->wait();
    loader->start();
}

ChiyoPluginsLoader* ChiyoPlugins::get_loader()
{
    return loader;
}

void ChiyoPlugins::setImageInterfaces(getImageFunc get_func, setImageFunc set_func)
{
    get_qimage = get_func;
    set_qimage = set_func;
    loader->setImageFuncs(getCvMat, setCvMat);
}

void ChiyoPlugins::setLogInterfaces(appendLogFunc info_func, appendLogFunc warning_func, appendLogFunc error_func)
{
    log_info = info_func;
    log_warning = warning_func;
    log_error = error_func;
    loader->setLogFuncs(info_func, warning_func, error_func);
}
