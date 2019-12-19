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
#include <LatAnalyze/Core/Plot.hpp>

class CorrelatorData : public QObject
{
    Q_OBJECT
public:
    explicit CorrelatorData(QObject *parent = nullptr);

public slots:
    void load(const QString &filename);

signals:
    void plotUpdate(const Latan::Plot &p);

private:
    Latan::DMatSample sample_;
    Latan::Plot       p_;
};

#endif // CORRELATORDATA_H
