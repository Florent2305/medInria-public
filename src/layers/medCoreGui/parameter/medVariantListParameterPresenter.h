#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2020. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medAbstractParameterPresenter.h>
#include <medVariantListParameter.h>

class QWidget;
//class QCheckBox;
//class QRadioButton;
//class QPushButton;
//class QAbstractButton;
class medCompositeParameterPresenterPrivate;


class MEDWIDGETS_EXPORT medCompositeParameterPresenter : public medAbstractParameterPresenter
{
    Q_OBJECT

public:
    medCompositeParameterPresenter(medVariantListParameter *parent);
    medCompositeParameterPresenter(const QString & newParameterId);
    virtual ~medCompositeParameterPresenter();

    virtual medVariantListParameter *parameter() const;

    QWidget * buildWidget() override;

private:
    const QScopedPointer<medCompositeParameterPresenterPrivate> d;
};
