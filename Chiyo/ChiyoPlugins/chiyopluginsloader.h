#ifndef CHIYOPLUGINSLOADER_H
#define CHIYOPLUGINSLOADER_H

#include <QThread>
#include "chiyoplugins.h"
#include <QFileInfoList>

#include <QList>


class ChiyoPluginsLoader : public QThread
{
    Q_OBJECT
public:
    explicit ChiyoPluginsLoader(QObject *parent = 0);

    void setInstallPluginActionFunc(installPluginActionFunc f);

protected:
    void run();

signals:
    void setTotalPlugins(int);
    void currentPluginChanged(QString);
    void allPluginsLoaded();
    void loadingError(QString title, QString text);

    void pluginAction(PluginAction pa);

public slots:

private:
    QFileInfoList getTargetPluginFiles();
    void loadPlugin(QFileInfo plugin);

    // Failure cases
    void fail_load_library(QString filename);
    void fail_invalid_chiyo_plugin(QString filename);

    installPluginActionFunc installPluginAction;
};

#endif // CHIYOPLUGINSLOADER_H
