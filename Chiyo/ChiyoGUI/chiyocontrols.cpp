#include "chiyocontrols.h"
#include "ui_chiyocontrols.h"

#include <QInputDialog>
#include <QMessageBox>

ChiyoControls::ChiyoControls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChiyoControls),
    scale(1.0)
{
    ui->setupUi(this);

    connect(ui->graphicsView, &ChiyoGraphics::pinchZoom, this, &ChiyoControls::pinch_zoom);

    syncScale();
}

ChiyoControls::~ChiyoControls()
{
    delete ui;
}

void ChiyoControls::setImage(QPixmap &pic)
{
    ui->graphicsView->setImage(pic);
}

void ChiyoControls::syncScale()
{
    if (scale >= 5) {
        scale = 5;
        ui->action_zoom_in->setEnabled(false);
    } else {
        ui->action_zoom_in->setEnabled(true);
    }
    if (scale <= 0.1) {
        scale = 0.1;
        ui->action_zoom_out->setEnabled(false);
    } else {
        ui->action_zoom_out->setEnabled(true);
    }
    ui->action_zoom_original->setEnabled(scale != 1.0);

    ui->graphicsView->setScale(scale, scale);
    ui->action_scale->setText(QString("%1%").arg(scale * 100, 0, 'f', 2));
}

void ChiyoControls::on_action_zoom_in_triggered()
{
    scale += 0.1;
    syncScale();
}

void ChiyoControls::on_action_zoom_out_triggered()
{
    scale -= 0.1;
    syncScale();
}

void ChiyoControls::on_action_zoom_original_triggered()
{
    scale = 1.0;
    syncScale();
}

void ChiyoControls::on_action_zoom_fit_triggered()
{
    scale = ui->graphicsView->getFitScale() - 0.1;
    syncScale();
}

void ChiyoControls::on_action_scale_triggered()
{
    bool ok;
    double user_input = QInputDialog::getDouble(this, tr("Set scale"),
                                 tr("Input a number between 10 and 500:"), scale * 100, 10, 500, 2, &ok);
    if (ok)
    {
        scale = user_input / 100;
        syncScale();
    }
}

void ChiyoControls::pinch_zoom(qreal factor)
{
    scale *= factor;
    syncScale();
}
