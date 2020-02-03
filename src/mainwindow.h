/*
 * mainwindow.h, part of Plateau
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <LatAnalyze/Physics/EffectiveMass.hpp>
#include <src/data/correlatordata.h>
#include <src/data/datatablemodel.h>
#include <src/gnuplot/gnuplotwidget.h>

/******************************************************************************
 *                          Main application window                           *
 ******************************************************************************/
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum PlotType
    {
        corr  = 0,
        em    = 1
    };
    static constexpr unsigned int nPlot = 2;

public:
    // constructor/destructor
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow(void);
    // access
    GnuplotWidget * gnuplotWidget(const PlotType p);
    // replot specific plot
    void replot(const PlotType p);
    // get checkboxes status
    bool logAbsChecked(void) const;
    bool combineDataChecked(void) const;
    // get em type
    Latan::CorrelatorType getEmType(void) const;
public slots:
    // add data dialog
    void addData(void);
    // edit data dialog
    void editData(void);
    // remove selected data
    void removeData(void);
    // combine data
    void combineData(void);
    // redo all plots
    void replot(void);
    // refresh all plots (does not actually redo the plots)
    void refreshPlots(void);
    // project management
    void newProject(void);
    void saveProject(void);
    void openProject(void);

signals:
    void status(const QString &msg);
    void plotOptionsChanged(void);

private:
    // plot a single correlator
    void plotCorr(Latan::Plot &p, const Latan::DVec &t,
                  const Latan::DMatSample &c, const QString name);
    // plot a single effective mass
    void plotEm(Latan::Plot &p, const Latan::EffectiveMass &em,
                const Latan::DMatSample &c, const QString name);

private:
    Ui::MainWindow                     *ui_;
    CorrelatorData                     *data_;
    std::array<GnuplotWidget *, nPlot> gpWidget_;
    std::array<Latan::Plot, nPlot>     plot_;
    DataTableModel                          *dataModel_;
};

#endif // MAINWINDOW_H
