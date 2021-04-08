/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2020. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medVector3DParameter.h"
#include <QDomDocument>
#include <QDomElement>
#include <QVector3D>

class medVector3DParameterPrivate
{
public:
    QVector3D value;
};

medVector3DParameter::medVector3DParameter(QString name, QObject *parent)
    : medAbstractParameter(name, parent)
{
}

medVector3DParameter::~medVector3DParameter()
{
}


void medVector3DParameter::setValue(const QVector3D &value)
{
    if (d->value != value)
    {
        d->value = value;
        emit valueChanged(value);
    }
}

QVector3D medVector3DParameter::value() const
{
    return d->value;
}

void medVector3DParameter::trigger()
{
    emit valueChanged(d->value);
}

QVector3D medVector3DParameter::fromString(QString value)
{
    QStringList tokens = value.split(" ");
    return QVector3D(tokens[0].toDouble(), tokens[1].toDouble(), tokens[2].toDouble());
}

void medVector3DParameter::toXMLNode(QDomDocument *doc, QDomElement *currentNode)
{
    QDomElement element = doc->createElement("name");
    element.appendChild(doc->createTextNode(id()));
    currentNode->appendChild(element);
    QDomElement type = doc->createElement("type");
    type.appendChild(doc->createTextNode("Vector3D"));
    currentNode->appendChild(type);
    element = doc->createElement("value");
    element.appendChild(doc->createTextNode(QString::number(value().x())
        + " "
        + QString::number(value().y())
        + " "
        + QString::number(value().z())));
    currentNode->appendChild(element);
}
