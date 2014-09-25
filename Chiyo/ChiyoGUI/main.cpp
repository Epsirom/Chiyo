#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDebug>


void loadLanguage(QTranslator& translator)
{
    static const QString default_language = "zh_CN";
    QString system_language = QLocale::system().name();

    bool b = translator.load(QString(":/lang/") + system_language);
    if (!b)
    {
        translator.load(QString(":/lang/") + default_language);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator t;
    loadLanguage(t);
    a.installTranslator(&t);

    MainWindow w;
    w.show();

    return a.exec();
}
