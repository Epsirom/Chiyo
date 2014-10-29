#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <Qsci/qsciscintilla.h>


#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>
#include <QMdiSubWindow>

MainWindow* MainWindow::instance(QWidget *parent)
{
    static MainWindow * _instance = 0;
    if (_instance == 0)
    {
        _instance = new MainWindow(parent);
    }
    return _instance;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this, &MainWindow::appendLogSignal, this, &MainWindow::appendLog, Qt::QueuedConnection);

    connect(ui->mdiArea, &QMdiArea::subWindowActivated, this, &MainWindow::updateMenus);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_open_image_triggered()
{
    QString fileName = QFileDialog::getOpenFileName
        (this, tr("open file"), NULL, tr(AVAILABLE_OPEN_FILE_TYPES));

    if(!fileName.isEmpty())
    {
        QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();
        auto child = ChiyoEditor::instance();
        ui->mdiArea->addSubWindow(child);
        if(child->loadImage(canonicalFilePath))
        {
            child->setVisible(true);
        }
        else
        {
            child->close();
        }
    }
}

void MainWindow::appendInfo(const QString &log)
{
    emit appendLogSignal(log, CHIYO_LOG_INFO);
}

void MainWindow::appendWarning(const QString &log)
{
    emit appendLogSignal(log, CHIYO_LOG_WARNING);
}

void MainWindow::appendError(const QString &log)
{
    emit appendLogSignal(log, CHIYO_LOG_ERROR);
}

void MainWindow::appendLog(const QString &log, int type)
{
    auto child = activeMdiChild();
    if (!child)
    {
        qDebug() << "No image opened!" << log;
        return;
    }
    child->appendLog(log, type);
}

QImage MainWindow::getImage()
{
    auto child = activeMdiChild();
    if (!child)
    {
        qDebug() << "No image opened!";
        return QImage();
    }
    return child->getImage();
}

void MainWindow::setImage(QImage img)
{
    auto child = activeMdiChild();
    if (!child)
    {
        qDebug() << "No image opened!";
        return;
    }
    child->setImage(img);
}

ChiyoEditor* MainWindow::activeMdiChild()
{
    if (QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow())
        return qobject_cast<ChiyoEditor *>(activeSubWindow->widget());
    return NULL;
}

void MainWindow::installPluginAction(PluginAction pa)
{
    auto & setting = pa.setting;

    if (setting.contains("level_1_menu"))
    {
        QString level_1_menu = setting["level_1_menu"];
        QMenu* level_1_menu_ptr = 0;

        QMenuBar* menubar = menuBar();

        QList<QMenu*> menus = menubar->findChildren<QMenu*>();
        for (QMenu* menu : menus)
        {
            if (menu->title() == level_1_menu)
            {
                level_1_menu_ptr = menu;
                break;
            }
        }
        if (!level_1_menu_ptr)
        {
            level_1_menu_ptr = menubar->addMenu(level_1_menu);
        }

        QAction* menu_action = new QAction(this);
        menu_action->setText(pa.action->text());
        menu_action->setShortcut(pa.action->shortcut());
        menu_action->setIcon(pa.action->icon());
        menu_action->setToolTip(pa.action->toolTip());
        connect(menu_action, &QAction::triggered, [=](){ pa.action->trigger(); });
        level_1_menu_ptr->addAction(menu_action);
        pluginActions.append(menu_action);
    }

    if (setting.contains("add_to_left_toolbox") && setting["add_to_left_toolbox"] == "True")
    {
        ui->leftToolBar->addAction(pa.action);
    }
    if (setting.contains("add_to_top_toolbox") && setting["add_to_top_toolbox"] == "True")
    {
        ui->mainToolBar->addAction(pa.action);
    }
    pluginActions.append(pa.action);
    updateMenus();
    emit pluginActionInstalled();
}

void MainWindow::updateMenus()
{
    bool has_child = (activeMdiChild() != 0);
    for (QAction* action : pluginActions)
    {
        action->setEnabled(has_child);
    }
    ui->action_save_as->setEnabled(has_child);
}

void MainWindow::on_action_save_as_triggered()
{
    auto child = activeMdiChild();
    if (child)
    {
        child->saveAs();
    }
}
