/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medAbstractParameter.h>

class medIntParameterPrivate;
class MEDCORE_EXPORT medIntParameter : public medAbstractParameter
{
    Q_OBJECT

public:
    medIntParameter(const QString & name, QObject *parent = nullptr);
    ~medIntParameter() override;

    medParameterType type() const override {return medParameterType::MED_PARAMETER_INT;}

    int value() const;

    void setRange(int min, int max);
    int minimum() const;
    int maximum() const;

public slots:
    void setValue(int value);

    void trigger() override;

signals:
    void valueChanged(int value);
    void rangeChanged(int min, int max);

private:
    const QScopedPointer<medIntParameterPrivate> d;
};
