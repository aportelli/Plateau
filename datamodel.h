/*
 * datamodel.h, part of Plateau
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

#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QAbstractTableModel>
#include "correlatordata.h"

/******************************************************************************
 *                       Model for correlator data table                      *
 ******************************************************************************/
class DataModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    struct Transform
    {
        bool         fold, ft;
        unsigned int shift;
    };

public:
    // constructor
    DataModel(CorrelatorData *data, QObject *parent = nullptr);
    // QAbstractTableModel subclassing
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    // add/remove files
    void addFile(const QString getFilename, const Transform tr = defaultTransform);
    void addFiles(const QStringList &list);
    void editFile(const QString getFilename, const QString newFilename,
                  const Transform tr = defaultTransform);
    void removeFile(const QString getFilename);
    // transform correlator
    void transform(const QString getFilename, const Transform tr);
    // clear all data
    void clear(void);
    // get file name & path
    QString   getFilepath(const int i) const;
    QString   getFilename(const int i) const;
    // get transform
    Transform getTransform(const int i) const;
    // get file list
    const QStringList & getList() const;
    // direct data access
    CorrelatorData * data(void);

public:
    static constexpr Transform  defaultTransform = {false, false, 0};

private:
    QStringList             fileList_;
    QList<Transform>        trList_;
    CorrelatorData          *data_;
};

// transform compare
bool operator==(const DataModel::Transform a, const DataModel::Transform b);
bool operator!=(const DataModel::Transform a, const DataModel::Transform b);

#endif // DATAMODEL_H
