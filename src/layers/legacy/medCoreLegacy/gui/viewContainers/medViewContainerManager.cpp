/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medViewContainerManager.h>

#include <QUuid>
#include <QHash>

#include <medViewContainer.h>


class medViewContainerManagerPrivate
{
public:
    QHash<QUuid, medViewContainer*> containers;
};

medViewContainerManager *medViewContainerManager::instance()
{
    if(!s_instance)
        s_instance = new medViewContainerManager;

    return s_instance;
}

medViewContainerManager::medViewContainerManager() : d(new medViewContainerManagerPrivate)
{
}

medViewContainerManager::~medViewContainerManager()
{
    delete d;
    d = nullptr;
}

void medViewContainerManager::registerNewContainer(medViewContainer* newContainer)
{
    d->containers.insert(newContainer->uuid(), newContainer);
}

void medViewContainerManager::unregisterContainer(medViewContainer *container)
{
    QUuid uuid = container->uuid();
    emit containerAboutToBeDestroyed(uuid);
    d->containers.remove(uuid);
}

medViewContainer* medViewContainerManager::container(QUuid uuid) const
{
    return d->containers.value(uuid);
}

medViewContainerManager *medViewContainerManager::s_instance = nullptr;
