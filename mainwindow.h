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
#include "correlatordata.h"
#include "datamodel.h"
#include "gnuplotwidget.h"

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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow(void);
    GnuplotWidget * gnuplotWidget(const PlotType p);
    void replot(const PlotType p);
    bool logAbsChecked(void);
    bool combineDataChecked(void);

public slots:
    void addData(void);
    void removeData(void);
    void replot(void);
    void combineData(void);

signals:
    void status(const QString &msg);
    void plotOptionsChanged(void);

private:
    void plotCorr(Latan::Plot &p, const Latan::DVec &t,
                  const Latan::DMatSample &c, const QString name);

private:
    Ui::MainWindow                     *ui_;
    CorrelatorData                     *data_;
    std::array<GnuplotWidget *, nPlot> gpWidget_;
    std::array<Latan::Plot, nPlot>     plot_;
    DataModel                          *dataModel_;
};

#endif // MAINWINDOW_H
