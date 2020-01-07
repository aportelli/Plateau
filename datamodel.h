#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QAbstractTableModel>

class DataModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    DataModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    void addFile(const QString filename);
    void removeFile(const QString filename);
    QString filepath(const int i) const;
    QString filename(const int i) const;
private:
    QStringList list_;
};

#endif // DATAMODEL_H
