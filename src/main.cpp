#include "MainWidget.h"
#include <QApplication>
#include <QFontDatabase>
#include <QTranslator>

QString G_FONT_BB3812 = QString("");

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 字体初始化
    int strFontID = QFontDatabase::addApplicationFont("./bb3812.ttf");
    QStringList listFont = QFontDatabase::applicationFontFamilies(strFontID);
    if (listFont.size() > 0)
        G_FONT_BB3812 = listFont.first();

    QTranslator *transltor = new QTranslator();
    transltor->load("./translations/language.qm");
    a.installTranslator(transltor);

    MainWidget w;
    w.show();

    return a.exec();
}
