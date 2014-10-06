#include "chiyoloader.h"
#include "ui_chiyoloader.h"

#include <QMouseEvent>

#include <QMovie>
#include <QDebug>
#include <QMessageBox>

#include <QApplication>
#include <QDesktopWidget>

void __installPluginAction(PluginAction pa)
{
    MainWindow::instance()->installPluginAction(pa);
}

QImage __getImageInterface()
{
    return ChiyoLoader::getImage();
}

QImage ChiyoLoader::getImage()
{
    return MainWindow::instance()->activeMdiChild()->getImage();
}

ChiyoLoader* ChiyoLoader::instance(QWidget *parent)
{
    static ChiyoLoader* _instance = 0;
    if (!_instance)
    {
        _instance = new ChiyoLoader(parent);
    }
    return _instance;
}

ChiyoLoader::ChiyoLoader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChiyoLoader),
    total_plugins(0),
    loaded_plugins(0)
{
    plugins_manager = ChiyoPlugins::instance();
    plugins_manager->setImageInterface(__getImageInterface);

    ui->setupUi(this);
    ui->loadingErrors->setVisible(false);
    ui->loadingErrors->setStyleSheet("background: url(:/loadingError)");
    mainWindow = MainWindow::instance(this);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    movie = new QMovie(":/loading");
    movie->setScaledSize(QSize(320, 320));

    ui->loading->setMovie(movie);
    movie->start();

    ChiyoPluginsLoader* pl = plugins_manager->get_loader();
    connect(pl, &ChiyoPluginsLoader::setTotalPlugins, this, &ChiyoLoader::setTotalPlugins, Qt::QueuedConnection);
    connect(pl, &ChiyoPluginsLoader::currentPluginChanged, this, &ChiyoLoader::setCurrentLoadingPlugin, Qt::QueuedConnection);
    connect(pl, &ChiyoPluginsLoader::allPluginsLoaded, this, &ChiyoLoader::finishLoader, Qt::QueuedConnection);
    connect(pl, &ChiyoPluginsLoader::loadingError, this, &ChiyoLoader::showLoadingError, Qt::QueuedConnection);

    pl->setInstallPluginActionFunc(__installPluginAction);

    connect(pl, &ChiyoPluginsLoader::pluginAction, mainWindow, &MainWindow::installPluginAction, Qt::QueuedConnection);

    plugins_manager->loadPlugins();
}

ChiyoLoader::~ChiyoLoader()
{
    delete ui;
    delete mainWindow;
    delete movie;
}

void ChiyoLoader::mousePressEvent(QMouseEvent *e)
{
    oldPos = e->globalPos();
    auto &localPos = e->localPos();
    // sync pos.
    move(oldPos.x() - localPos.x(), oldPos.y() - localPos.y());
}

void ChiyoLoader::mouseMoveEvent(QMouseEvent *e)
{
    const QPoint delta = e->globalPos() - oldPos;
    move(x() + delta.x(), y() + delta.y());
    oldPos = e->globalPos();
}

void ChiyoLoader::setTotalPlugins(int total)
{
    total_plugins = total;
    ui->progressBar->setMaximum(total + 1);
}

void ChiyoLoader::finishLoader()
{
    ui->progressBar->setValue(total_plugins + 1);
    ui->progressBarText->setText(tr("Loading main window..."));
    movie->stop();
    mainWindow->show();
    this->hide();
}

void ChiyoLoader::setCurrentLoadingPlugin(QString name)
{
    ++loaded_plugins;
    ui->progressBar->setValue(loaded_plugins);
    ui->progressBarText->setText(tr("(%1/%2)Loading %3").arg(loaded_plugins).arg(total_plugins).arg(name));
}

void ChiyoLoader::showLoadingError(QString title, QString text)
{
    //qDebug() << tr("[%1] %2").arg(title).arg(text);
    ui->loadingErrors->setVisible(true);
    ui->loadingErrors->appendPlainText(tr("[%1] %2").arg(title).arg(text));
    ui->loadingErrors->moveCursor(QTextCursor::End);
}

