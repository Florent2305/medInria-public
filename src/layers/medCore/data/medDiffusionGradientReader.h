/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medCoreExport.h>
#include <QString>
#include <QScopedPointer>
#include <vector>

class medDiffusionGradientReaderPrivate;
class MEDCORE_EXPORT medDiffusionGradientReader
{
public:
    medDiffusionGradientReader();
    virtual ~medDiffusionGradientReader();

    using VectorType = std::vector <double>;
    using GradientsVectorType = std::vector <VectorType>;

    void read(QString gradientFileName, QString bvaluesFileName);
    void readGradients(QString gradientFileName);
    void readBValues(QString bvaluesFileName);

    GradientsVectorType &gradients();
    VectorType &bvalues();

protected:
    void readBVecFile(QString gradientFileName);

private:
    const QScopedPointer<medDiffusionGradientReaderPrivate> d;
};
