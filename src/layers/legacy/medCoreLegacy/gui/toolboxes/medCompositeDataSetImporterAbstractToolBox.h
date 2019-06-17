/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medToolBox.h>

#include <medCoreLegacyExport.h>

class medCompositeDataSetImporterSelectorToolBox;
class medCompositeDataSetImporterAbstractToolBoxPrivate;
class dtkAbstractProcess;
class medAbstractData;

class MEDCORELEGACY_EXPORT medCompositeDataSetImporterAbstractToolBox : public medToolBox
{
    Q_OBJECT
public:
     medCompositeDataSetImporterAbstractToolBox(QWidget *parent = nullptr);
    ~medCompositeDataSetImporterAbstractToolBox() override;
    virtual void setCompositeDataSetImporterToolBox(medCompositeDataSetImporterSelectorToolBox *toolbox);

    QString description() const  override = 0;

    virtual medAbstractData *output() const;

public slots:
    virtual bool import() = 0;
    virtual void reset()  {}
    virtual void load() {}

protected:
    medCompositeDataSetImporterSelectorToolBox *parent();

protected:
    medCompositeDataSetImporterAbstractToolBoxPrivate *d;
};


