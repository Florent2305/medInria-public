/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include <medAbstractDbController.h>

#include <medCoreLegacyExport.h>

class medAbstractData;
class medDatabaseControllerPrivate;
class medJobItemL;

/**
 * Concrete dbController implementation adhering to abstract base class
 */
class MEDCORELEGACY_EXPORT medDatabaseController: public medAbstractDbController
{
    Q_OBJECT

public:
    static medDatabaseController* instance();
    ~medDatabaseController() override;

    const QSqlDatabase& database() const;

    bool createConnection();
    bool  closeConnection();

    medDataIndex indexForPatient(int id);
    medDataIndex indexForStudy  (int id);
    medDataIndex indexForSeries (int id);
    medDataIndex indexForImage  (int id);


    medDataIndex indexForPatient (const QString &patientName);
    medDataIndex indexForStudy   (const QString &patientName, const QString &studyName);
    medDataIndex indexForSeries  (const QString &patientName, const QString &studyName,
                                  const QString &seriesName);
    medDataIndex indexForImage   (const QString &patientName, const QString &studyName,
                                  const QString &seriesName,  const QString &imageName);

    QString stringForPath(const QString & name) const;

    bool moveDatabase(QString newLocation);

    bool isConnected() const override;

    QList<medDataIndex> patients() const override;
    QList<medDataIndex> studies(const medDataIndex& index ) const override;
    QList<medDataIndex> series(const medDataIndex& index) const override;
    QList<medDataIndex> images(const medDataIndex& index ) const override;
    QPixmap thumbnail(const medDataIndex& index) const override;

    QString metaData(const medDataIndex& index,const QString& key) const override;
    bool setMetaData(const medDataIndex& index, const QString& key, const QString& value) override;

    bool isPersistent() const override;

public slots:

    medAbstractData *retrieve(const medDataIndex &index) const override;

    void importPath(const QString& file, const QUuid& importUuid, bool indexWithoutCopying = false) override;
    void importData(medAbstractData *data, const QUuid & importUuid) override;

    void remove(const medDataIndex& index) override;

    QList<medDataIndex> moveStudy(const medDataIndex& indexStudy, const medDataIndex& toPatient) override;
    medDataIndex moveSerie(const medDataIndex& indexSerie, const medDataIndex& toStudy) override;

    int dataSourceId() const override;

     bool contains(const medDataIndex &index) const override;

     void removeAll() override;

protected slots:
    void showOpeningError(QObject *sender);

private:
    medDatabaseController();

    void createPatientTable();
    void   createStudyTable();
    void  createSeriesTable();
    void   createImageTable();

    QSqlDatabase m_database;

    medDatabaseControllerPrivate * d;
    static medDatabaseController * s_instance;
};
