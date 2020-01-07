/*
 * mainwindow.cpp, part of Plateau
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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gnuplotwidget.h"

using namespace Latan;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow),
    data_(new CorrelatorData(this)),
    dataModel_(new DataModel(this))
{
    ui_->setupUi(this);
    connect(data_, SIGNAL(dataChanged()), this, SLOT(replot()));
    connect(this, SIGNAL(plotOptionsChanged()), this, SLOT(replot()));
    for (unsigned int p = 0; p < nPlot; ++p)
    {
        gpWidget_[p] = new GnuplotWidget(&plot_[p], this);
    }
    ui_->corrPlotLayout->addWidget(gpWidget_[PlotType::corr]);
    ui_->emPlotLayout->addWidget(gpWidget_[PlotType::em]);
    ui_->dataTableView->setModel(dataModel_);
    ui_->dataTableView->hideColumn(2);
    ui_->addDataButton->setDefaultAction(ui_->actionAddFile);
    ui_->removeDataButton->setDefaultAction(ui_->actionRemoveFile);
}

MainWindow::~MainWindow(void)
{
    delete ui_;
    delete data_;
    delete dataModel_;
    for (unsigned int p = 0; p < nPlot; ++p)
    {
        delete gpWidget_[p];
    }
}

GnuplotWidget * MainWindow::gnuplotWidget(const PlotType p)
{
    return gpWidget_[p];
}

void MainWindow::replot(const PlotType p)
{
    if (data_->loaded())
    {
        const DMatSample &smp = data_->sample();
        Index nt;
        DVec  t;

        nt = smp[central].rows();
        t.setLinSpaced(nt, 0, nt - 1);
        plot_[p].reset();
        switch (p)
        {
        case PlotType::corr:
        {
            DMatSample tmp = smp;

            if (logAbs_)
            {
                FOR_STAT_ARRAY(tmp, s)
                {
                    tmp[s] = tmp[s].cwiseAbs();
                }
                plot_[p] << LogScale(Axis::y);
            }
            plot_[p] << PlotData(t, tmp);
            plot_[p] << PlotRange(Axis::x, 0, nt);
            gnuplotWidget(p)->plot();
        }
        case PlotType::em:
        {

        }
        }
    }
}

void MainWindow::open(void)
{
    QString filename = QFileDialog::getOpenFileName(this, "Open file",
                                                    QDir::homePath(),
                                                    "LatAnalyze sample (*.h5 *.dat)");
    emit status("loading ...");
    if (!filename.isEmpty())
    {
        data_->load(filename);
    }
    emit status("");
}

void MainWindow::addData(void)
{
    QString filename = QFileDialog::getOpenFileName(this, "Add file",
                                                    QDir::homePath(),
                                                    "LatAnalyze sample (*.h5 *.dat)");

    if (!filename.isEmpty())
    {
        dataModel_->addFile(filename);
    }
}

void MainWindow::removeData(void)
{
    QModelIndexList l = ui_->dataTableView->selectionModel()->selectedIndexes();
    QStringList     toRemove;

    for (int i = 0; i < l.size(); ++i)
    {
        toRemove.append(dataModel_->filepath(l.at(i).row()));
    }
    for (int j = 0; j < toRemove.size(); ++j)
    {
        dataModel_->removeFile(toRemove.at(j));
    }
}

void MainWindow::replot(void)
{
    for (unsigned int p = 0; p < nPlot; ++p)
    {
        replot(static_cast<PlotType>(p));
    }
}

void MainWindow::corrLogAbs(int logAbs)
{
    logAbs_ = (logAbs == Qt::CheckState::Checked);
    emit plotOptionsChanged();
}

