#include "chiyopluginsloader.h"

#include <QDir>
#include <QDebug>
#include <QLibrary>
#include <QMessageBox>
#include <QFunctionPointer>

#include <QCoreApplication>

ChiyoPluginsLoader::ChiyoPluginsLoader(QObject *parent) :
    QThread(parent)
{
}

void ChiyoPluginsLoader::setInstallPluginActionFunc(installPluginActionFunc f)
{
    installPluginAction = f;
}

void ChiyoPluginsLoader::setImageFuncs(getCvMatFunc f, setCvMatFunc f2)
{
    get_img = f;
    set_img = f2;
}

void ChiyoPluginsLoader::setLogFuncs(appendLogFunc info_func, appendLogFunc warning_func, appendLogFunc error_func)
{
    log_info = info_func;
    log_warning = warning_func;
    log_error = error_func;
}

void ChiyoPluginsLoader::run()
{
    QFileInfoList plugins = getTargetPluginFiles();
    emit setTotalPlugins(plugins.count());
    for (QFileInfo &plugin : plugins)
    {
        emit currentPluginChanged(plugin.fileName());
        loadPlugin(plugin.canonicalFilePath());
    }
    emit allPluginsLoaded();
}

QFileInfoList ChiyoPluginsLoader::getTargetPluginFiles()
{
    QDir dir(application_dir + CHIYO_PLUGINS_PATH);
    QStringList filters;
    // for mac
    filters << "*.dylib";
    return dir.entryInfoList(filters);
}

#define FAIL_LOAD_LIBRARY fail_load_library(plugin.fileName());return
#define FAIL_INVALID_CHIYO_PLUGIN fail_invalid_chiyo_plugin(plugin.fileName());return
#define CHIYO_PLUGIN_RESOLVE_DATA(name, param, data_type) \
    tmp_ptr = lib.resolve(#name); \
    if (!tmp_ptr) { FAIL_INVALID_CHIYO_PLUGIN; } \
    data_type param = *(data_type*)tmp_ptr
#define CHIYO_PLUGIN_RESOLVE_FUNC(name, func, func_type) \
    tmp_ptr = lib.resolve(#name); \
    if (!tmp_ptr) { FAIL_INVALID_CHIYO_PLUGIN; } \
    func_type func = (func_type)tmp_ptr

void ChiyoPluginsLoader::loadPlugin(QFileInfo plugin)
{
    QLibrary lib(plugin.canonicalFilePath());
    bool flag = lib.load();
    if (!flag)
    {
        FAIL_LOAD_LIBRARY;
    }

    QFunctionPointer tmp_ptr;
    typedef QList<PluginAction> (*InitializePluginActionsFunc)();
    CHIYO_PLUGIN_RESOLVE_FUNC(initialize_plugin_actions, initialize_plugin_actions, InitializePluginActionsFunc);
    QList<PluginAction> pas = initialize_plugin_actions();
    for (PluginAction &pa : pas)
    {
        //emit pluginAction(pa);
        installPluginAction(pa);
    }
    typedef void (*InstallImageInterfacesFunc)(getCvMatFunc, setCvMatFunc);
    CHIYO_PLUGIN_RESOLVE_FUNC(install_image_interfaces, install_image_interfaces, InstallImageInterfacesFunc);
    install_image_interfaces(get_img, set_img);
    typedef void (*InstallLogInterfacesFunc)(appendLogFunc, appendLogFunc, appendLogFunc);
    CHIYO_PLUGIN_RESOLVE_FUNC(install_log_interfaces, install_log_interfaces, InstallLogInterfacesFunc);
    install_log_interfaces(log_info, log_warning, log_error);
}

void ChiyoPluginsLoader::fail_load_library(QString filename)
{
    emit loadingError(tr("Load plugin failed"),
                      tr("%1 is not a valid dynamic library.").arg(filename));
}

void ChiyoPluginsLoader::fail_invalid_chiyo_plugin(QString filename)
{
    emit loadingError(tr("Load plugin failed"),
                      tr("%1 is not a valid Chiyo plugin or is not compatible with current Chiyo version.").arg(filename));
}
