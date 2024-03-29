/*
 * mainwindow.cpp, part of Plateau
 *
 * Copyright (C) 2019-2020 Antonin Portelli
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

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <src/Global.h>
#include <src/data/DataOptionDialog.h>
#include <src/gnuplot/GnuplotWidget.h>

using namespace Latan;

/******************************************************************************
 *                              MainWindow methods                            *
 ******************************************************************************/
// constructor /////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow),
    data_(new CorrelatorData(this)),
    dataModel_(new DataTableModel(data_, this))

{
    for (unsigned int p = 0; p < nPlot; ++p)
    {
        gpWidget_[p] = new GnuplotWidget(&plot_[p], this);
    }
    ui_->setupUi(this);
    setWindowTitle("Plateau");
    connect(data_, SIGNAL(combinedSampleChanged()), this, SLOT(replot()));
    connect(ui_->logAbsCheckBox, SIGNAL(stateChanged(int)),
            this, SIGNAL(plotOptionsChanged()));
    connect(ui_->combineCheckBox, SIGNAL(stateChanged(int)),
            this, SIGNAL(plotOptionsChanged()));
    connect(ui_->combineCode, SIGNAL(textChanged()),
            this, SLOT(storePlotOptions()));
    connect(ui_->emTypeComboBox, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(plotOptionsChanged()));
    connect(this, SIGNAL(plotOptionsChanged()), this, SLOT(storePlotOptions()));
    connect(this, SIGNAL(plotOptionsChanged()), this, SLOT(replot()));
    ui_->corrPlotLayout->addWidget(gpWidget_[PlotType::corr]);
    ui_->emPlotLayout->addWidget(gpWidget_[PlotType::em]);
    ui_->dataTableView->setModel(dataModel_);
    ui_->dataTableView->hideColumn(2);
    ui_->addDataButton->setDefaultAction(ui_->actionAddFile);
    ui_->editDataButton->setDefaultAction(ui_->actionEditData);
    ui_->removeDataButton->setDefaultAction(ui_->actionRemoveFile);
    ui_->combineButton->setDefaultAction(ui_->actionCombine);
    storePlotOptions();
    ui_->combineCode->setVisible(plotOptions_.combineData);
    ui_->combineButton->setVisible(plotOptions_.combineData);
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

    if (plotOptions_.logAbs)
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
            if (plotOptions_.combineData and data_->hasCombination())
            {
                plotCorr(plot_[p], t, data_->combinedSample(), "combined data");
            }
            else
            {
                for (int i = 0; i < data_->size(); ++i)
                {
                    plotCorr(plot_[p], t, data_->sample(i),
                             dataModel_->getFilename(i));
                }
            }
            plot_[p] << PlotRange(Axis::x, 0, nt);
            gnuplotWidget(p)->plot();
        }
        break;
        case PlotType::em:
        {
            EffectiveMass em(plotOptions_.emType);

            if (plotOptions_.combineData and data_->hasCombination())
            {
                plotEm(plot_[p], em, data_->combinedSample(), "combined data");
            }
            else
            {
                for (int i = 0; i < data_->size(); ++i)
                {
                    plotEm(plot_[p], em, data_->sample(i),
                           dataModel_->getFilename(i));
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

/******************************************************************************
 *                              MainWindow slots                              *
 ******************************************************************************/
// add data dialog /////////////////////////////////////////////////////////////
void MainWindow::addData(void)
{
    QStringList filename = QFileDialog::getOpenFileNames(this, "Add files",
                                                         QDir::homePath(),
                                                         "LatAnalyze sample (*.h5 *.dat);;All Files (*)");

    if (!filename.empty())
    {
        dataModel_->addFiles(filename);
        if (!plotOptions_.combineData)
        {
            emit plotOptionsChanged();
        }
    }
}

// edit data dialog ////////////////////////////////////////////////////////////
void MainWindow::editData(void)
{
    QModelIndexList l = ui_->dataTableView->selectionModel()->selectedIndexes();

    if (l.size() == 1)
    {
        DataOptionDialog::Option opt;
        QString                  filename;

        filename     = dataModel_->getFilepath(l.first().row());
        opt.filename = filename;
        opt.tr       = dataModel_->getTransform(l.first().row());
        opt          = DataOptionDialog::getDataOption(opt, this);
        if (opt.accepted)
        {
            if (QFile::exists(opt.filename))
            {
                dataModel_->editFile(filename, opt.filename, opt.tr);
                if (!plotOptions_.combineData)
                {
                    emit plotOptionsChanged();
                }
            }
            else
            {
                QMessageBox msgBox;

                msgBox.warning(this, "Error",
                               "file '" + filename + "' does not exists");
            }
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
        toRemove.append(dataModel_->getFilepath(l.at(i).row()));
    }
    for (int j = 0; j < toRemove.size(); ++j)
    {
        dataModel_->removeFile(toRemove.at(j));
    }
    if (!plotOptions_.combineData)
    {
        emit plotOptionsChanged();
    }
}

// combine data ////////////////////////////////////////////////////////////////
void MainWindow::combineData(void)
{
   data_->setFunction(plotOptions_.combineCode);
   data_->combine();
}

// refresh plot options ////////////////////////////////////////////////////////
#define IF_TYPE(t) \
if (selected == #t) {plotOptions_.emType = CorrelatorType::t;}
#define ELIF_TYPE(t) else IF_TYPE(t)

void MainWindow::storePlotOptions(void)
{
    QString selected = ui_->emTypeComboBox->currentText();

    plotOptions_.logAbs =
        (ui_->logAbsCheckBox->checkState() == Qt::CheckState::Checked);
    plotOptions_.combineData =
        (ui_->combineCheckBox->checkState() == Qt::CheckState::Checked);
    plotOptions_.combineCode = ui_->combineCode->toPlainText();
    qDebug() << "store" << plotOptions_.combineCode;
    plotOptions_.emType = CorrelatorType::undefined;
    IF_TYPE(exp)
    ELIF_TYPE(cosh)
    ELIF_TYPE(sinh)
    ELIF_TYPE(linear)
    ELIF_TYPE(cst)
}

#undef IF_TYPE
#undef ELIF_TYPE

#define IF_TYPE(t) \
if (plotOptions_.emType == CorrelatorType::t) {selected = #t;}
#define ELIF_TYPE(t) else IF_TYPE(t)

void MainWindow::restorePlotOptions(void)
{
    QString selected;
    int     index;

    ui_->logAbsCheckBox->setCheckState(
        plotOptions_.logAbs ?
        Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    ui_->combineCheckBox->setCheckState(
        plotOptions_.combineData ?
        Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    qDebug() << "restore" << plotOptions_.combineCode;
    ui_->combineCode->setPlainText(plotOptions_.combineCode);
    IF_TYPE(exp)
    ELIF_TYPE(cosh)
    ELIF_TYPE(sinh)
    ELIF_TYPE(linear)
    ELIF_TYPE(cst)
    index = ui_->emTypeComboBox->findText(selected);
    ui_->emTypeComboBox->setCurrentIndex(index);

    emit plotOptionsChanged();
}

#undef IF_TYPE
#undef ELIF_TYPE

// redo all plots //////////////////////////////////////////////////////////////
void MainWindow::replot(void)
{
    for (unsigned int p = 0; p < nPlot; ++p)
    {
        replot(static_cast<PlotType>(p));
    }
}

// refresh all plots (does not actually redo the plots) ////////////////////////
void MainWindow::refreshPlots(void)
{
    for (unsigned int p = 0; p < nPlot; ++p)
    {
        gnuplotWidget(static_cast<PlotType>(p))->refresh();
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
        out << plotOptions_;
        out << *dataModel_;
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

        in.setVersion(QDataStream::Qt_4_5);
        in >> plotOptions_;
        qDebug() << "load" << plotOptions_.combineCode;
        in >> *dataModel_;
        restorePlotOptions();
    }
}

/******************************************************************************
 *                       plot options serialisation                           *
 ******************************************************************************/
QDataStream &operator<<(QDataStream &out, const MainWindow::PlotOptions &p)
{
    out << p.logAbs;
    out << p.combineData;
    out << p.combineCode;
    out << static_cast<qint32>(p.emType);

    return out;
}

QDataStream &operator>>(QDataStream &in, MainWindow::PlotOptions &p)
{
    qint32 emType;

    in >> p.logAbs;
    in >> p.combineData;
    in >> p.combineCode;
    in >> emType;
    p.emType = static_cast<Latan::CorrelatorType>(emType);

    return in;
}
