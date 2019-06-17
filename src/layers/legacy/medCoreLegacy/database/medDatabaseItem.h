/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once


#include <QtDebug>
#include <QtGui>

#include <medDataIndex.h>
#include <medAbstractDatabaseItem.h>

#include <medCoreLegacyExport.h>

class medDatabaseItemPrivate;

class MEDCORELEGACY_EXPORT medDatabaseItem : public medAbstractDatabaseItem
{
public:
     medDatabaseItem(medDataIndex index, const QList<QVariant>& attributes, const QList<QVariant>& data, medAbstractDatabaseItem *parent = nullptr);
    ~medDatabaseItem() override;

    medAbstractDatabaseItem *child(int row) override;
    medAbstractDatabaseItem *parent() override;

    void setParent(medAbstractDatabaseItem *parent) override;

    void append(medAbstractDatabaseItem *item) override;

    int row() const override;
    int childCount() const override;
    int childNumber() const override;
    int columnCount() const override;

    QVariant data(int column) const override;

    bool insertChildren(const medDataIndex& index, int position, int count, int columns) override;
    bool insertColumns(int position, int columns) override;

    bool removeChildren(int position, int count,  bool deleteChildren = true) override;
    bool removeColumns(int position, int columns) override;

    bool setData(int column, const QVariant& value) override;

    const medDataIndex & dataIndex () const override;

    void setDataIndex (const medDataIndex &index) override;

    int rowOf(medAbstractDatabaseItem *child) const override;

public:
    QVariant attribute(int column) override;
    QVariant     value(int column) override;

    QList<QVariant> attributes() override;
    QList<QVariant> values() override;

private:
    medDatabaseItemPrivate *d;
};


