QT += core gui svg network printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 silent

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/data/correlatordata.cpp \
    src/data/dataoptiondialog.cpp \
    src/data/datatablemodel.cpp \
    src/gnuplot/QtGnuplotEvent.cpp \
    src/gnuplot/QtGnuplotInstance.cpp \
    src/gnuplot/QtGnuplotItems.cpp \
    src/gnuplot/QtGnuplotScene.cpp \
    src/gnuplot/QtGnuplotWidget.cpp \
    src/gnuplot/gnuplotwidget.cpp

HEADERS += \
    src/global.h \
    src/mainwindow.h \
    src/data/correlatordata.h \
    src/data/dataoptiondialog.h \
    src/data/datatablemodel.h \
    src/gnuplot/QtGnuplotEvent.h \
    src/gnuplot/QtGnuplotInstance.h \
    src/gnuplot/QtGnuplotItems.h \
    src/gnuplot/QtGnuplotScene.h \
    src/gnuplot/QtGnuplotWidget.h \
    src/gnuplot/mousecmn.h \
    src/gnuplot/gnuplotwidget.h

FORMS += \
    src/data/dataoptiondialog.ui \
    src/gnuplot/gnuplotwidget.ui \
    src/mainwindow.ui

RESOURCES +=

TEMPLATE

defined(LATAN_PREFIX, var) {
    DEP = $$LATAN_PREFIX
} else {
    DEP = $$PWD/dependencies/prefix
}

DEPENDPATH += $$DEP/include
DEPENDPATH += $$DEP/lib
LIBS       += -L$$DEP/lib/ -lLatAnalyze

QMAKE_CXXFLAGS += $$system("$$DEP/bin/latan-config --cxxflags")
QMAKE_CXXFLAGS += -I$$DEP/include

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.14
QMAKE_TARGET_BUNDLE_PREFIX = org.aportelli

DISTFILES += \
    README.md \
    update-latan.sh

SUBDIRS += \
    src/gnuplot/QtGnuplot.pro
