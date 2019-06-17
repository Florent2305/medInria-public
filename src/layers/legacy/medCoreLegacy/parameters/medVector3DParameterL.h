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

class medVector3DParameterLPrivate;
class QVector3D;

class MEDCORELEGACY_EXPORT medVector3DParameterL : public medAbstractVector3DParameterL
{
public:
    medVector3DParameterL(QString name = "Unknow Vector 3D parameter", QObject* parent = nullptr);
    ~medVector3DParameterL() override;

    QWidget* getWidget() override;

protected:
    void updateInternWigets() override;

private:
    medVector3DParameterLPrivate *d;

};
