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
#include <LatAnalyze/Functional/CompiledFunction.hpp>
#include <LatAnalyze/Io/Io.hpp>

using namespace Latan;

CorrelatorData::CorrelatorData(QObject *parent) : QObject(parent)
{
    markDirty();
    connect(this, SIGNAL(dataChanged()), this, SLOT(markDirty()));
}

const Latan::DMatSample & CorrelatorData::sample(const int i)
{
    return sample_.at(i);
}

const Latan::DMatSample & CorrelatorData::combinedSample(void)
{
    return combined_;
}

bool CorrelatorData::hasCombination(void)
{
    return hasCombination_;
}

bool CorrelatorData::isClean(void)
{
    return isClean_;
}

void CorrelatorData::load(const int i, const QString filename)
{ 
    Index nt;

    sample_.insert(i, Io::load<DMatSample>(filename.toStdString()));
    nt         = sample_[i][central].rows();
    sample_[i] = sample_[i].block(0, 0, nt, 1);
    emit dataChanged();
}

void CorrelatorData::unload(const int i)
{
    sample_.removeAt(i);
    emit dataChanged();
}

void CorrelatorData::setFunction(const QString code)
{
    code_  = code;
    emit dataChanged();
}

void CorrelatorData::markDirty(void)
{
    isClean_ = false;
}

void CorrelatorData::combine(void)
{
    const unsigned int n = sample_.size();

    if ((n > 0) and !isClean())
    {
        DoubleFunction     f = compile(code_.toStdString(), n);
        DVec               buf(n);

        combined_ = sample_.at(0);
        FOR_STAT_ARRAY(combined_, s)
        {
            FOR_MAT(combined_[s], i, j)
            {
                for (unsigned int k = 0; k < n; ++k)
                {
                    buf[k] = sample_.at(k)[s](i,j);
                }
                combined_[s](i, j) = f(buf);
            }
        }
        isClean_        = true;
        hasCombination_ = true;
        emit combinedSampleChanged();
    }
}

