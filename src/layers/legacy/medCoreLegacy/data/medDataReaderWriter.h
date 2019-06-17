/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <dtkCoreSupport/dtkSmartPointer.h>
#include <dtkCoreSupport/dtkAbstractDataReader.h>
#include <dtkCoreSupport/dtkAbstractDataWriter.h>
#include <medAbstractData.h>

#include <medCoreLegacyExport.h>

struct MEDCORELEGACY_EXPORT medDataReaderWriter {

    using Reader = dtkSmartPointer<dtkAbstractDataReader>;
    using Writer = dtkSmartPointer<dtkAbstractDataWriter>;

    static Reader reader(const QString& path);
    static Writer writer(const QString& path,const medAbstractData* data);

    static medAbstractData *read(const QString& path);
    static bool write(const QString& path,medAbstractData* data);
    };


