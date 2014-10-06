#include "chiyoeditor.h"
#include "ui_chiyoeditor.h"

#include "chiyogui_global.h"

#include <QSplitter>
#include <QDebug>

#include <QFileInfo>

ChiyoEditor* ChiyoEditor::instance(QWidget *parent)
{
    static int _editor_id = -1;
    return new ChiyoEditor(++_editor_id, parent);
}

ChiyoEditor::ChiyoEditor(int eid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChiyoEditor),
    editor_id(eid)
{
    ui->setupUi(this);

    ui->splitterMain->setStretchFactor(0, 3);
    ui->splitterMain->setStretchFactor(1, 2);
}

ChiyoEditor::~ChiyoEditor()
{
    delete ui;
}

bool ChiyoEditor::loadImage(const QString &filename)
{
    QPixmap pic;
    if (!pic.load(filename))
    {
        return false;
    }
    ui->controls->setImage(pic);
    current_file = filename;
    setWindowTitleByFileName(filename);
    appendLog(tr("Image loaded: %1").arg(filename), 0);
    return true;
}

void ChiyoEditor::setWindowTitleByFileName(const QString &filename)
{
    auto fileInfo = QFileInfo(filename);
    setWindowTitle(QString("%1 - %2").arg(fileInfo.fileName()).arg(filename));
}

void ChiyoEditor::appendLog(const QString &log, int type)
{
    ui->console->appendLog(log, type);
}

QImage ChiyoEditor::getImage()
{
    return ui->controls->getImage();
}
