QT += core gui svg network printsupport

QMAKE_CC = /usr/bin/clang
QMAKE_CXX = /usr/bin/clang++

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    datamodel.cpp \
    gnuplot/QtGnuplotEvent.cpp \
    gnuplot/QtGnuplotInstance.cpp \
    gnuplot/QtGnuplotItems.cpp \
    gnuplot/QtGnuplotScene.cpp \
    gnuplot/QtGnuplotWidget.cpp \
    correlatordata.cpp \
    gnuplotwidget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    datamodel.h \
    gnuplot/QtGnuplotEvent.h \
    gnuplot/QtGnuplotInstance.h \
    gnuplot/QtGnuplotItems.h \
    gnuplot/QtGnuplotScene.h \
    gnuplot/QtGnuplotWidget.h \
    gnuplot/mousecmn.h \
    correlatordata.h \
    gnuplotwidget.h \
    mainwindow.h

FORMS += \
    gnuplotwidget.ui \
    mainwindow.ui

RESOURCES +=

DEP = dependencies/prefix

INCLUDEPATH += $$PWD/$$DEP/include
DEPENDPATH += $$PWD/$$DEP/include

LIBS += -L$$PWD/$$DEP/lib/ -lLatAnalyze

