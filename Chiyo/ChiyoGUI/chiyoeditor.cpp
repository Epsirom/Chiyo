#include "chiyoeditor.h"
#include "ui_chiyoeditor.h"

#include "chiyogui_global.h"

#include <QSplitter>
#include <QDebug>

#include <QFileInfo>
#include <QFileDialog>

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

    ui->history->setVisible(false);
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

bool ChiyoEditor::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save as"), current_file, tr(AVAILABLE_SAVE_FILE_TYPES));
    if (fileName.isEmpty())
    {
        return false;
    }
    if (!ui->controls->saveImage(fileName))
    {
        appendLog(tr("Save to %1 failed.").arg(fileName), CHIYO_LOG_INFO);
        return false;
    }
    else
    {
        appendLog(tr("Successfullly saved to %1.").arg(fileName), CHIYO_LOG_INFO);
        return true;
    }
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

void ChiyoEditor::setImage(QImage &img)
{
    QPixmap pix = QPixmap::fromImage(img);
    ui->controls->setImage(pix);
}
