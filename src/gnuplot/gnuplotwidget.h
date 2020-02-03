/*
 * gnuplotwidget.h, part of Plateau
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

#ifndef GNUPLOTWIDGET_H
#define GNUPLOTWIDGET_H

#include <QMainWindow>
#include <LatAnalyze/Core/Plot.hpp>
#include <src/global.h>
#include <src/gnuplot/QtGnuplotInstance.h>
#include <src/gnuplot/QtGnuplotWidget.h>

/******************************************************************************
 *                             Gnuplot widget                                 *
 ******************************************************************************/
namespace Ui {
class GnuplotWidget;
}

class GnuplotWidget : public QMainWindow
{
    Q_OBJECT

public:
    // constructor/destructor
    explicit GnuplotWidget(Latan::Plot *plot, QWidget *parent = nullptr);
    ~GnuplotWidget();

public slots:
    // send string to stdout
    void gnuplotOutput(const QString& output);
    // display plot
    void plot(void);
    // refresh plot
    void refresh(void);
    // save PDF
    void savePdf(void);
    // save script & PDF
    void savePlot(void);
    // reset plot
    void reset(void);

private:
    Ui::GnuplotWidget *ui_;
    QtGnuplotInstance *gp_;
    QtGnuplotWidget   *gpWidget_;
    QToolBar          *mouseToolBar_;
    QLabel            *mouseLabel_;
    Latan::Plot       *plot_;
};

#endif // GNUPLOTWIDGET_H
