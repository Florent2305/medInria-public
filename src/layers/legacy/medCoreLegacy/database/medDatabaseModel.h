/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <QtCore/QAbstractItemModel>
#include <QtGui>

#include <medCoreLegacyExport.h>

class medAbstractDatabaseItem;
class medDatabaseModelPrivate;
class medDataIndex;

class MEDCORELEGACY_EXPORT medDatabaseModel : public QAbstractItemModel
{
    Q_OBJECT

public:
     medDatabaseModel(QObject *parent = nullptr, bool justBringStudies = false);
    ~medDatabaseModel() override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnIndex(const QString& title) const;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int colum, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant& value, int role = Qt::EditRole) override;

    bool insertColumns(int position, int columns, const QModelIndex& parent = QModelIndex()) override;
    bool removeColumns(int position, int columns, const QModelIndex& parent = QModelIndex()) override;

    bool insertRows(int position, int rows, const QModelIndex& parent = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex& parent = QModelIndex()) override;
    bool removeAllRows();

    QStringList mimeTypes() const override;

    Qt::DropActions supportedDropActions() const override;

    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex& parent) override;

    QMimeData *mimeData(const QModelIndexList &indexes) const override;

    QStringList columnNames() const;

    bool hasChildren ( const QModelIndex & parent = QModelIndex() ) const override;

protected slots:
    void repopulate();

protected:
    void populate(medAbstractDatabaseItem *root);

private:
    medDatabaseModelPrivate *d;
public slots:
    void update(const medDataIndex &dataIndex);

private:
    void updateSerie(const medDataIndex &dataIndex);
    void updateStudy(const medDataIndex &dataIndex, bool updateChildren = true);
    void updatePatient(const medDataIndex &dataIndex, bool updateChildren = true);
    QVariant convertQStringToQVariant(QString key, QString value);
    void changePersistenIndexAndSubIndex(QModelIndex index);
};


