/*
 * gnuplotwidget.cpp, part of Plateau
 *
 * Copyright (C) 2019 Antonin Portelli
 *
 * Plateau is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Plateau is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Plateau.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gnuplotwidget.h"
#include "ui_gnuplotwidget.h"

using namespace Latan;

GnuplotWidget::GnuplotWidget(Plot *plot, QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::GnuplotWidget),
    plot_(plot)
{
    QString gnuplotPath = plot->getProgramPath().c_str();

    gnuplotPath += "/gnuplot";
    INFO("found gnuplot at " + gnuplotPath);
    ui_->setupUi(this);
    gpWidget_ = new QtGnuplotWidget;
    gpWidget_->setBackgroundColor(Qt::white);
    gpWidget_->setAntialias(true);
    gpWidget_->setRounded(true);
    gpWidget_->setReplotOnResize(true);
    gp_ = new QtGnuplotInstance(gpWidget_, gnuplotPath);
    connect(gp_, SIGNAL(gnuplotOutput(const QString&)), this,
            SLOT(gnuplotOutput(const QString&)));
    setCentralWidget(gpWidget_);
    mouseLabel_   = new QLabel;
    ui_->mouseToolBar->addWidget(mouseLabel_);
    connect(gpWidget_, SIGNAL(statusTextChanged(const QString&)), mouseLabel_,
            SLOT(setText(const QString &)));
    reset();
}

GnuplotWidget::~GnuplotWidget()
{
    delete ui_;
    delete gp_;
    delete gpWidget_;
    delete mouseLabel_;
}

void GnuplotWidget::gnuplotOutput(const QString& output)
{
    QTextStream out(stdout);

    out << output << endl;
}

void GnuplotWidget::plot(void)
{
    std::ostringstream out;

    out << *plot_;
    *gp_ << out.str().c_str() << "\n";
}

void GnuplotWidget::savePdf(void)
{
    QString filename = QFileDialog::getSaveFileName(this, "Export to PDF", "",
                                                    "PDF files (*.pdf)");

    if (filename.isEmpty())
    {
        return;
    }
    if (!filename.endsWith(".pdf", Qt::CaseInsensitive))
    {
        filename += ".pdf";
    }
    gpWidget_->exportToPdf(filename);
}

void GnuplotWidget::savePlot(void)
{
    QString dirname = QFileDialog::getSaveFileName(this);

    if (dirname.isEmpty())
    {
        return;
    }
    plot_->save(dirname.toStdString(), false);
    gpWidget_->exportToPdf(dirname + "/plot.pdf");
}

void GnuplotWidget::reset(void)
{
    plot_->reset();
    *plot_ << PlotRange(Axis::x, -1, 1) << PlotRange(Axis::y, -1, 1);
    *plot_ << PlotCommand("1/0");
    plot();
}
