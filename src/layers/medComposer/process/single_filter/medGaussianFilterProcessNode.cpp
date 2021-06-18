/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medGaussianFilterProcessNode.h>

#include <medCore.h>
#include <medWidgets.h>

medGaussianFilterProcessNode::medGaussianFilterProcessNode()
{
    this->setFactory(medCore::singleFilterOperation::gaussianFilter::pluginFactory());
}

QWidget* medGaussianFilterProcessNode::editor()
{
    medAbstractProcessDTK2* process = this->object();
    if (!process)
    {
        return nullptr;
    }
    medAbstractProcessDTK2Presenter* presenter = medWidgets::singleFilterOperation::gaussianFilter::presenterFactory().create(process);
    return presenter->buildToolBoxWidget();
}
