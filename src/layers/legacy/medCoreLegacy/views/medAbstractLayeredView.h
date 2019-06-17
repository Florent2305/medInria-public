/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medAbstractView.h>

#include <medCoreLegacyExport.h>

#include <dtkCoreSupport/dtkSmartPointer.h>

#include <medAbstractLayeredViewInteractor.h>
#include <medAbstractLayeredViewNavigator.h>
#include <medAbstractData.h>
#include <medAbstractInteractor.h>

class medAbstractData;
class medAbstractBoolParameterL;
class medStringListParameterL;
class medDataListParameterL;

class medAbstractLayeredViewPrivate;
class MEDCORELEGACY_EXPORT medAbstractLayeredView : public medAbstractView
{
    Q_OBJECT

public:
    medAbstractLayeredView(QObject * parent = nullptr);
    ~medAbstractLayeredView() override;

    virtual void addLayer(medAbstractData *data);
    virtual void removeData(medAbstractData *data);
    void removeLayer(unsigned int layer);
    void insertLayer(unsigned int layer, medAbstractData *data);

    medAbstractData * layerData(unsigned int layer) const;
    QList<medDataIndex> dataList() const;

    bool contains(medAbstractData * data) const;
    bool contains(QString identifier) const;

    unsigned int layersCount() const;
    unsigned int layer(medAbstractData * data);

    void setCurrentLayer(unsigned int layer);
    unsigned int currentLayer() const;

    QList <medAbstractInteractor*> interactors() override;
    QList <medAbstractInteractor*> layerInteractors(unsigned int layer);
    QList<medAbstractNavigator*> navigators() override;

    medAbstractBoolParameterL* visibilityParameter(unsigned int layer);
    medDataListParameterL *dataListParameter() const;

    QList<medAbstractParameterL*> linkableParameters() override;
    virtual QList<medAbstractParameterL*> linkableParameters(unsigned int layer);

public slots:
    void setDataList(QList<medDataIndex> dataList);
    void removeLayer();

signals:
    void layerAdded(unsigned int layer);
    void layerRemoved(unsigned int layer);
    void layerRemoved(medAbstractData*);
    void currentLayerChanged() const;

protected:
    medAbstractLayeredViewInteractor * primaryInteractor(medAbstractData* data) override;
    QList<medAbstractInteractor *> extraInteractors(medAbstractData* data) override;
    virtual medAbstractLayeredViewInteractor * primaryInteractor(unsigned int layer);
    virtual QList<medAbstractInteractor *> extraInteractors(unsigned int layer);
    medAbstractLayeredViewInteractor * primaryInteractor() override;
    QList<medAbstractInteractor *> extraInteractors() override;

    medAbstractLayeredViewNavigator * primaryNavigator() override;
    QList<medAbstractNavigator *> extraNavigators() override;

    bool initialiseInteractors(medAbstractData* data) override;
    bool initialiseNavigators() override;
    void removeInteractors(medAbstractData *data) override;

    virtual QList<medAbstractParameterL*> interactorsParameters(unsigned int layer);

private slots:
    void updateDataListParameter(unsigned int layer);

private:
    medAbstractLayeredViewPrivate *d;
};
