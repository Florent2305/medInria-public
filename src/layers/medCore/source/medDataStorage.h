#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2023. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medCoreExport.h>

#include <QMap>
#include <QList>
#include <QMutex>

#include <medDataIndex.h>
#include <medAbstractData.h>

class medDataStorage;
class medDataStoragePrivate;
using cachePolicy = QList<medDataIndex> (*) (medDataStorage*);

class  medDataStorage : public QObject
{
    Q_OBJECT

public:
    medDataStorage(QObject *parent = nullptr);
    ~medDataStorage();

    medAbstractData * getData(medDataIndex const & index);
    void setMaxCacheEntry(int max);
    void setMaxMemoryUsage(int sizeInMBytes);
    void setCachePolicy(cachePolicy policy);

    QList<medDataIndex> getIndexList() const;
    bool getIndexInfo(medDataIndex index, int & refCount, int & sizeMB, qint64 & insertionTimeMap, qint64 & accessTimeMap);
    int  getMaxCacheEntry() const;
    int  getMaxMemoryUsage(int sizeInMBytes) const;


public slots:
    void addData(medDataIndex index, medAbstractData *data);
    void addDataWithSize(medDataIndex index, medAbstractData *data, int size);
   void removeData(medDataIndex index);
   void clean();
   void hardClean();

signals:
    void dataRemoved(medDataIndex index);


private:
    void noLockRemoveData(medDataIndex index);
    void noLockAddData(medDataIndex index, medAbstractData *data, int size = -1);

private:
    medDataStoragePrivate * d;
};
