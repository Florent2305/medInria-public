/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medBoolParameter.h>

class medBoolParameterPrivate
{
public:
    bool value;
};

medBoolParameter::medBoolParameter(QString const& name,  QObject *parent)
    : medAbstractParameter(name, parent), d(new medBoolParameterPrivate)
{
    connect(this, &medBoolParameter::valueChanged, this, &medBoolParameter::triggered);
}

medBoolParameter::~medBoolParameter()
{

}

bool medBoolParameter::value() const
{
    return d->value;
}

bool medBoolParameter::copyValueTo(medAbstractParameter & dest)
{
    bool bRes = typeid(dest) == typeid(*this);

    if (bRes)
    {
        dynamic_cast<medBoolParameter*>(&dest)->setValue(value());
    }

    return bRes;
}

void medBoolParameter::setValue(bool value)
{
    if(value != d->value)
    {
        d->value = value;
        emit valueChanged(d->value);
    }
}

void medBoolParameter::trigger()
{
    emit valueChanged(d->value);
}
