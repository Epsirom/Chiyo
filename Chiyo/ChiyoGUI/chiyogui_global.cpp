#include "chiyogui_global.h"
#include "mainwindow.h"

#define _CHIYO_MAIN MainWindow::instance()
#define _CHIYO_LOG_TEMPLATE(type) MainWindow::instance()->append##type(log)

void chiyoLog(const QString &log)
{
    _CHIYO_LOG_TEMPLATE(Info);
}

void chiyoWarning(const QString &log)
{
    _CHIYO_LOG_TEMPLATE(Warning);
}

void chiyoError(const QString &log)
{
    _CHIYO_LOG_TEMPLATE(Error);
}

