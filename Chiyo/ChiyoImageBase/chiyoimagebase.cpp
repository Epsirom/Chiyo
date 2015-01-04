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
#include "saturation.h"
#include "sharpen.h"
#include "liquefied.h"
#include "lomo.h"
#include "beautiful.h"
#include "inpainting.h"

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
    list.append(action_saturation());
    list.append(action_sharpen());
    list.append(action_liquefied());
    list.append(action_beautiful());
    list.append(action_lomo());
    list.append(action_inpainting());
    return list;
}

QImage cvMatToQImage( const cv::Mat &inMat )
{
    switch ( inMat.type() )
    {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image( (const uchar*)(inMat.data), inMat.cols, inMat.rows, inMat.step, QImage::Format_ARGB32 );

        return image;
    }

        // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888 );

        return image.rgbSwapped();
    }

        // 8-bit, 1 channel
    case CV_8UC1:
    {
        static QVector<QRgb>  sColorTable;

        // only create our color table once
        if ( sColorTable.isEmpty() )
        {
            for ( int i = 0; i < 256; ++i )
                sColorTable.push_back( qRgb( i, i, i ) );
        }

        QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_Indexed8 );

        image.setColorTable( sColorTable );

        return image;
    }

    default:
        qWarning() << "Chiyo::Converter::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
        break;
    }

    return QImage();
}

QPixmap cvMatToQPixmap( const cv::Mat &inMat )
{
    return QPixmap::fromImage( cvMatToQImage( inMat ) );
}

int intMin(int a, int b)
{
    return a < b ? a : b;
}

int intMax(int a, int b)
{
    return a < b ? b : a;
}

int intAbs(int a)
{
    return a < 0 ? -a: a;
}

int intSqr(int a)
{
    return a * a;
}

double doubleSqr(double a)
{
    return a * a;
}

double doubleMin(double a, double b)
{
    return a < b ? a : b;
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

PluginAction action_saturation()
{
    static QAction* action = 0;
    if (!action)
    {
        action = new QAction(QIcon(":/saturation/icon"), tr("Adjust Saturation"), 0);
        QObject::connect(action, &QAction::triggered, action_saturation_triggered);
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

void action_saturation_triggered()
{
    Saturation saturation;
    saturation.exec();
}

PluginAction action_sharpen()
{
    static QAction* action = 0;
    if (!action)
    {
        action = new QAction(QIcon(":/sharpen/icon"), tr("Sharpen"), 0);
        QObject::connect(action, &QAction::triggered, action_sharpen_triggered);
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

void action_sharpen_triggered()
{
    setImage(auto_sharpen(getImage()));
}

PluginAction action_liquefied()
{
    static QAction* action = 0;
    if (!action)
    {
        action = new QAction(QIcon(":/liquefied/icon"), tr("Liquefied"), 0);
        QObject::connect(action, &QAction::triggered, action_liquefied_triggered);
    }
    PluginAction pa = {
        QMap<QString, QString>(),
        action
    };
    pa.setting["level_1_menu"] = tr("Enhancement");
    pa.setting["add_to_left_toolbox"] = QString("False");
    pa.setting["add_to_top_toolbox"] = QString("True");
    return pa;
}

void action_liquefied_triggered()
{
    Liquefied liquefied;
    liquefied.exec();
}

PluginAction action_beautiful()
{
    static QAction* action = 0;
    if (!action)
    {
        action = new QAction(QIcon(":/beautiful/icon"), tr("Beautiful Face"), 0);
        QObject::connect(action, &QAction::triggered, action_beautiful_triggered);
    }
    PluginAction pa = {
        QMap<QString, QString>(),
        action
    };
    pa.setting["level_1_menu"] = tr("Enhancement");
    pa.setting["add_to_left_toolbox"] = QString("True");
    pa.setting["add_to_top_toolbox"] = QString("False");
    return pa;
}

void action_beautiful_triggered()
{
    setImage(auto_beautiful_face(getImage()));
}

PluginAction action_lomo()
{
    static QAction* action = 0;
    if (!action)
    {
        action = new QAction(QIcon(":/lomo/icon"), tr("Lomo"), 0);
        QObject::connect(action, &QAction::triggered, action_lomo_triggered);
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

void action_lomo_triggered()
{
    Lomo lomo;
    lomo.exec();
}

PluginAction action_inpainting()
{
    static QAction* action = 0;
    if (!action)
    {
        action = new QAction(QIcon(":/inpainting/icon"), tr("Inpainting"), 0);
        QObject::connect(action, &QAction::triggered, action_inpainting_triggered);
    }
    PluginAction pa = {
        QMap<QString, QString>(),
        action
    };
    pa.setting["level_1_menu"] = tr("Enhancement");
    pa.setting["add_to_left_toolbox"] = QString("True");
    pa.setting["add_to_top_toolbox"] = QString("False");
    return pa;
}

void action_inpainting_triggered()
{
    Inpainting inpainting;
    inpainting.exec();
}
