#include "gnuplotwidget.h"
#include "gnuplot/QtGnuplotWidget.h"
#include "gnuplot/QtGnuplotWindow.h"

GnuplotWidget::GnuplotWidget(QWidget *parent)
: QWidget(parent), gp(0, "/usr/local/bin/gnuplot")
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    gpWindow = new QtGnuplotWindow(0, nullptr, nullptr);
    gpWidget = dynamic_cast<QtGnuplotWidget *>(gpWindow->centralWidget());
    gpWindow->setWindowFlag(Qt::Widget);
    layout->addWidget(gpWindow);
    setLayout(layout);
    gp.setWidget(gpWidget);
    connect(&gp, SIGNAL(gnuplotOutput(const QString&)), this,
            SLOT(gnuplotOutput(const QString&)));

}

void GnuplotWidget::plot(const QString &gpCmd)
{
    gp << gpCmd << "\n";
}

void GnuplotWidget::gnuplotOutput(const QString& output)
{
    QTextStream out(stdout);

    out << output << endl;
}
