/*
 * correlatordata.cpp, part of Plateau
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

#include "correlatordata.h"
#include <QtCore>
#include <LatAnalyze/Io/Io.hpp>

using namespace Latan;

CorrelatorData::CorrelatorData(QObject *parent) : QObject(parent)
{

}

void CorrelatorData::load(const QString &filename)
{
    Index nt;
    DVec  t;

    sample_ = Io::load<DMatSample>(filename.toStdString());
    nt      = sample_[central].rows();
    sample_ = sample_.block(0, 0, nt, 1);
    t.setLinSpaced(nt, 0, nt - 1);
    p_.reset();
    p_ << PlotData(t, sample_);
    emit plotUpdate(p_);
}
