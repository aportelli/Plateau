/*
 * datamodel.cpp, part of Plateau
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

#include "DataTableModel.h"
#include <src/Global.h>
#include <QFileInfo>
#include <QDebug>

constexpr DataTableModel::Transform DataTableModel::defaultTransform;

/******************************************************************************
 *                           DataModel methods                                *
 ******************************************************************************/
// constructor /////////////////////////////////////////////////////////////////
DataTableModel::DataTableModel(CorrelatorData *data, QObject *parent)
    : QAbstractTableModel(parent)
    , data_(data)
{}

// QAbstractTableModel subclassing /////////////////////////////////////////////
int DataTableModel::rowCount(const QModelIndex &) const
{
    return fileList_.size();
}

int DataTableModel::columnCount(const QModelIndex &) const
{
    return 4;
}

QVariant DataTableModel::data(const QModelIndex &index, int role) const
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

QVariant DataTableModel::headerData(int section, Qt::Orientation orientation,
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

Qt::ItemFlags DataTableModel::flags(const QModelIndex &index) const
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

bool DataTableModel::insertRows(int row, int count, const QModelIndex &parent)
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

bool DataTableModel::removeRows(int row, int count, const QModelIndex &parent)
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
void DataTableModel::addFile(const QString filename, const Transform tr)
{
    int i = fileList_.indexOf(filename);

    if (i == -1)
    {
        if (QFile::exists(filename))
        {
            int last = rowCount();

            insertRows(last, 1);
            fileList_[last] = filename;
            i               = last;
            data_->load(i, filename);
            transform(filename, tr);

            QModelIndex topLeft = createIndex(0, 0),
                        bottomRight = createIndex(rowCount() - 1, columnCount() - 1);

            emit dataChanged(topLeft, bottomRight);
        }
    }
}

void DataTableModel::addFiles(const QStringList &list)
{
    for (int i = 0; i < list.size(); ++i)
    {
        addFile(list.at(i));
    }
}

void DataTableModel::editFile(const QString filename, const QString newFilename,
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

void DataTableModel::removeFile(const QString filename)
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
void DataTableModel::transform(const QString filename, const Transform tr)
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
void DataTableModel::clear(void)
{
    while (!fileList_.empty())
    {
        removeFile(fileList_.front());
    }
}

// get file name & path ////////////////////////////////////////////////////////
QString DataTableModel::getFilepath(const int i) const
{
    return data(index(i, 2)).toString();
}

QString DataTableModel::getFilename(const int i) const
{
    return data(index(i, 1)).toString();
}

DataTableModel::Transform DataTableModel::getTransform(const int i) const
{
    return trList_.at(i);
}

// get file list ///////////////////////////////////////////////////////////////
const QStringList & DataTableModel::getFileList() const
{
    return fileList_;
}

// direct data access //////////////////////////////////////////////////////////
CorrelatorData * DataTableModel::data(void)
{
    return data_;
}

/******************************************************************************
 *                             Serialisation                                  *
 ******************************************************************************/
QDataStream &operator<<(QDataStream &s, const DataTableModel &d)
{
    quint32 size = d.fileList_.size();

    s << size;
    for (quint32 i = 0; i < size; ++i)
    {
        s << d.fileList_.at(i);
        s << d.trList_.at(i).ft;
        s << d.trList_.at(i).fold;
        s << static_cast<quint32>(d.trList_.at(i).shift);
    }

    return s;
}

QDataStream &operator>>(QDataStream &s, DataTableModel &d)
{
    quint32 size;

    d.clear();
    s >> size;
    for (quint32 i = 0; i < size; ++i)
    {
        DataTableModel::Transform tr;
        QString                   filename;
        quint32                   buf;

        s >> filename;
        s >> tr.ft;
        s >> tr.fold;
        s >> buf; tr.shift = buf;
        d.addFile(filename, tr);
    }

    return s;
}

/******************************************************************************
 *                           Transform compare                                *
 ******************************************************************************/
bool operator==(const DataTableModel::Transform a, const DataTableModel::Transform b)
{
    return ((a.fold == b.fold) and (a.ft == b.ft) and (a.shift == b.shift));
}

bool operator!=(const DataTableModel::Transform a, const DataTableModel::Transform b)
{
    return !(a == b);
}
