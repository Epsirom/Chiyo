#ifndef CHIYOLOADER_H
#define CHIYOLOADER_H

#include <QWidget>

#include <QPoint>
#include <QImage>

#include "mainwindow.h"
#include "chiyoplugins.h"
#include "chiyopluginsloader.h"

namespace Ui {
class ChiyoLoader;
}

class ChiyoLoader : public QWidget
{
    Q_OBJECT

public:
    explicit ChiyoLoader(QWidget *parent = 0);
    ~ChiyoLoader();

    static ChiyoLoader* instance(QWidget *parent = 0);

    void installPluginAction(PluginAction pa);

signals:
    void pluginAction(PluginAction pa);

protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);

public slots:

private slots:
    void setTotalPlugins(int total);
    void setCurrentLoadingPlugin(QString name);
    void finishLoader();
    void showLoadingError(QString title, QString text);

private:
    Ui::ChiyoLoader *ui;
    ChiyoPlugins* plugins_manager;

    QPoint oldPos;
    QMovie *movie;

    MainWindow* mainWindow;

    int total_plugins;
    int loaded_plugins;
};

#endif // CHIYOLOADER_H
