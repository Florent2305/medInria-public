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
#include <medTimeLineParameter.h>

class QWidget;

class medTimeLineParameterPresenterPrivate;

class MEDWIDGETS_EXPORT medTimeLineParameterPresenter : public medAbstractParameterPresenter
{
    Q_OBJECT

public:
    medTimeLineParameterPresenter(medTimeLineParameter *parent);
    virtual ~medTimeLineParameterPresenter();

    virtual QWidget *buildWidget();


    private:

    private:
        const QScopedPointer<medTimeLineParameterPresenterPrivate> d;
};
