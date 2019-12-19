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
