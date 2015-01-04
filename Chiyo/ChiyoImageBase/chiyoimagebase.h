#ifndef CHIYOIMAGEBASE_H
#define CHIYOIMAGEBASE_H

#include "chiyoimagebase_global.h"

#include <QAction>
#include <QMap>
#include <QList>
#include <QPixmap>

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

PluginAction action_saturation();
void action_saturation_triggered();
PluginAction action_sharpen();
void action_sharpen_triggered();
PluginAction action_liquefied();
void action_liquefied_triggered();

PluginAction action_beautiful();
void action_beautiful_triggered();
PluginAction action_lomo();
void action_lomo_triggered();

PluginAction action_inpainting();
void action_inpainting_triggered();

extern "C" CHIYOIMAGEBASESHARED_EXPORT QList<PluginAction> initialize_plugin_actions();

QImage cvMatToQImage( const cv::Mat &inMat );
QPixmap cvMatToQPixmap( const cv::Mat &inMat );

int intMin(int a, int b);
int intMax(int a, int b);
int intAbs(int a);
int intSqr(int a);
double doubleSqr(double a);
double doubleMin(double a, double b);

//extern const char* CHIYOIMAGEBASESHARED_EXPORT plugin_actions_name;

#endif // CHIYOIMAGEBASE_H
