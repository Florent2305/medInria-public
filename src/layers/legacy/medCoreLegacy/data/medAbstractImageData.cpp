/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medAbstractImageData.h>

#include <QtGui>

// /////////////////////////////////////////////////////////////////
// medAbstractImageDataPrivate
// /////////////////////////////////////////////////////////////////

//  Unneeded in this case.

// /////////////////////////////////////////////////////////////////
// medAbstractImageData
// /////////////////////////////////////////////////////////////////

const char* medAbstractImageData::PixelMeaningMetaData = "PixelMeaning";

medAbstractImageData::medAbstractImageData()
{
    dtkDebug() << "constructing medAbstractImageData";
}

medAbstractImageData::medAbstractImageData(const medAbstractImageData& other) = default;

medAbstractImageData::~medAbstractImageData()
{
    dtkDebug() << "deleting medAbstractImageData";
}

void *medAbstractImageData::image()
{
    return this->data();
}

int medAbstractImageData::Dimension() const
{
    DTK_DEFAULT_IMPLEMENTATION;

    return 0;
}

const medAbstractImageData::PixId& medAbstractImageData::PixelType() const
{
    DTK_DEFAULT_IMPLEMENTATION;

    return typeid(void);
}

medAbstractImageData::MatrixType medAbstractImageData::orientationMatrix()
{
    DTK_DEFAULT_IMPLEMENTATION;

    return MatrixType();
}

int medAbstractImageData::xDimension()
{
    DTK_DEFAULT_IMPLEMENTATION;

    return 0;
}

int medAbstractImageData::yDimension()
{
    DTK_DEFAULT_IMPLEMENTATION;

    return 0;
}

int medAbstractImageData::zDimension()
{
    DTK_DEFAULT_IMPLEMENTATION;

    return 0;
}

int medAbstractImageData::tDimension()
{
    DTK_DEFAULT_IMPLEMENTATION;
	
    return 0;
}

int medAbstractImageData::minRangeValue()
{
    DTK_DEFAULT_IMPLEMENTATION;

    return 0;
}

int medAbstractImageData::maxRangeValue()
{
    DTK_DEFAULT_IMPLEMENTATION;

    return 0;
}

int medAbstractImageData::scalarValueCount(int value)
{
    DTK_DEFAULT_IMPLEMENTATION;
    DTK_UNUSED(value);

    return 0;
}

int medAbstractImageData::scalarValueMinCount()
{
    DTK_DEFAULT_IMPLEMENTATION;

    return 0;
}

int medAbstractImageData::scalarValueMaxCount()
{
    DTK_DEFAULT_IMPLEMENTATION;

    return 0;
}
