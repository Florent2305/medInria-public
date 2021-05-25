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
#include <medStringParameter.h>

class QWidget;
class QLineEdit;
class medStringParameterPresenterPrivate;

class MEDCOREGUI_EXPORT medStringParameterPresenter : public medAbstractParameterPresenter
{
    Q_OBJECT

public:
    medStringParameterPresenter(medStringParameter *parent);
    medStringParameterPresenter(const QString & newParameterId);
    virtual ~medStringParameterPresenter();

    virtual medStringParameter *parameter() const;

    virtual QWidget *buildWidget();
    QLineEdit *buildLineEdit();

private:
    const QScopedPointer<medStringParameterPresenterPrivate> d;
};
