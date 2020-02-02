/*
 * dataoptiondialog.cpp, part of Plateau
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

#include "dataoptiondialog.h"
#include "ui_dataoptiondialog.h"
#include <QFileDialog>

DataOptionDialog::DataOptionDialog(Option opt, QWidget *parent) :
    QDialog(parent),
    ui_(new Ui::DataOptionDialog)
{
    ui_->setupUi(this);
    setWindowTitle("Edit data");
    ui_->filenameLineEdit->setText(opt.filename);
    ui_->ftCheckBox->setCheckState(opt.tr.ft ? Qt::Checked : Qt::Unchecked);
    ui_->foldCheckBox->setCheckState(opt.tr.fold ? Qt::Checked : Qt::Unchecked);
    ui_->shiftSpinBox->setValue(opt.tr.shift);
}

DataOptionDialog::~DataOptionDialog()
{
    delete ui_;
}

DataOptionDialog::Option DataOptionDialog::getDataOption(Option opt,
                                                         QWidget *parent)
{
    DataOptionDialog *dialog = new DataOptionDialog(opt, parent);
    Option           newOpt = {DataModel::defaultTransform, "", false};

    dialog->exec();
    if (dialog->result() == QDialog::Accepted)
    {
        newOpt.accepted = true;
        newOpt.tr       = dialog->transform();
        newOpt.filename = dialog->filename();
    }

    delete dialog;

    return newOpt;
}

QString DataOptionDialog::filename(void)
{
    return ui_->filenameLineEdit->text();
}

DataModel::Transform DataOptionDialog::transform(void)
{
    DataModel::Transform tr;

    tr.ft    = (ui_->ftCheckBox->checkState() == Qt::Checked);
    tr.fold  = (ui_->foldCheckBox->checkState() == Qt::Checked);
    tr.shift = (ui_->shiftSpinBox->value());

    return tr;
}


void DataOptionDialog::getFilename(void)
{
    QString filename = QFileDialog::getOpenFileName(this, "Select file",
                                                    QDir::homePath(),
                                                    "LatAnalyze sample (*.h5 *.dat);;All Files (*)");

    ui_->filenameLineEdit->setText(filename);
}
