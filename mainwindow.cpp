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

/******************************************************************************
 *                              MainWindow methods                            *
 ******************************************************************************/
// constructor /////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow),
    data_(new CorrelatorData(this)),
    dataModel_(new DataModel(data_, this))

{
    ui_->setupUi(this);
    connect(data_, SIGNAL(combinedSampleChanged()), this, SLOT(replot()));
    connect(ui_->logAbsCheckBox, SIGNAL(stateChanged(int)),
            this, SIGNAL(plotOptionsChanged()));
    connect(ui_->combineCheckBox, SIGNAL(stateChanged(int)),
            this, SIGNAL(plotOptionsChanged()));
    connect(ui_->emTypeComboBox, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(plotOptionsChanged()));
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
    ui_->combineButton->setDefaultAction(ui_->actionCombine);
    ui_->combineCode->setVisible(combineDataChecked());
    ui_->combineButton->setVisible(combineDataChecked());
}

// destructor //////////////////////////////////////////////////////////////////
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

// access //////////////////////////////////////////////////////////////////////
GnuplotWidget * MainWindow::gnuplotWidget(const PlotType p)
{
    return gpWidget_[p];
}

// plot a single correlator ////////////////////////////////////////////////////
void MainWindow::plotCorr(Plot &p, const DVec &t, const DMatSample &c,
                          const QString name)
{
    DMatSample tmp = c;

    if (logAbsChecked())
    {
        FOR_STAT_ARRAY(tmp, s)
        {
            tmp[s] = tmp[s].cwiseAbs();
        }
        p << LogScale(Axis::y);
    }
    p << Title(name.toStdString()) << PlotData(t, tmp);
}

// plot a single effective mass ////////////////////////////////////////////////
void MainWindow::plotEm(Latan::Plot &p, const Latan::EffectiveMass &em,
                        const Latan::DMatSample &c, const QString name)
{
    Index      nt = c[central].rows();
    DVec       t = em.getTime(nt);
    DMatSample tmp = em(c);

    p << Title(name.toStdString()) << PlotData(t, tmp);
}

// replot specific plot ////////////////////////////////////////////////////////
void MainWindow::replot(const PlotType p)
{
    if (data_->size() > 0)
    {
        Index nt = data_->sample(0)[central].rows();
        DVec  t;

        t.setLinSpaced(nt, 0, nt - 1);
        plot_[p].reset();
        switch (p)
        {
        case PlotType::corr:
        {
            if (combineDataChecked() and data_->hasCombination())
            {
                plotCorr(plot_[p], t, data_->combinedSample(), "combined data");
            }
            else
            {
                for (int i = 0; i < data_->size(); ++i)
                {
                    plotCorr(plot_[p], t, data_->sample(i),
                             dataModel_->filename(i));
                }
            }
            plot_[p] << PlotRange(Axis::x, 0, nt);
            gnuplotWidget(p)->plot();
        }
        break;
        case PlotType::em:
        {
            EffectiveMass em(getEmType());

            if (combineDataChecked() and data_->hasCombination())
            {
                plotEm(plot_[p], em, data_->combinedSample(), "combined data");
            }
            else
            {
                for (int i = 0; i < data_->size(); ++i)
                {
                    plotEm(plot_[p], em, data_->sample(i),
                           dataModel_->filename(i));
                }
            }
            plot_[p] << PlotRange(Axis::x, 0, nt);
            gnuplotWidget(p)->plot();
        }
        break;
        }
    }
    else
    {
        gnuplotWidget(p)->reset();
    }
}

// get checkboxes status ///////////////////////////////////////////////////////
bool MainWindow::logAbsChecked(void) const
{
    return (ui_->logAbsCheckBox->checkState() == Qt::CheckState::Checked);
}

bool MainWindow::combineDataChecked(void) const
{
    return (ui_->combineCheckBox->checkState() == Qt::CheckState::Checked);
}

// get em type /////////////////////////////////////////////////////////////////
#define IF_TYPE(t) \
if (selected == #t) {type = CorrelatorType::t;}
#define ELIF_TYPE(t) else IF_TYPE(t)

CorrelatorType MainWindow::getEmType(void) const
{
    QString        selected = ui_->emTypeComboBox->currentText();
    CorrelatorType type = CorrelatorType::undefined;

    IF_TYPE(exp)
    ELIF_TYPE(cosh)
    ELIF_TYPE(sinh)
    ELIF_TYPE(linear)
    ELIF_TYPE(cst)

    return type;
}

#undef IF_TYPE
#undef ELIF_TYPE

/******************************************************************************
 *                              MainWindow slots                              *
 ******************************************************************************/
// add data dialog /////////////////////////////////////////////////////////////
void MainWindow::addData(void)
{
    QStringList filename = QFileDialog::getOpenFileNames(this, "Add files",
                                                         QDir::homePath(),
                                                         "LatAnalyze sample (*.h5 *.dat)");

    if (!filename.empty())
    {
        dataModel_->addFiles(filename);
        if (!combineDataChecked())
        {
            emit plotOptionsChanged();
        }
    }
}

// remove selected data ////////////////////////////////////////////////////////
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
    if (!combineDataChecked())
    {
        emit plotOptionsChanged();
    }
}

// combine data ////////////////////////////////////////////////////////////////
void MainWindow::combineData(void)
{
   data_->setFunction(ui_->combineCode->toPlainText());
   data_->combine();
}

// refresh all plots ///////////////////////////////////////////////////////////
void MainWindow::replot(void)
{
    for (unsigned int p = 0; p < nPlot; ++p)
    {
        replot(static_cast<PlotType>(p));
    }
}

// project management //////////////////////////////////////////////////////////
void MainWindow::newProject(void)
{
    dataModel_->clear();
    emit plotOptionsChanged();
}

void MainWindow::saveProject(void)
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Save project", "",
                                                    "Plateau project (*.pla);;All Files (*)");

    if (!fileName.isEmpty())
    {
        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(this, "Unable to open file",
                                     file.errorString());
            return;
        }

        QDataStream out(&file);

        out.setVersion(QDataStream::Qt_4_5);
        out << dataModel_->getList();
    }
}

void MainWindow::openProject(void)
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Open project", "",
                                                    "Plateau project (*.pla);;All Files (*)");

    if (!fileName.isEmpty())
    {
        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, "Unable to open file",
                                     file.errorString());
            return;
        }

        QDataStream in(&file);
        QStringList list;

        in.setVersion(QDataStream::Qt_4_5);
        in >> list;
        dataModel_->clear();
        dataModel_->addFiles(list);
        if (!combineDataChecked())
        {
            emit plotOptionsChanged();
        }
    }
}
