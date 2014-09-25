#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <Qsci/qsciscintilla.h>

#include "chiyographics.h"

#include "chiyoplugins.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_open_image_triggered()
{
    auto child = new ChiyoGraphics(this);
    ui->mdiArea->addSubWindow(child);
    child->show();
}
