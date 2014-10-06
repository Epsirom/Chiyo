#include "chiyoloader.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDebug>

#include "chiyoplugins.h"

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

    qRegisterMetaType<PluginAction>("PluginAction");

    ChiyoLoader* w = ChiyoLoader::instance();
    w->show();

    int rtn = a.exec();
    delete w;

    return rtn;
}
