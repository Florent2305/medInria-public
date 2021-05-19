/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2020. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medVariantListParameter.h>
#include <medVariantListParameterPresenter.h>
#include <medVariantListParameterWidgetPrivate.h>

#include <QWidget>
#include <QCheckBox>
#include <QSpinBox>
#include <QFormLayout>
#include <QDebug>


class medCompositeParameterPresenterPrivate
{
public:
    medVariantListParameter* parameter;
};

medCompositeParameterPresenter::medCompositeParameterPresenter(medVariantListParameter* parameter)
    :medAbstractParameterPresenter(parameter), d(new medCompositeParameterPresenterPrivate)
{
    d->parameter = parameter;
}

medCompositeParameterPresenter::medCompositeParameterPresenter(QString const& newParameterId)
    : medCompositeParameterPresenter(new medVariantListParameter(newParameterId))
{
}

medCompositeParameterPresenter::~medCompositeParameterPresenter()
{
}

medVariantListParameter* medCompositeParameterPresenter::parameter() const
{
    return d->parameter;
}


QWidget* medCompositeParameterPresenter::buildWidget()
{
    medVariantListParameterWidgetPrivate *poWidgetRes = new medVariantListParameterWidgetPrivate();

    connect(d->parameter, &medVariantListParameter::valueChanged,   poWidgetRes, &medVariantListParameterWidgetPrivate::changeValue);
    connect(d->parameter, &medVariantListParameter::variantAdded,   poWidgetRes, &medVariantListParameterWidgetPrivate::addVariant);
    connect(d->parameter, &medVariantListParameter::variantRemoved, poWidgetRes, &medVariantListParameterWidgetPrivate::removeVariant);
    
    connect(poWidgetRes, &medVariantListParameterWidgetPrivate::valueChanged, d->parameter, &medVariantListParameter::setValue);

    return poWidgetRes; 
}
