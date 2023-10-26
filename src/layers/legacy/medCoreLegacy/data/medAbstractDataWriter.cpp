/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2020. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medAbstractDataWriter.h>
#include <medAbstractData.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

void medAbstractDataWriter::setData(QList<medAbstractData*> data)
{
	dataList = data;
}

void medAbstractDataWriter::setData(dtkAbstractData* data)
{
    dtkAbstractDataWriter::setData(data);
    dataList = QList<medAbstractData*>() << dynamic_cast<medAbstractData*>(data);
}

QList<medAbstractData*> medAbstractDataWriter::getDataList()
{
	return dataList;
}


bool medAbstractDataWriter::writeMetaData(const QString& file)
{
    bool bRes = false;

    medAbstractData * pData = (medAbstractData *) data();

    if (pData)
    {
        QJsonDocument jsonDoc;
        //QJsonArray jsonArrayOfMetaData;
        //QMap <QString, QStringList> metaDataMap;
        QJsonObject jsonObj;
        for (auto key : pData->metaDataList())
        {
            //metaDataMap[key] = pData->metaDataValues(key);
            QJsonValue jsonValue;
            auto val = pData->metaDataValues(key);
            if (val.size() == 1)
            {
                jsonValue = val[0];
            }
            else
            {
                jsonValue = QJsonArray::fromStringList(val);
            }
            jsonObj.insert(key, jsonValue);
            //jsonArrayOfMetaData << jsonObj;
        }
        //jsonDoc.setArray(jsonArrayOfMetaData);
        jsonDoc.setObject(jsonObj);
        

        QFile compagonMetaDataFile(file + ".cmm");
        compagonMetaDataFile.open(QIODevice::ReadWrite);
        bRes = compagonMetaDataFile.write(jsonDoc.toJson()) > 0;
        compagonMetaDataFile.close();
    }

    return bRes;
}
