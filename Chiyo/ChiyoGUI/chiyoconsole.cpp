#include "chiyoconsole.h"
#include "ui_chiyoconsole.h"
#include <QTime>

#include <QDebug>

ChiyoConsole::ChiyoConsole(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChiyoConsole)
{
    ui->setupUi(this);

    ui->chiyoLog->setWordWrapMode(QTextOption::WrapAnywhere);
}

ChiyoConsole::~ChiyoConsole()
{
    delete ui;
}

void ChiyoConsole::on_aciton_clear_logs_triggered()
{
    ui->chiyoLog->clear();
}

void ChiyoConsole::appendLog(const QString &log, int type)
{
    static const QString colors[] = {"black", "orange", "red"};
                                     // INFO, WARNING, ERROR
    QString color = colors[int(type)];

    ui->chiyoLog->appendHtml(QString("<span style='color:%1'>[%2] %3</span>")
                             .arg(color).arg(QTime::currentTime().toString(QString("hh:mm:ss.zzz"))).arg(log));
    ui->chiyoLog->moveCursor(QTextCursor::End);
}
