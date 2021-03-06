#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2020. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medSingleFilterDoubleOperationProcessNode.h>
#include <medAbstractMultiplyFilterProcess.h>

#include <medComposerExport.h>

class medMultiplyFilterProcessNodePrivate;

class MEDCOMPOSER_EXPORT medMultiplyFilterProcessNode : public medSingleFilterDoubleOperationProcessNode<medAbstractMultiplyFilterProcess>
{
public:
     medMultiplyFilterProcessNode();

     virtual QWidget *editor(void);
};
