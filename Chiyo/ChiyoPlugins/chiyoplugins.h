#ifndef CHIYOPLUGINS_H
#define CHIYOPLUGINS_H

#include "chiyoplugins_global.h"
#include <QPixmap>
#include <QThread>
#include <QString>
#include <QMap>
#include <QAction>

#include <QMetaType>

typedef struct PluginAction
{
    QMap<QString, QString> setting;
    QAction* action;
} PluginAction;

Q_DECLARE_METATYPE(PluginAction)

class ChiyoPluginsLoader;

typedef void (*installPluginActionFunc)(PluginAction p);

class CHIYOPLUGINSSHARED_EXPORT ChiyoPlugins
{
public:
    ChiyoPlugins();
    ~ChiyoPlugins();
    void loadPlugins();

    ChiyoPluginsLoader* get_loader();

private:
    ChiyoPluginsLoader* loader;

};

#endif // CHIYOPLUGINS_H
