#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2020. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medAbstractJob.h>
#include <medCoreExport.h>

#include <dtkCore>

class medAbstractParameter;
class medAbstractProcessPrivate;

class MEDCORE_EXPORT medAbstractProcessDTK2: public medAbstractJob
{
    Q_OBJECT
public:
    medAbstractProcessDTK2(QObject *parent = nullptr);
    virtual ~medAbstractProcessDTK2();

    virtual QString description() const = 0;

    medAbstractParameter* parameter(QString const& id) const;
    QList<medAbstractParameter*> parameters() const;

protected:
    void registerParameter(medAbstractParameter *parameter);

    //! Utility function to add a process description to outputs
    virtual QString outputNameAddon() const {return "processed";}

private:
    const QScopedPointer<medAbstractProcessPrivate> d;
};

// Why is this here?
// Because unfortunately, the current implementation prevents us from having processes *in workspaces* that are not attached to a abstraction
// Because we can't predict all types of abstractions, we need to provide a generic abstraction that can be used by plugin designers
DTK_DECLARE_PLUGIN        (medAbstractProcessDTK2, MEDCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(medAbstractProcessDTK2, MEDCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(medAbstractProcessDTK2, MEDCORE_EXPORT)
