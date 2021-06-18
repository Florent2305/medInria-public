/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2020. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medAbstractParameter.h>
#include <medAbstractProcess.h>

#include <QMap>

class medAbstractProcessPrivate
{
public:
    QMap<QString, medAbstractParameter*> parameters;
};

medAbstractProcessDTK2::medAbstractProcessDTK2(QObject *parent)
    : medAbstractJob(parent), d(new medAbstractProcessPrivate)
{

}

medAbstractProcessDTK2::~medAbstractProcessDTK2()
{
}

void medAbstractProcessDTK2::registerParameter(medAbstractParameter * parameter)
{
    parameter->setParent(this);
    d->parameters.insert(parameter->id(), parameter);
}

medAbstractParameter* medAbstractProcessDTK2::parameter(QString const& id) const
{
    return d->parameters[id];
}

QList<medAbstractParameter*> medAbstractProcessDTK2::parameters() const
{
    return d->parameters.values();
}
