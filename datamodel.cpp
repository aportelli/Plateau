#include "datamodel.h"
#include <QFileInfo>
#include <QDebug>

DataModel::DataModel(QObject *parent)
    : QAbstractTableModel(parent)
{}

int DataModel::rowCount(const QModelIndex &) const
{
    return list_.size();
}

int DataModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant DataModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
        {
            return index.row();
        }
        if (index.column() == 1)
        {
            QFileInfo fileInfo(list_.at(index.row()));

            return fileInfo.fileName();
        }
        if (index.column() == 2)
        {
            return list_.at(index.row());
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
        list_.insert(row, "");
    }
    endInsertRows();

    return true;
}

bool DataModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
    {
        list_.removeAt(row);
    }
    endRemoveRows();

    return true;
}

void DataModel::addFile(const QString filename)
{
    int i = list_.indexOf(filename);

    if (i == -1)
    {
        int last = list_.size();

        insertRows(last, 1);
        list_[last] = filename;
    }

    QModelIndex topLeft = createIndex(0,0),
                bottomRight = createIndex(list_.size() - 1, 1);

    emit dataChanged(topLeft, bottomRight);
}

void DataModel::removeFile(const QString filename)
{
    int i = list_.indexOf(filename);

    if (i != -1)
    {
        removeRows(i, 1);
    }

    QModelIndex topLeft = createIndex(0,0),
                bottomRight = createIndex(list_.size() - 1, 1);

    emit dataChanged(topLeft, bottomRight);
}

QString DataModel::filepath(const int i) const
{
    return data(index(i, 2)).toString();
}

QString DataModel::filename(const int i) const
{
    return data(index(i, 1)).toString();
}
