#-------------------------------------------------
#
# Project created by QtCreator 2017-08-20T09:33:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpeedCodeTablePro
TEMPLATE = app

DESTDIR = $$PWD\..\bin

SOURCES += main.cpp\
    SpeedWidget.cpp \
    MainWidget.cpp \
    CodeTableWnd.cpp

HEADERS  += \
    CodeTableWnd.h \
    MainWidget.h \
    SpeedWidget.h

RESOURCES += \
    resource.qrc

TRANSLATIONS += language.ts

fontfiles.files = $$PWD\*.ttf
fontfiles.path = $$DESTDIR
INSTALLS += fontfiles

imagefiles.files = $$PWD\*.png
imagefiles.path = $$DESTDIR
INSTALLS += imagefiles

languages.files = $$PWD\*.qm
languages.path = $$DESTDIR\translations
INSTALLS += languages

FORMS += \
    MainWidget.ui \
    SpeedWidget.ui
