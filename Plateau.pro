QT += core gui svg network printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 silent

SOURCES += \
    src/MainWindow.cpp \
    src/data/CorrelatorData.cpp \
    src/data/DataOptionDialog.cpp \
    src/data/DataTableModel.cpp \
    src/gnuplot/GnuplotWidget.cpp \
    src/main.cpp \
    src/gnuplot/QtGnuplotEvent.cpp \
    src/gnuplot/QtGnuplotInstance.cpp \
    src/gnuplot/QtGnuplotItems.cpp \
    src/gnuplot/QtGnuplotScene.cpp \
    src/gnuplot/QtGnuplotWidget.cpp

HEADERS += \
    src/Global.h \
    src/MainWindow.h \
    src/data/CorrelatorData.h \
    src/data/DataOptionDialog.h \
    src/data/DataTableModel.h \
    src/gnuplot/GnuplotWidget.h \
    src/gnuplot/QtGnuplotEvent.h \
    src/gnuplot/QtGnuplotInstance.h \
    src/gnuplot/QtGnuplotItems.h \
    src/gnuplot/QtGnuplotScene.h \
    src/gnuplot/QtGnuplotWidget.h \
    src/gnuplot/mousecmn.h

FORMS += \
    src/MainWindow.ui \
    src/data/DataOptionDialog.ui \
    src/gnuplot/GnuplotWidget.ui

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
