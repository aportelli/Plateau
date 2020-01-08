/*
 * correlatordata.h, part of Plateau
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

#ifndef CORRELATORDATA_H
#define CORRELATORDATA_H

#include <QObject>
#include <LatAnalyze/Statistics/MatSample.hpp>

class CorrelatorData : public QObject
{
    Q_OBJECT

public:
    explicit CorrelatorData(QObject *parent = nullptr);
    const Latan::DMatSample & sample(const int i);
    const Latan::DMatSample & combinedSample(void);
    bool hasCombination(void);
    bool isClean(void);
    int  size(void);

public slots:
    void load(const int i, const QString filename);
    void unload(const int i);
    void setFunction(const QString code);
    void markDirty(void);
    void combine(void);

signals:
    void dataChanged(void);
    void combinedSampleChanged(void);

private:
    bool                     isClean_, hasCombination_{false};
    QList<Latan::DMatSample> sample_;
    Latan::DMatSample        combined_;
    QString                  code_;
};

#endif // CORRELATORDATA_H
