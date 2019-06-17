/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <QtCore/QObject>

#include <medDataIndex.h>
#include <medJobItemL.h>

#include <medCoreLegacyExport.h>

class medDatabaseRemoverPrivate;

/**
 * @class medDatabaseRemover
 * @brief Removes given data from the database.
 */
class MEDCORELEGACY_EXPORT medDatabaseRemover : public medJobItemL
{
    Q_OBJECT

public:
     medDatabaseRemover(const medDataIndex &index);
    ~medDatabaseRemover() override;


signals:

    /**
    * Signal emitted when the removing process finishes.
    * @param index – the @medDataIndex of the removed item
    **/
    void removed(const medDataIndex &index);

public slots:
    void onCancel(QObject *obj) override;

protected:
    void internalRun() override;

    void removeImage(int patientDbId, int studyDbId, int seriesDbId, int imageId);


    bool isSeriesEmpty( int seriesDbId );
    void removeSeries(int patientDbId, int studyDbId, int seriesDbId);
    bool isStudyEmpty( int studyDbId);
    void removeStudy(int patientDbId, int studyDbId);
    bool isPatientEmpty( int patientDbId);
    void removePatient( int patientDbId);

    void removeFile( const QString & filename );

    void removeDataFile( const medDataIndex &index, const QString & filename );
    bool removeTableRow( const QString &table, int id );

private:
    medDatabaseRemoverPrivate *d;
};


