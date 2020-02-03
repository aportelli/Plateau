/*
 * correlatordata.h, part of Plateau
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

#ifndef CORRELATORDATA_H
#define CORRELATORDATA_H

#include <QObject>
#include <LatAnalyze/Statistics/MatSample.hpp>

/******************************************************************************
 *                      Container for loaded correlator                       *
 ******************************************************************************/
class CorrelatorData : public QObject
{
    Q_OBJECT

public:
    // constructor
    explicit CorrelatorData(QObject *parent = nullptr);
    // sample access
    const Latan::DMatSample & sample(const int i) const;
    const Latan::DMatSample & combinedSample(void) const;
    // tests
    bool hasCombination(void) const;
    bool isDirty(void) const;
    // size of the container
    int size(void)const;
    // load new file, insert at position i
    void load(const int i, const QString filename);
    // unload file at position i
    void unload(const int i);
    // set combination function code
    void setFunction(const QString code);
    // time shift
    void shift(const int i, const int ts);
    // time fold
    void fold(const int i);
    // Fourier transform
    void fourierTransform(const int i);
    // perform combination
    void combine(void);

public slots:
    // mark combination to be recomputed
    void markDirty(void);

signals:
    void dataChanged(void);
    void combinedSampleChanged(void);

private:
    bool                     isDirty_, hasCombination_{false};
    QList<Latan::DMatSample> sample_;
    Latan::DMatSample        combined_;
    QString                  code_;
};

#endif // CORRELATORDATA_H
