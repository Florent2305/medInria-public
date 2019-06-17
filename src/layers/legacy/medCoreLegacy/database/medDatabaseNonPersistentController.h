/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medAbstractDbController.h>
#include <medDataIndex.h>

#include <QtCore/QObject>
#include <QtCore/QList>

#include <medCoreLegacyExport.h>

class medAbstractData;
class medDatabaseNonPersistentItem;
class medDatabaseNonPersistentControllerPrivate;
class medImportJobWatcher;


class MEDCORELEGACY_EXPORT medDatabaseNonPersistentController: public medAbstractDbController
{
    Q_OBJECT

public:
    static medDatabaseNonPersistentController * instance();
    ~medDatabaseNonPersistentController() override;

    int patientId(bool increment=false);
    int   studyId(bool increment=false);
    int  seriesId(bool increment=false);
    int   imageId(bool increment=false);

    int nonPersistentDataStartingIndex() const;

    QList<medDatabaseNonPersistentItem *> items();

    void insert(medDataIndex index, medDatabaseNonPersistentItem *item);

    bool isConnected() const override;

    QList<medDataIndex> availableItems() const;
    QString metaData(const medDataIndex& index, const QString& key) const override;

    int dataSourceId() const override;

    QList<medDataIndex> patients() const override;
    QList<medDataIndex> studies(const medDataIndex& index ) const override;
    QList<medDataIndex> series(const medDataIndex& index ) const override;
    QList<medDataIndex> images(const medDataIndex& index ) const override;

    QPixmap thumbnail(const medDataIndex &index) const override;

    bool isPersistent() const override;

    bool setMetaData(const medDataIndex& index, const QString& key, const QString& value) override;

public slots:
     medAbstractData* retrieve(const medDataIndex& index) const override;

    void importData(medAbstractData *data, const QUuid & callerUuid) override;
    void importPath(const QString& file, const QUuid & callerUuid, bool indexWithoutCopying) override;

    void remove(const medDataIndex& index) override;

    QList<medDataIndex> moveStudy(const medDataIndex& indexStudy, const medDataIndex& toPatient) override;

    medDataIndex moveSerie(const medDataIndex& indexSerie, const medDataIndex& toStudy) override;

    bool contains( const medDataIndex& index) const override;

    void removeAll() override;

private:
    medDatabaseNonPersistentController();

    medDatabaseNonPersistentControllerPrivate *d;
    static medDatabaseNonPersistentController* s_instance;
};


