#ifndef CHIYOEDITOR_H
#define CHIYOEDITOR_H

#include <QWidget>
#include <QSplitter>

namespace Ui {
class ChiyoEditor;
}

class ChiyoEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ChiyoEditor(int eid, QWidget *parent = 0);
    ~ChiyoEditor();

    static ChiyoEditor* instance(QWidget *parent = 0);

    bool loadImage(const QString &filename);

    void appendLog(const QString &log, int type);

private:
    void setWindowTitleByFileName(const QString &filename);

private:
    Ui::ChiyoEditor *ui;

    int editor_id;

    QString current_file;
};

#endif // CHIYOEDITOR_H
