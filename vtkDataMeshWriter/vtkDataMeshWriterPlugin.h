// /////////////////////////////////////////////////////////////////
// Generated by dtkPluginGenerator
// /////////////////////////////////////////////////////////////////

#ifndef VTKDATAMESHWRITERPLUGIN_H
#define VTKDATAMESHWRITERPLUGIN_H

#include <dtkCore/dtkPlugin.h>

#include "vtkDataMeshWriterPluginExport.h"

class VTKDATAMESHWRITERPLUGIN_EXPORT vtkDataMeshWriterPluginPrivate;

class VTKDATAMESHWRITERPLUGIN_EXPORT vtkDataMeshWriterPlugin : public dtkPlugin
{
    Q_OBJECT
    Q_INTERFACES(dtkPlugin)

public:
     vtkDataMeshWriterPlugin(QObject *parent = 0);
    ~vtkDataMeshWriterPlugin(void);

    virtual bool initialize(void);
    virtual bool uninitialize(void);

    virtual QString name(void) const;
    virtual QString description(void) const;

    virtual QStringList tags(void) const;
    virtual QStringList types(void) const;

private:
     vtkDataMeshWriterPluginPrivate *d;
};

#endif
