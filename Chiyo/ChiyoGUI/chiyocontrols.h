#ifndef CHIYOCONTROLS_H
#define CHIYOCONTROLS_H

#include <QWidget>
#include <QPixmap>
#include <QImage>

namespace Ui {
class ChiyoControls;
}

class ChiyoControls : public QWidget
{
    Q_OBJECT

public:
    explicit ChiyoControls(QWidget *parent = 0);
    ~ChiyoControls();

    void setImage(QPixmap &pic);
    QImage getImage();
    bool saveImage(QString &path);

private slots:

    void on_action_zoom_in_triggered();

    void on_action_zoom_out_triggered();

    void on_action_zoom_original_triggered();

    void on_action_zoom_fit_triggered();

    void on_action_scale_triggered();

    void pinch_zoom(qreal factor);

private:
    Ui::ChiyoControls *ui;

    void syncScale();

    qreal scale;
};

#endif // CHIYOCONTROLS_H
