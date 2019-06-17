/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medAbstractLayeredView.h>

#include <medAbstractImageViewInteractor.h>
#include <medAbstractImageViewNavigator.h>
#include <medImageViewEnum.h>

#include <medCoreLegacyExport.h>

class QVector3D;

class medAbstractData;

class medAbstractImageViewInteractor;
class medAbstractInteractor;

class medAbstractImageViewNavigator;
class medAbstractNavigator;

class medDoubleParameterL;
class medCompositeParameterL;
class medTriggerParameterL;
class medAbstractVector3DParameterL;

class medAbstractImageViewPrivate;

/**
 * @class medAbstractImageView
 * @brief Base class for image view types in medInria
 * medAbstractImageView specializes a medAbstractLayeredView.
 **/
class MEDCORELEGACY_EXPORT medAbstractImageView: public medAbstractLayeredView
{
    Q_OBJECT

public:
    medAbstractImageView(QObject * parent = nullptr);
    ~medAbstractImageView() override;

    void removeData(medAbstractData *data) override;

    medImageView::Orientation orientation();

    virtual QPointF mapWorldToDisplayCoordinates( const QVector3D & worldVec ) = 0;
    virtual QVector3D mapDisplayToWorldCoordinates( const QPointF & scenePoint ) = 0;
    virtual QVector3D viewCenter() = 0;
    virtual QVector3D viewPlaneNormal() = 0;
    virtual QVector3D viewUp() = 0;
    virtual bool is2D() = 0;
    virtual qreal sliceThickness() = 0;
    virtual qreal scale() = 0;

    QWidget* toolBarWidget() override;

    medCompositeParameterL *cameraParameter();
    medAbstractVector3DParameterL *positionBeingViewedParameter();
    medCompositeParameterL *windowLevelParameter(unsigned int layer);
    medDoubleParameterL *opacityParameter(unsigned int layer);
    medTriggerParameterL *fourViewsParameter();
    medTimeLineParameterL *timeLineParameter();

public slots:
    void switchToFourViews();
    void setOrientation(medImageView::Orientation orientation);


signals:
    void orientationChanged();
    void currentTimeChanged(const double &time);


protected:
    medAbstractImageViewInteractor* primaryInteractor(medAbstractData* data) override;
    QList<medAbstractInteractor*> extraInteractors(medAbstractData* data) override;
    medAbstractImageViewInteractor* primaryInteractor(unsigned int layer) override;
    QList<medAbstractInteractor*> extraInteractors(unsigned int layer) override;

    medAbstractImageViewNavigator* primaryNavigator() override;
    QList<medAbstractNavigator*> extraNavigators() override;

    bool initialiseInteractors(medAbstractData* data) override;
    bool initialiseNavigators() override;
    void removeInteractors(medAbstractData *data) override;

private:
    medAbstractImageViewPrivate *d;
};


