/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/


#include <medAbstractParameterGroup.h>

#include <QtCore>
#include <QColor>
#include <medAbstractParameter.h>

class medAbstractParameterGroupPrivate
{
public:
    QString name;
    QStringList paramsLinked;
    QStringList paramsNotLinked;
    bool linkAll;
    QColor color;
    QString workspace;
};

medAbstractParameterGroup::medAbstractParameterGroup(QString name, QObject *parent, QString workspace) : d(new medAbstractParameterGroupPrivate)
{
    setParent(parent);
    d->name = name;
    d->linkAll = false;
    d->color = QColor::fromHsv(qrand()%360, 255, 210);
    d->workspace = workspace;
}

medAbstractParameterGroup::~medAbstractParameterGroup()
{
    delete d;
}

void medAbstractParameterGroup::setName(QString name)
{
    d->name = name;
}

QString medAbstractParameterGroup::name() const
{
    return d->name;
}

void medAbstractParameterGroup::addParameterToLink(QString parameter)
{
    if(!d->paramsLinked.contains(parameter))
      d->paramsLinked.append(parameter);

    updatePool();
}

void medAbstractParameterGroup::removeParameter(QString parameter)
{
    d->paramsLinked.removeAll(parameter);

    if(!d->paramsNotLinked.contains(parameter))
      d->paramsNotLinked.append(parameter);

    setLinkAllParameters(false);

    updatePool();
}

void medAbstractParameterGroup::setParametersToLink(QStringList parameters)
{
    d->paramsLinked = parameters;
}

void medAbstractParameterGroup::setLinkAllParameters(bool linkAll)
{
    d->linkAll = linkAll;
}

bool medAbstractParameterGroup::linkAll() const
{
    return d->linkAll;
}

QStringList medAbstractParameterGroup::parametersToLink() const
{
    return d->paramsLinked;
}

QStringList medAbstractParameterGroup::parametersNotToLink() const
{
    return d->paramsNotLinked;
}

void medAbstractParameterGroup::setColor(QColor color)
{
    QColor oldColor = d->color;
    d->color = color;
    emit groupColorChanged(oldColor, color);
}

QColor medAbstractParameterGroup::color() const
{
    return d->color;
}

void medAbstractParameterGroup::setWorkspace(QString workspace)
{
    d->workspace = workspace;
}

QString medAbstractParameterGroup::workspace() const
{
    return d->workspace;
}


