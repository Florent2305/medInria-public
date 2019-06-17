/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medAbstractFibersData.h>
#include <dtkLog/dtkLog.h>

// /////////////////////////////////////////////////////////////////
// medAbstractFibersDataPrivate
// /////////////////////////////////////////////////////////////////

class medAbstractFibersDataPrivate
{
public:
};

// /////////////////////////////////////////////////////////////////
// medAbstractFibersData
// /////////////////////////////////////////////////////////////////

medAbstractFibersData::medAbstractFibersData() : d(new medAbstractFibersDataPrivate)
{
}

medAbstractFibersData::medAbstractFibersData(const  medAbstractFibersData& other): medAbstractData(other), d(new medAbstractFibersDataPrivate)
{
}

medAbstractFibersData::~medAbstractFibersData()
{
    delete d;
    d = nullptr;
}

void *medAbstractFibersData::fibers()
{
    return this->data();
}
