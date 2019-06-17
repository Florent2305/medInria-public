/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medDataIndex.h>

#include <QtCore/QObject>
#include <QtCore/QList>

#include <medCoreLegacyExport.h>

class medAbstractData;

class medDatabaseNonPersistentItemPrivate;

class MEDCORELEGACY_EXPORT medDatabaseNonPersistentItem : public QObject
{
    Q_OBJECT

public:
     medDatabaseNonPersistentItem();
    ~medDatabaseNonPersistentItem() override;

    const QString& name() const;
    const QString& birthdate() const;
    const QString& patientId() const;
    const QString& studyName() const;
    const QString& seriesName() const;
    const QString& file() const;
    const QString& studyId() const;
    const QString& studyUid() const;
    const QString& seriesId() const;
    const QString& seriesUid() const;
    const QImage& thumb() const;
    const QString orientation() const;
    const QString seriesNumber() const;
    const QString sequenceName() const;
    const QString sliceThickness() const;
    const QString rows() const;
    const QString columns() const;

    const medDataIndex& index() const;

    medAbstractData *data();


    void setName(const QString &name);
    void setBirthdate(const QString &birthdate);
    void setPatientId(const QString &patientId);
    void setStudyName(const QString &studyName);
    void setSeriesName(const QString &seriesName);
    void setFile(const QString &file);
    void setStudyId(const QString &studyId);
    void setStudyUid(const QString &studyUid);
    void setSeriesId(const QString &seriesId);
    void setSeriesUid(const QString &seriesUid);
    void setThumb(const QImage &thumb);

    void setIndex(const medDataIndex &index);

    void setData(medAbstractData *data);

    bool Match(medAbstractData *medData);

private:
    friend class medDatabaseNonPersistentControllerImpl;
    friend class medDatabaseNonPersistentImporter;

private:
    medDatabaseNonPersistentItemPrivate *d;
};

