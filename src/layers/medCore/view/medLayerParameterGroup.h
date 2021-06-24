#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2020. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medAbstractParameterGroup.h>
#include <medAbstractData.h>

class medLayerParameterGroupPrivate;

class MEDCORE_EXPORT medLayerParameterGroup : public medAbstractParameterGroup
{
    Q_OBJECT

public:
    medLayerParameterGroup(QString name = "", QObject *parent = nullptr, QString workspace = "");
    virtual ~medLayerParameterGroup();

    void addImpactedlayer(medAbstractLayeredView *view, medAbstractData* layerData);
    void removeImpactedlayer(medAbstractLayeredView *view, medAbstractData* layerData);
    QMultiHash<medAbstractLayeredView*, medAbstractData*> impactedLayers();

    void setLinkAllParameters(bool linkAll);

protected:
    virtual void updatePool();
    void updateParameterToLinkList(medAbstractLayeredView *view, medAbstractData* layerData);

private slots:
    void removeImpactedlayer(medAbstractData*);

private:
    medLayerParameterGroupPrivate *d;
};