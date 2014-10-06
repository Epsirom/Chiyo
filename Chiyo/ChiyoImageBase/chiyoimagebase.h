#ifndef CHIYOIMAGEBASE_H
#define CHIYOIMAGEBASE_H

#include "chiyoimagebase_global.h"

#include <QAction>
#include <QMap>
#include <QList>

#include <opencv2/opencv.hpp>

typedef cv::Mat (*getCvMatFunc)();

extern "C" CHIYOIMAGEBASESHARED_EXPORT void install_image_interfaces(getCvMatFunc get_img);

typedef struct PluginAction
{
    QMap<QString, QString> setting;
    QAction* action;
} PluginAction;

PluginAction action_vignette();
void action_vignette_triggered();

extern "C" CHIYOIMAGEBASESHARED_EXPORT QList<PluginAction> initialize_plugin_actions();

//extern const char* CHIYOIMAGEBASESHARED_EXPORT plugin_actions_name;

#endif // CHIYOIMAGEBASE_H
