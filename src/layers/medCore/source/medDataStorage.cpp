/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medDataStorage.h>

#include <dtkCoreSupport/dtkSmartPointer.h>

class medDataStoragePrivate
{
public:
    QMap<medDataIndex, dtkSmartPointer<medAbstractData>> cacheMap;
    QMap < medDataIndex, QMetaObject::Connection>    connectionMap;
    QMap < medDataIndex, int>    dataSizeMap;
    QMap < medDataIndex, qint64> insertionTimeMap;
    QMap < medDataIndex, qint64> accessTimeMap;

    int maxEntriesExpected;
    int maxMemoryExpected;
    
    //QMutex mutex;
    QReadWriteLock mutex;

    cachePolicy policy;
};

QList<medDataIndex> dummyPolicy (medDataStorage * storage)
{
    QList<medDataIndex> indexToRemoveList;

    auto indexList = storage->getIndexList();
    for (auto index : indexList)
    {
        int refCount = 0;
        int sizeMB = -1;
        qint64 insertionTime = 0;
        qint64 accessTime = 0;
        if (storage->getIndexInfo(index, refCount, sizeMB, insertionTime, accessTime))
        {
            if (refCount < 2 && insertionTime + 120 < QDateTime::currentSecsSinceEpoch())
            {
                indexToRemoveList << index;
            }
        }
    }

    return indexToRemoveList;
}


medDataStorage::medDataStorage(QObject *parent)
{
    d = new medDataStoragePrivate;

    d->policy = &dummyPolicy;
    d->maxEntriesExpected = 10;
    d->maxMemoryExpected = 2048;


    QTimer *timer = new QTimer(this);
    timer->setInterval(10000);
    connect(timer, &QTimer::timeout, this, &medDataStorage::clean);
    timer->start();
}
medDataStorage::~medDataStorage()
{
    delete d;
}

medAbstractData * medDataStorage::getData(medDataIndex const & index)
{
    medAbstractData * dataRes = nullptr;

    d->mutex.lockForRead();
    dataRes = d->cacheMap.value(index);
    if (dataRes)
    {
        d->accessTimeMap[index] = QDateTime::currentSecsSinceEpoch();
    }
    d->mutex.unlock();

    return dataRes;
}

void medDataStorage::setMaxCacheEntry(int max)
{
    d->maxEntriesExpected = max;
}

void medDataStorage::setMaxMemoryUsage(int sizeInMBytes)
{
    d->maxMemoryExpected = sizeInMBytes;
}

void medDataStorage::setCachePolicy(cachePolicy policy)
{
    d->policy = policy;
}

QList<medDataIndex> medDataStorage::getIndexList() const
{
    QList<medDataIndex> resIndexList;

    d->mutex.lockForRead();
    resIndexList = d->cacheMap.keys();
    d->mutex.unlock();

    return resIndexList;
}

bool medDataStorage::getIndexInfo(medDataIndex index, int & refCount, int & sizeMB, qint64 & insertionTimeMap, qint64 & accessTimeMap)
{
    bool bRes = false;

    d->mutex.lockForRead();
    if (d->cacheMap.contains(index))
    {
        refCount         = d->cacheMap[index].refCount();
        insertionTimeMap = d->insertionTimeMap[index];
        accessTimeMap    = d->accessTimeMap[index];
        if (d->dataSizeMap.contains(index))
        {
            sizeMB = d->dataSizeMap[index];
        }
        bRes = true;
    }
    d->mutex.unlock();

    return bRes;
}

int medDataStorage::getMaxCacheEntry() const
{
    return d->maxEntriesExpected;
}

int medDataStorage::getMaxMemoryUsage(int sizeInMBytes) const
{
    return d->maxMemoryExpected;
}

void medDataStorage::addData(medDataIndex index, medAbstractData *data)
{
    addDataWithSize(index, data, -1);
}

void medDataStorage::addDataWithSize(medDataIndex index, medAbstractData * data, int size)
{
    d->mutex.lockForWrite();
    noLockAddData(index, data, size);
    d->mutex.unlock();
}

void medDataStorage::removeData(medDataIndex index)
{
    d->mutex.lockForWrite();
    if (d->cacheMap.contains(index))
    {
        noLockRemoveData(index);
    }
    d->mutex.unlock();
}

void medDataStorage::clean()
{
    auto listToRemove = d->policy(this);

    d->mutex.lockForWrite();
    for (auto index : listToRemove)
    {
        if (d->cacheMap.contains(index) && d->cacheMap[index].refCount() == 1)
        {
            noLockRemoveData(index);
        }
    }
    d->mutex.unlock();
}

void medDataStorage::hardClean()
{
    d->mutex.lockForWrite();
    auto keys = d->cacheMap.keys();
    for (auto key : keys)
    {
        if (d->cacheMap[key].refCount() == 1 && d->insertionTimeMap[key]+600 < QDateTime::currentSecsSinceEpoch())
        {
            noLockRemoveData(key);
        }
    }
    d->mutex.unlock();
}

void medDataStorage::noLockRemoveData(medDataIndex index)
{
    disconnect(d->connectionMap[index]);

    d->connectionMap.remove(index);
    d->cacheMap.remove(index);
    d->insertionTimeMap.remove(index);
    d->accessTimeMap.remove(index);
    if (d->dataSizeMap.contains(index))
    {
        d->dataSizeMap.remove(index);
    }
}

void medDataStorage::noLockAddData(medDataIndex index, medAbstractData *data, int size)
{
    if (!d->cacheMap.contains(index))
    {
        auto time = QDateTime::currentSecsSinceEpoch();
        d->connectionMap[index] = connect(data, &medAbstractData::medDataRemoved, this, &medDataStorage::removeData);
        d->cacheMap[index] = data;
        d->insertionTimeMap[index] = time;
        d->accessTimeMap[index] = time;
        if (size > -1)
        {
            d->dataSizeMap[index] = size;
        }
    }
}
