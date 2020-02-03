/*
 * datamodel.cpp, part of Plateau
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

#include "datamodel.h"
#include <QFileInfo>
#include <QDebug>
#include "global.h"

constexpr DataModel::Transform DataModel::defaultTransform;

/******************************************************************************
 *                           DataModel methods                                *
 ******************************************************************************/
// constructor /////////////////////////////////////////////////////////////////
DataModel::DataModel(CorrelatorData *data, QObject *parent)
    : QAbstractTableModel(parent)
    , data_(data)
{}

// QAbstractTableModel subclassing /////////////////////////////////////////////
int DataModel::rowCount(const QModelIndex &) const
{
    return fileList_.size();
}

int DataModel::columnCount(const QModelIndex &) const
{
    return 4;
}

QVariant DataModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
        {
            return index.row();
        }
        else if (index.column() == 1)
        {
            QFileInfo fileInfo(fileList_.at(index.row()));

            return fileInfo.fileName();
        }
        else if (index.column() == 2)
        {
            return fileList_.at(index.row());
        }
        else if (index.column() == 3)
        {
            QString   trString = "";
            Transform tr = trList_.at(index.row());

            if (tr == defaultTransform)
            {
                trString = "none";
            }
            else
            {
                if (tr.shift != 0)
                {
                    trString += "shift:" + QString::number(tr.shift) + " ";
                }
                if (tr.fold)
                {
                    trString += "fold ";
                }
                if (tr.ft)
                {
                    trString += "ft";
                }
                trString = trString.trimmed();
            }

            return trString;
        }
    }

    return QVariant();
}

QVariant DataModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0:
            return QString("index");
        case 1:
            return QString("file");
        case 2:
            return QString("path");
        case 3:
            return QString("transform");
        }
    }

    return QVariant();
}

Qt::ItemFlags DataModel::flags(const QModelIndex &index) const
{
    if (index.column() == 0)
    {
        return Qt::ItemIsEnabled;
    }
    else
    {
        return Qt::ItemIsSelectable|Qt::ItemIsEnabled;
    }
}

bool DataModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);

    for (int i = 0; i < count; ++i)
    {
        fileList_.insert(row, "");
        trList_.insert(row, defaultTransform);
    }

    endInsertRows();

    return true;
}

bool DataModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);

    for (int i = 0; i < count; ++i)
    {
        fileList_.removeAt(row);
        trList_.removeAt(row);
    }

    endRemoveRows();

    return true;
}

// add/remove files ////////////////////////////////////////////////////////////
void DataModel::addFile(const QString filename, const Transform tr)
{
    int i = fileList_.indexOf(filename);

    if (i == -1)
    {
        if (QFile::exists(filename))
        {
            int last = rowCount();

            insertRows(last, 1);
            fileList_[last] = filename;
            trList_[last]   = tr;
            i               = last;
            data_->load(i, filename);
            transform(filename, tr);

            QModelIndex topLeft = createIndex(0, 0),
                        bottomRight = createIndex(rowCount() - 1, columnCount() - 1);

            emit dataChanged(topLeft, bottomRight);
        }
    }
}

void DataModel::addFiles(const QStringList &list)
{
    for (int i = 0; i < list.size(); ++i)
    {
        addFile(list.at(i));
    }
}

void DataModel::editFile(const QString filename, const QString newFilename,
                         const Transform tr)
{
    int i = fileList_.indexOf(filename);

    if (i != -1)
    {
        if (filename != newFilename)
        {
            fileList_[i] = newFilename;
            trList_[i]   = defaultTransform;
            data_->load(i, newFilename);
        }
        transform(newFilename, tr);
    }
}

void DataModel::removeFile(const QString filename)
{
    int i = fileList_.indexOf(filename);

    if (i != -1)
    {
        removeRows(i, 1);
        data_->unload(i);

        QModelIndex topLeft = createIndex(0, 0),
                    bottomRight = createIndex(rowCount() - 1, columnCount() - 1);

        emit dataChanged(topLeft, bottomRight);
    }
}

// transform correlator ////////////////////////////////////////////////////////
void DataModel::transform(const QString filename, const Transform tr)
{
    int i = fileList_.indexOf(filename);

    if (i != -1)
    {
        if (tr != trList_.at(i))
        {
            if (trList_.at(i) != defaultTransform)
            {
                data_->load(i, filename);
            }
            if (tr.shift != 0)
            {
                data_->shift(i, tr.shift);
            }
            if (tr.fold)
            {
                data_->fold(i);
            }
            if (tr.ft)
            {
                data_->fourierTransform(i);
            }
            trList_[i] = tr;

            QModelIndex topLeft = createIndex(i, 0),
                        bottomRight = createIndex(i, columnCount() - 1);

            emit dataChanged(topLeft, bottomRight);
        }
    }
}

// clear all data //////////////////////////////////////////////////////////////
void DataModel::clear(void)
{
    while (!fileList_.empty())
    {
        removeFile(fileList_.front());
    }
}

// get file name & path ////////////////////////////////////////////////////////
QString DataModel::getFilepath(const int i) const
{
    return data(index(i, 2)).toString();
}

QString DataModel::getFilename(const int i) const
{
    return data(index(i, 1)).toString();
}

DataModel::Transform DataModel::getTransform(const int i) const
{
    return trList_.at(i);
}

// get file list ///////////////////////////////////////////////////////////////
const QStringList & DataModel::getFileList() const
{
    return fileList_;
}

// direct data access //////////////////////////////////////////////////////////
CorrelatorData * DataModel::data(void)
{
    return data_;
}

/******************************************************************************
 *                           Transform compare                                *
 ******************************************************************************/
bool operator==(const DataModel::Transform a, const DataModel::Transform b)
{
    return ((a.fold == b.fold) and (a.ft == b.ft) and (a.shift == b.shift));
}

bool operator!=(const DataModel::Transform a, const DataModel::Transform b)
{
    return !(a == b);
}
