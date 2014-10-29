#ifndef CHIYOPLUGINS_H
#define CHIYOPLUGINS_H

#include "chiyoplugins_global.h"
#include <QPixmap>
#include <QThread>
#include <QString>
#include <QMap>
#include <QAction>
#include <QImage>

#include <QMetaType>

typedef struct PluginAction
{
    QMap<QString, QString> setting;
    QAction* action;
} PluginAction;

Q_DECLARE_METATYPE(PluginAction)

class ChiyoPluginsLoader;

typedef void (*installPluginActionFunc)(PluginAction p);
typedef QImage (*getImageFunc)();
typedef void (*setImageFunc)(QImage);
typedef void (*appendLogFunc)(QString);

class CHIYOPLUGINSSHARED_EXPORT ChiyoPlugins
{
public:
    ChiyoPlugins();
    ~ChiyoPlugins();
    static ChiyoPlugins* instance();

    void loadPlugins();
    void setImageInterfaces(getImageFunc get_func, setImageFunc set_func);
    void setLogInterfaces(appendLogFunc info_func, appendLogFunc warning_func, appendLogFunc error_func);

    ChiyoPluginsLoader* get_loader();

    getImageFunc get_qimage;
    setImageFunc set_qimage;
    appendLogFunc log_info;
    appendLogFunc log_warning;
    appendLogFunc log_error;

private:
    ChiyoPluginsLoader* loader;


};

#endif // CHIYOPLUGINS_H
