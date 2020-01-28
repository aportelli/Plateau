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

#include "global.h"
#include "correlatordata.h"
#include <QtCore>
#include <LatAnalyze/Functional/CompiledFunction.hpp>
#include <LatAnalyze/Io/Io.hpp>

using namespace Latan;

/******************************************************************************
 *                        CorrelatorData methods                              *
 ******************************************************************************/
// constructor /////////////////////////////////////////////////////////////////
CorrelatorData::CorrelatorData(QObject *parent) : QObject(parent)
{
    markDirty();
    connect(this, SIGNAL(dataChanged()), this, SLOT(markDirty()));
}

// sample access ///////////////////////////////////////////////////////////////
const Latan::DMatSample & CorrelatorData::sample(const int i) const
{
    return sample_.at(i);
}

const Latan::DMatSample & CorrelatorData::combinedSample(void) const
{
    return combined_;
}

// tests ///////////////////////////////////////////////////////////////////////
bool CorrelatorData::hasCombination(void) const
{
    return hasCombination_;
}

bool CorrelatorData::isDirty(void) const
{
    return isDirty_;
}

// size of the container ///////////////////////////////////////////////////////
int CorrelatorData::size(void) const
{
    return sample_.size();
}

/******************************************************************************
 *                            CorrelatorData slots                            *
 ******************************************************************************/
// load new file, insert at position i /////////////////////////////////////////
void CorrelatorData::load(const int i, const QString filename)
{ 
    Index nt;

    sample_.insert(i, Io::load<DMatSample>(filename.toStdString()));
    nt         = sample_[i][central].rows();
    sample_[i] = sample_[i].block(0, 0, nt, 1);
    emit dataChanged();
}

// unload file at position i ///////////////////////////////////////////////////
void CorrelatorData::unload(const int i)
{
    sample_.removeAt(i);
    emit dataChanged();
}

// set combination function code ///////////////////////////////////////////////
void CorrelatorData::setFunction(const QString code)
{
    code_  = code;
    emit dataChanged();
}

// mark combination to be recomputed ///////////////////////////////////////////
void CorrelatorData::markDirty(void)
{
    isDirty_ = true;
}

// perform combination /////////////////////////////////////////////////////////
void CorrelatorData::combine(void)
{
    const unsigned int n = sample_.size();

    CATCH_WARNING(
    if ((n > 0) and isDirty())
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
        isDirty_        = false;
        hasCombination_ = true;
        emit combinedSampleChanged();
    })
}

