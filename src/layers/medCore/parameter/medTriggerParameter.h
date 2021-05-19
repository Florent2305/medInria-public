#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2020. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medAbstractParameter.h>
#include <medCoreExport.h>

class QIcon;
class QPushButton;
class QSize;
class QWidget;

class MEDCORE_EXPORT medTriggerParameter : public medAbstractParameter
{
    Q_OBJECT

public:
    medTriggerParameter(QString name = "Unknown trigger parameter", QObject* parent = nullptr);
    ~medTriggerParameter() override;

    medParameterType type() const override { return medParameterType::MED_PARAMETER_TRIGGER; }

    void trigger() override;
    bool copyValueTo(medAbstractParameter &dest) override { return false; };

signals:
    void pushed(bool);
    void triggered();
};
