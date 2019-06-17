/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medAbstractDataSourceFactory.h>
#include <medAbstractDataSource.h>

class medAbstractDataSourceFactoryPrivate
{
public:
    medAbstractDataSourceFactory::medAbstractDataSourceCreatorHash dataSourceCreators;
};

medAbstractDataSourceFactory *medAbstractDataSourceFactory::instance()
{
    if(!s_instance)
        s_instance = new medAbstractDataSourceFactory;

    return s_instance;
}

bool medAbstractDataSourceFactory::registerDataSource(QString type, medAbstractDataSourceCreator func)
{
    if(!d->dataSourceCreators.contains(type)) {
        d->dataSourceCreators.insert(type, func);
        return true;
    }

    return false;
}

QList<QString> medAbstractDataSourceFactory::dataSourcePlugins()
{
    return d->dataSourceCreators.keys();
}

medAbstractDataSource *medAbstractDataSourceFactory::create(QString type,QWidget * parent)
{
    if(!d->dataSourceCreators.contains(type))
        return nullptr;

    medAbstractDataSource *conf = d->dataSourceCreators[type](parent);

    return conf;
}

medAbstractDataSourceFactory::medAbstractDataSourceFactory() : d(new medAbstractDataSourceFactoryPrivate)
{
}

medAbstractDataSourceFactory::~medAbstractDataSourceFactory()
{
    delete d;
    d = nullptr;
}

medAbstractDataSourceFactory *medAbstractDataSourceFactory::s_instance = nullptr;
