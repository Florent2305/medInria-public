/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <typeinfo>
#include <vector>

#include <medAbstractData.h>

#include <medCoreLegacyExport.h>

class MEDCORELEGACY_EXPORT medAbstractImageData: public medAbstractData
{
    Q_OBJECT

public:
    using PixId = std::type_info;
    using MatrixType = std::vector < std::vector <double> >;

    medAbstractImageData();
    medAbstractImageData(const medAbstractImageData& other);
    ~medAbstractImageData() override;

    void *image();

    virtual int   Dimension() const;
    virtual const PixId& PixelType() const;
    virtual MatrixType orientationMatrix();

    virtual int xDimension();
    virtual int yDimension();
    virtual int zDimension();
    virtual int tDimension();

    virtual int minRangeValue();
    virtual int maxRangeValue();

    virtual int scalarValueCount(int value);
    virtual int scalarValueMinCount();
    virtual int scalarValueMaxCount();

    static const char* PixelMeaningMetaData;
};

Q_DECLARE_METATYPE(medAbstractImageData)
Q_DECLARE_METATYPE(medAbstractImageData*)
