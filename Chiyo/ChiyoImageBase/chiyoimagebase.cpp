#include "chiyoimagebase.h"
#include <QString>
#include <QObject>
#include <QList>
#include <QMap>
#include <QKeySequence>
#include <QDebug>

#define tr QObject::tr

QList<PluginAction> initialize_plugin_actions()
{
    QList<PluginAction> list;
    list.append(action_vignette());
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
    pa.setting["add_to_left_toolbox"] = QString("True");
    pa.setting["add_to_top_toolbox"] = QString("True");
    return pa;
}

void action_vignette_triggered()
{
    qDebug() << "Vignette!";
}
