/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <dtkCoreSupport/dtkAbstractProcess.h>
#include <medAbstractData.h>

#include <medCoreLegacyExport.h>

class medAbstractProcessLegacyPrivate;


/**
 * Extending dtkAbstractProcess class to hold more specific information
 */
class MEDCORELEGACY_EXPORT medAbstractProcessLegacy : public dtkAbstractProcess
{
    Q_OBJECT

public:
    medAbstractProcessLegacy( medAbstractProcessLegacy * parent = nullptr );
    ~medAbstractProcessLegacy() override;

public slots:
    medAbstractData *output() override = 0;
    int update () override = 0;

private:
    using dtkAbstractProcess::onCanceled;
    using dtkAbstractProcess::read;
    using dtkAbstractProcess::write;
    using dtkAbstractProcess::setParameter;
    using dtkAbstractProcess::setInput;
    using dtkAbstractProcess::setData;
    //TODO rename our output methode
    //using dtkAbstractProcess::output;
    using dtkAbstractProcess::data;
    using dtkAbstractProcess::channelCount;

private:
    medAbstractProcessLegacyPrivate* d;

};


