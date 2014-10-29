#ifndef CHIYOIMAGEBASE_H
#define CHIYOIMAGEBASE_H

#include "chiyoimagebase_global.h"

#include <QAction>
#include <QMap>
#include <QList>

#include <opencv2/opencv.hpp>

typedef cv::Mat (*getCvMatFunc)();
typedef void (*setCvMatFunc)(cv::Mat);
typedef void (*appendLogFunc)(QString);

extern getCvMatFunc getImage;
extern setCvMatFunc setImage;
extern appendLogFunc logInfo;
extern appendLogFunc logWarning;
extern appendLogFunc logError;

extern "C" CHIYOIMAGEBASESHARED_EXPORT void install_image_interfaces(getCvMatFunc get_img, setCvMatFunc set_img);
extern "C" CHIYOIMAGEBASESHARED_EXPORT void install_log_interfaces(appendLogFunc info_func, appendLogFunc warning_func, appendLogFunc error_func);

typedef struct PluginAction
{
    QMap<QString, QString> setting;
    QAction* action;
} PluginAction;

PluginAction action_vignette();
void action_vignette_triggered();
PluginAction action_white_balance();
void action_white_balance_triggered();
PluginAction action_median();
void action_median_triggered();
PluginAction action_gaussian_blur();
void action_gaussian_blur_triggered();

extern "C" CHIYOIMAGEBASESHARED_EXPORT QList<PluginAction> initialize_plugin_actions();

//extern const char* CHIYOIMAGEBASESHARED_EXPORT plugin_actions_name;

#endif // CHIYOIMAGEBASE_H
