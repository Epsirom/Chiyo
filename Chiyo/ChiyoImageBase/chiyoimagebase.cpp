#include "chiyoimagebase.h"
#include <QString>
#include <QObject>
#include <QList>
#include <QMap>
#include <QKeySequence>
#include <QDebug>
#include "vignette.h"
#include "white_balance.h"
#include "median.h"
#include "gaussianblur.h"

#define tr QObject::tr

getCvMatFunc getImage;
setCvMatFunc setImage;
appendLogFunc logInfo;
appendLogFunc logWarning;
appendLogFunc logError;

void install_image_interfaces(getCvMatFunc get_img, setCvMatFunc set_img)
{
    getImage = get_img;
    setImage = set_img;
}

void install_log_interfaces(appendLogFunc info_func, appendLogFunc warning_func, appendLogFunc error_func)
{
    logInfo = info_func;
    logWarning = warning_func;
    logError = error_func;
}

QList<PluginAction> initialize_plugin_actions()
{
    QList<PluginAction> list;
    list.append(action_vignette());
    list.append(action_white_balance());
    list.append(action_median());
    list.append(action_gaussian_blur());
    return list;
}

PluginAction action_vignette()
{
    static QAction* action = 0;
    if (!action)
    {
        action = new QAction(QIcon(":/vignette/icon"), tr("Vignette"), 0);
        QObject::connect(action, &QAction::triggered, action_vignette_triggered);
    }
    PluginAction pa = {
        QMap<QString, QString>(),
        action
    };
    pa.setting["level_1_menu"] = tr("Filter");
    pa.setting["add_to_left_toolbox"] = QString("False");
    pa.setting["add_to_top_toolbox"] = QString("True");
    return pa;
}

void action_vignette_triggered()
{
    Vignette vignette;
    vignette.exec();
}

PluginAction action_white_balance()
{
    static QAction* action = 0;
    if (!action)
    {
        action = new QAction(QIcon(":/white_balance/icon"), tr("White Balance"), 0);
        QObject::connect(action, &QAction::triggered, action_white_balance_triggered);
    }
    PluginAction pa = {
        QMap<QString, QString>(),
        action
    };
    pa.setting["level_1_menu"] = tr("Filter");
    pa.setting["add_to_left_toolbox"] = QString("True");
    pa.setting["add_to_top_toolbox"] = QString("False");
    return pa;
}

void action_white_balance_triggered()
{
    setImage(auto_adjust_white_balance(getImage()));
}

PluginAction action_median()
{
    static QAction* action = 0;
    if (!action)
    {
        action = new QAction(QIcon(":/median/icon"), tr("Median"), 0);
        QObject::connect(action, &QAction::triggered, action_median_triggered);
    }
    PluginAction pa = {
        QMap<QString, QString>(),
        action
    };
    pa.setting["level_1_menu"] = tr("Filter");
    pa.setting["add_to_left_toolbox"] = QString("True");
    pa.setting["add_to_top_toolbox"] = QString("False");
    return pa;
}

void action_median_triggered()
{
    setImage(apply_median(getImage(), 2));
}

PluginAction action_gaussian_blur()
{
    static QAction* action = 0;
    if (!action)
    {
        action = new QAction(QIcon(":/gaussian_blur/icon"), tr("Gaussian Blur"), 0);
        QObject::connect(action, &QAction::triggered, action_gaussian_blur_triggered);
    }
    PluginAction pa = {
        QMap<QString, QString>(),
        action
    };
    pa.setting["level_1_menu"] = tr("Filter");
    pa.setting["add_to_left_toolbox"] = QString("False");
    pa.setting["add_to_top_toolbox"] = QString("True");
    return pa;
}

void action_gaussian_blur_triggered()
{
    GaussianBlur gaussian;
    gaussian.exec();
}
