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
class QDoubleSpinBox;
class QSlider;


class medDoubleParameterLPrivate;
class MEDCORELEGACY_EXPORT medDoubleParameterL : public medAbstractDoubleParameterL
{
    Q_OBJECT

public:
    medDoubleParameterL(QString name = "Unknow double parameter", QObject* parent = nullptr);
    ~medDoubleParameterL() override;

    void setRange(double min, double max);
    void setSingleStep(double step);
    void setDecimals(unsigned int decimals);

    QDoubleSpinBox* getSpinBox();
    QSlider* getSlider();
    QLabel* getValueLabel();

    QWidget* getWidget() override;

public slots:
    void setValue(double value) override;

protected:
    void updateInternWigets() override;

private slots:
    void removeInternSpinBox();
    void removeInternSlider();

    void setIntValue(int value);

private:
    int convertToInt(double value);

private:
    medDoubleParameterLPrivate* d;
};
