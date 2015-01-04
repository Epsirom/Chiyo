#include "inpainting.h"
#include "ui_inpainting.h"
#include "inpaintingthread.h"

Inpainting::Inpainting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Inpainting)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    thread = new InpaintingThread;
    connect(thread, &InpaintingThread::finished, this, &Inpainting::inpaintingFinished);
}

Inpainting::~Inpainting()
{
    delete ui;
    delete thread;
}

int Inpainting::exec()
{
    inpainting();
    return QDialog::exec();
}

void Inpainting::inpainting()
{
    logInfo("Start inpainting...");
    timestamp = cv::getTickCount();
    thread->start();
}

void Inpainting::inpaintingFinished()
{
    logInfo(tr("Finished inpainting in %1 s.").arg(((double)cv::getTickCount() - timestamp) / cv::getTickFrequency()));
    accept();
}
