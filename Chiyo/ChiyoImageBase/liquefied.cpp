#include "liquefied.h"
#include "ui_liquefied.h"

Liquefied::Liquefied(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Liquefied)
{
    ui->setupUi(this);

    ui->graphicsView->setImage(getImage());
}

Liquefied::~Liquefied()
{
    delete ui;
}

void Liquefied::on_radius_valueChanged(int value)
{
    ui->radius_text->setText(QString("%1").arg(value));
    ui->graphicsView->setRadius(value);
}

void Liquefied::on_Liquefied_accepted()
{
    setImage(ui->graphicsView->getCurrent());
    logInfo(tr("Liquefied!"));
}

void Liquefied::on_Liquefied_rejected()
{
    logInfo(tr("Liquefying canceled."));
}
