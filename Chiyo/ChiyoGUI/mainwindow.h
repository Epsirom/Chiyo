#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

#include "chiyogui_global.h"
#include "chiyoeditor.h"
#include "chiyoplugins.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static MainWindow* instance(QWidget *parent = 0);

    void appendInfo(const QString &log);
    void appendWarning(const QString &log);
    void appendError(const QString &log);

    QImage getImage();
    void setImage(QImage img);

public slots:
    void installPluginAction(PluginAction pa);
    void updateMenus();

signals:
    void appendLogSignal(const QString &log, int type);
    void pluginActionInstalled();

private slots:
    void on_action_open_image_triggered();

    void on_action_save_as_triggered();

private:
    void appendLog(const QString &log, int type = CHIYO_LOG_INFO);
    ChiyoEditor * activeMdiChild();

private:
    Ui::MainWindow *ui;
    QList<QAction*> pluginActions;

    friend class ChiyoLoader;
};

#endif // MAINWINDOW_H
