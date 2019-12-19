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

using namespace Latan;

GnuplotWidget::GnuplotWidget(QWidget *parent)
: QWidget(parent), gp_(0, "/usr/local/bin/gnuplot")
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    gpWindow_ = new QtGnuplotWindow(0, nullptr, nullptr);
    gpWidget_ = qobject_cast<QtGnuplotWidget *>(gpWindow_->centralWidget());
    gpWindow_->setWindowFlag(Qt::Widget);
    layout->addWidget(gpWindow_);
    setLayout(layout);
    gp_.setWidget(gpWidget_);
    connect(&gp_, SIGNAL(gnuplotOutput(const QString&)), this,
            SLOT(gnuplotOutput(const QString&)));

}

void GnuplotWidget::plot(const QString &gpCmd)
{
    gp_ << gpCmd << "\n";
}

void GnuplotWidget::plot(const Plot &p)
{
    std::ostringstream out;

    out << p;
    plot(out.str().c_str());
}

void GnuplotWidget::gnuplotOutput(const QString& output)
{
    QTextStream out(stdout);

    out << output << endl;
}
