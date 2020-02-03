/*
 * dataoptiondialog.h, part of Plateau
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

#ifndef DATAOPTIONDIALOG_H
#define DATAOPTIONDIALOG_H

#include <QDialog>
#include <src/data/datatablemodel.h>

namespace Ui {
class DataOptionDialog;
}

class DataOptionDialog : public QDialog
{
    Q_OBJECT

public:
    struct Option
    {
        DataTableModel::Transform tr;
        QString              filename;
        bool                 accepted;
    };

public:
    explicit DataOptionDialog(Option opt, QWidget *parent = nullptr);
    ~DataOptionDialog();
    static Option getDataOption(Option opt,
                                QWidget *parent = nullptr);

    QString              filename(void);
    DataTableModel::Transform transform(void);

public slots:
    void getFilename(void);

private:
    Ui::DataOptionDialog *ui_;
};

#endif // DATAOPTIONDIALOG_H
