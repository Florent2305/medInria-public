#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medCoreExport.h>

#include <medAbstractData.h>
#include <medAbstractDataWriter.h>

/**
 * @class   medDataExporter
 * @author  Florent Leray
 * @date    13/10/2021
 *          
 * @brief   medDataExporter write data to file-system and converts them from medAbstractData.
 *          
 */
class MEDCORE_EXPORT medDataExporter : public QObject
{
    Q_OBJECT

public:	
    medDataExporter(QObject *parent = nullptr);
    ~medDataExporter();

    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Conversion functions from path to medAbstractData
    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static bool convertSingleDataOnfly(medAbstractData * data, QString &path);
    static bool convertSingleDataOnfly(medAbstractData * data, QString &path, QStringList fileExts);
    //medAbstractData *        convertSingleData(QString path);
    //QList<medAbstractData*>  convertMultipData(QString path);

    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Control functions 
    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //medAbstractData * convertWithOtherReader(medAbstractData *&data, QString readerId = QString());
    //bool              releaseWriters(medAbstractData *data = nullptr);
    //bool              forgotData(medAbstractData *data = nullptr);

    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Getters functions 
    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //QList<medAbstractData*> getData();
    //medAbstractDataReader*  getCurrentWriterInstance(QString volumeId = QString());
    //medAbstractDataReader*  getCurrentWriterInstance(medAbstractData *data = nullptr);
    //QString                 getCurrentWriter(QString volumeId = QString());
    //QString                 getCurrentWriter(medAbstractData *data = nullptr);
    //QStringList             getAvailableWriters(QString volumeId = QString());
    static QStringList               getAvailableWriters(medAbstractData *data = nullptr);
    static QStringList               getExtensionForWriter(QString writerId);
    //QStringList             getPaths(QString volumeId = QString());
    //QStringList             getPaths(medAbstractData *data = nullptr);

    typedef struct writerInfo 
    { 
        QString Id; 
        QString descr; 
        QStringList dataType; 
        QStringList File;
    };
    static QList <writerInfo> getWriterInfoList(QStringList writers = QStringList());


private:
    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Private Static functions to:             list files,   getReaders,   readFiles,         opt find volumes
    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static QList<medAbstractDataWriter*>  getSuitableWriter(medAbstractData* medData, QString filePath, QStringList fileExts = QStringList(), QStringList *writersId = nullptr);
    //static medAbstractData * readFiles(QList<medAbstractDataReader *> &readers, QStringList &fileList, medAbstractDataReader **usedReader = nullptr);
    //void findVolumesInDirectory(QString &path);

    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Private tools functions 
    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //void reset();
    //inline bool getVolumeFromData(medAbstractData*, QString & volume);
    //template<typename T> T get(QString volumeId, const QMap<QString, T>& map);

    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Other 
    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //static QString createVolumeId(medAbstractData* data); //TODO move into the medAbstractReader with virtual but not pure to have default impl
signals:


private:
    //QMap <QString, QStringList>            m_pathsVolumesMap;
    //QMap <QString, QStringList>            m_availablesReadersVolumesMap;
    //QMap <QString, medAbstractData*>       m_meddataVolumesMap;
    //QMap <QString, medAbstractDataReader*> m_currentReaderVolumesMap;

    QMap <QString, QPair<QStringList, QStringList>> m_writerIdsDataTypesExtsMaps;
};

/*
template<typename T>
inline T medDataImporter::get(QString volumeId, const QMap<QString, T>& map)
{
    T *res = nullptr;
    
    if (volumeId.isEmpty() && !map.isEmpty())
    {
        res = map.first();
    }
    else if (map.contains(volumeId))
    {
        res = map[volumeId];
    }

    return res;
}

medAbstractDataReader*  medDataImporter::getCurrentReaderInstance(QString volumeId) { return get(volumeId, m_currentReaderVolumesMap); }
QString                 medDataImporter::getCurrentReader(QString volumeId)         { return get(volumeId, m_currentReaderVolumesMap)->identifier(); }
QStringList             medDataImporter::getAvailableReader(QString volumeId)       { return get(volumeId, m_availablesReadersVolumesMap); }
QStringList             medDataImporter::getPaths(QString volumeId)                 { return get(volumeId, m_pathsVolumesMap); }
*/

