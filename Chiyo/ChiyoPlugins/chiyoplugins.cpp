#include "chiyoplugins.h"

#include <QMessageBox>
#include "chiyopluginsloader.h"


ChiyoPlugins::ChiyoPlugins()
{
    loader = new ChiyoPluginsLoader;
}

ChiyoPlugins::~ChiyoPlugins()
{
    delete loader;
}

void ChiyoPlugins::loadPlugins()
{
    loader->start();
}

ChiyoPluginsLoader* ChiyoPlugins::get_loader()
{
    return loader;
}
