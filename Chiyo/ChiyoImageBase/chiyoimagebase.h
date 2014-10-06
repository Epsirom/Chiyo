#ifndef CHIYOIMAGEBASE_H
#define CHIYOIMAGEBASE_H

#include "chiyoimagebase_global.h"

#include <QAction>
#include <QMap>
#include <QList>

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
