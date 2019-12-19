/*
 * gnuplotwidget.h, part of Plateau
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
    QtGnuplotInstance gp_;
    QtGnuplotWidget*  gpWidget_;
    QtGnuplotWindow*  gpWindow_;
};

#endif // GNUPLOTWIDGET_H
