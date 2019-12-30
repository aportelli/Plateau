#ifndef GNUPLOTWIDGET_H
#define GNUPLOTWIDGET_H

#include "gnuplot/QtGnuplotInstance.h"
#include "gnuplot/QtGnuplotWidget.h"
#include <QMainWindow>
#include <LatAnalyze/Core/Plot.hpp>

namespace Ui {
class GnuplotWidget;
}

class GnuplotWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit GnuplotWidget(Latan::Plot *plot, QWidget *parent = nullptr);
    ~GnuplotWidget();

public slots:
    void gnuplotOutput(const QString& output);
    void plot(void);
    void savePdf(void);
    void savePlot(void);

private:
    Ui::GnuplotWidget *ui_;
    QtGnuplotInstance gp_;
    QtGnuplotWidget   *gpWidget_;
    QToolBar          *mouseToolBar_;
    QLabel            *mouseLabel_;
    Latan::Plot       *plot_;
};

#endif // GNUPLOTWIDGET_H
