#include "chiyoplugins.h"

#include <QMessageBox>
#include "chiyopluginsloader.h"

#include "chiyoconverter.h"
#include <opencv2/opencv.hpp>

cv::Mat getCvMat()
{
    return Chiyo::Converter::QImageToCvMat(ChiyoPlugins::instance()->get_qimage());
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
    loader->start();
}

ChiyoPluginsLoader* ChiyoPlugins::get_loader()
{
    return loader;
}

void ChiyoPlugins::setImageInterface(getImageFunc get_func)
{
    get_qimage = get_func;
    loader->setGetImageFunc(getCvMat);
}
