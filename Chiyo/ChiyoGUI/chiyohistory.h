#ifndef CHIYOHISTORY_H
#define CHIYOHISTORY_H

#include <QWidget>

namespace Ui {
class ChiyoHistory;
}

class ChiyoHistory : public QWidget
{
    Q_OBJECT

public:
    explicit ChiyoHistory(QWidget *parent = 0);
    ~ChiyoHistory();

private:
    Ui::ChiyoHistory *ui;
};

#endif // CHIYOHISTORY_H
