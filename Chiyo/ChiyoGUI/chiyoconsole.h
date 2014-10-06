#ifndef CHIYOCONSOLE_H
#define CHIYOCONSOLE_H

#include <QWidget>

namespace Ui {
class ChiyoConsole;
}

class ChiyoConsole : public QWidget
{
    Q_OBJECT

public:
    explicit ChiyoConsole(QWidget *parent = 0);
    ~ChiyoConsole();

    void appendLog(const QString &log, int type);

private slots:
    void on_aciton_clear_logs_triggered();

private:
    Ui::ChiyoConsole *ui;
};

#endif // CHIYOCONSOLE_H
