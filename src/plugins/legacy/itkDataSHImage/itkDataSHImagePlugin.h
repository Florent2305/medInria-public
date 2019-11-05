#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2020. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medPluginLegacy.h>
#include <itkDataSHImagePluginExport.h>

class ITKDATASHIMAGEPLUGIN_EXPORT itkDataSHImagePluginPrivate;

class ITKDATASHIMAGEPLUGIN_EXPORT itkDataSHImagePlugin : public medPluginLegacy
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "fr.inria.itkDataSHImagePlugin" FILE "itkDataSHImagePlugin.json")
    Q_INTERFACES(dtkPlugin)

public:
     itkDataSHImagePlugin(QObject *parent = nullptr);
    ~itkDataSHImagePlugin();

    virtual bool initialize();
    virtual bool uninitialize();

    virtual QString name() const;
    virtual QString description() const;

    virtual QStringList tags() const;
    virtual QStringList types() const;

    virtual QString version() const;
    virtual QString contact(void) const;
    virtual QStringList authors() const;
    virtual QStringList contributors() const;

private:
     itkDataSHImagePluginPrivate *d;
};
