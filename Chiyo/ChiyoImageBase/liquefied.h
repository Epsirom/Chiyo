#ifndef LIQUEFIED_H
#define LIQUEFIED_H

#include <QDialog>
#include <opencv2/opencv.hpp>
#include "chiyoimagebase.h"

namespace Ui {
class Liquefied;
}

class Liquefied : public QDialog
{
    Q_OBJECT

public:
    explicit Liquefied(QWidget *parent = 0);
    ~Liquefied();

private slots:
    void on_radius_valueChanged(int value);

    void on_Liquefied_accepted();

    void on_Liquefied_rejected();

private:
    Ui::Liquefied *ui;
};

#endif // LIQUEFIED_H
