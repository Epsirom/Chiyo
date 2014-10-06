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

class CHIYOPLUGINSSHARED_EXPORT ChiyoPlugins
{
public:
    ChiyoPlugins();
    ~ChiyoPlugins();
    static ChiyoPlugins* instance();

    void loadPlugins();
    void setImageInterface(getImageFunc get_func);

    ChiyoPluginsLoader* get_loader();

    getImageFunc get_qimage;

private:
    ChiyoPluginsLoader* loader;


};

#endif // CHIYOPLUGINS_H
