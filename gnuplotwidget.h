#ifndef GNUPLOTWIDGET_H
#define GNUPLOTWIDGET_H

#include "gnuplot/QtGnuplotInstance.h"
#include "gnuplot/QtGnuplotWindow.h"

#include <QtCore>
#include <QtGui>
#include <QWidget>

class GnuplotWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GnuplotWidget(QWidget *parent = nullptr);
public slots:
    void plot(const QString &gpCmd);
    void gnuplotOutput(const QString& output);
private:
    QtGnuplotInstance gp;
    QtGnuplotWidget*  gpWidget;
    QtGnuplotWindow*  gpWindow;
};

#endif // GNUPLOTWIDGET_H
