#ifndef CHIYOPLUGINSLOADER_H
#define CHIYOPLUGINSLOADER_H

#include <QThread>
#include "chiyoplugins.h"
#include <QFileInfoList>

#include <QList>

namespace cv
{
class Mat;
}

typedef cv::Mat (*getCvMatFunc)();
typedef void (*setCvMatFunc)(cv::Mat);

class ChiyoPluginsLoader : public QThread
{
    Q_OBJECT
public:
    explicit ChiyoPluginsLoader(QObject *parent = 0);

    void setInstallPluginActionFunc(installPluginActionFunc f);
    void setImageFuncs(getCvMatFunc f, setCvMatFunc f2);
    void setLogFuncs(appendLogFunc info_func, appendLogFunc warning_func, appendLogFunc error_func);

    QString application_dir;

protected:
    void run();

signals:
    void setTotalPlugins(int);
    void currentPluginChanged(QString);
    void allPluginsLoaded();
    void loadingError(QString title, QString text);

    void pluginAction(PluginAction pa);

public slots:

private:
    QFileInfoList getTargetPluginFiles();
    void loadPlugin(QFileInfo plugin);

    // Failure cases
    void fail_load_library(QString filename);
    void fail_invalid_chiyo_plugin(QString filename);

    installPluginActionFunc installPluginAction;

    getCvMatFunc get_img;
    setCvMatFunc set_img;
    appendLogFunc log_info;
    appendLogFunc log_warning;
    appendLogFunc log_error;

    // loading status
    QFileInfoList plugins_toload;
};

#endif // CHIYOPLUGINSLOADER_H
