#include "chiyohistory.h"
#include "ui_chiyohistory.h"

ChiyoHistory::ChiyoHistory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChiyoHistory)
{
    ui->setupUi(this);
}

ChiyoHistory::~ChiyoHistory()
{
    delete ui;
}
