/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medAbstractParameterL.h>

#include <medCoreLegacyExport.h>

class QWidget;
class QSpinBox;
class QSlider;


class medIntParameterLPrivate;
class MEDCORELEGACY_EXPORT medIntParameterL : public medAbstractIntParameterL
{
    Q_OBJECT

public:
    medIntParameterL(QString name = "Unknow int parameter", QObject* parent = nullptr);
    ~medIntParameterL() override;

    void setRange(int min, int max);
    QSpinBox* getSpinBox();
    QSlider* getSlider();

    QWidget* getWidget() override;

public slots:
    void setValue(int value) override;

protected:
    void updateInternWigets() override;

private slots:
    void removeInternSpinBox();
    void removeInternSlider();

private:
    medIntParameterLPrivate* d;
};
