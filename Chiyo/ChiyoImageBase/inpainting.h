#ifndef INPAINTING_H
#define INPAINTING_H

#include <QDialog>
#include <opencv2/opencv.hpp>
#include "chiyoimagebase.h"
#include "inpaintingthread.h"

namespace Ui {
class Inpainting;
}

class Inpainting : public QDialog
{
    Q_OBJECT

public:
    explicit Inpainting(QWidget *parent = 0);
    ~Inpainting();

public slots:
    void inpainting();
    void inpaintingFinished();

    int exec();

private:
    Ui::Inpainting *ui;

    InpaintingThread* thread;

    double timestamp;
};

#endif // INPAINTING_H
