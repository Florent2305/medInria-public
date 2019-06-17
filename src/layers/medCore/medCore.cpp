/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medCore.h>

namespace medCore
{

DTK_DEFINE_LAYER_MANAGER;

namespace pluginManager
{
    void initialize(const QString& path)
    {
        for(QString const& realpath : path.split(';'))
        {
            if (realpath.isEmpty())
            {
                break;
            }

            medCore::arithmeticOperation::pluginManager().initialize(realpath);
            medCore::dwiMasking::pluginManager().initialize(realpath);
            medCore::diffusionModelEstimation::pluginManager().initialize(realpath);
            medCore::diffusionScalarMaps::pluginManager().initialize(realpath);
            medCore::tractography::pluginManager().initialize(realpath);
            medCore::morphomathOperation::pluginManager().initialize(realpath);
            medCore::maskImage::pluginManager().initialize(realpath);
            medCore::singleFilterOperation::pluginManager().initialize(realpath);
            medCore::dataConverter::pluginManager().initialize(realpath);
        }
    }
}

DTK_DEFINE_CONCEPT(medAbstractProcess,generic,medCore)
DTK_DEFINE_CONCEPT(medAbstractDWIMaskingProcess,dwiMasking,medCore)
DTK_DEFINE_CONCEPT(medAbstractDiffusionModelEstimationProcess,diffusionModelEstimation,medCore)
DTK_DEFINE_CONCEPT(medAbstractDiffusionScalarMapsProcess,diffusionScalarMaps,medCore)
DTK_DEFINE_CONCEPT(medAbstractTractographyProcess,tractography,medCore)
DTK_DEFINE_CONCEPT(medAbstractMaskImageProcess,maskImage,medCore)
DTK_DEFINE_CONCEPT(medAbstractDataConverter,dataConverter,medCore)

// arithimetic
namespace arithmeticOperation
{
    namespace _private
    {
        medAbstractArithmeticOperationProcessPluginManager manager;
        medAbstractArithmeticOperationProcessPluginFactory factory;
    }

    medAbstractArithmeticOperationProcessPluginManager& pluginManager( )
    {
        return _private::manager;
    }

    medAbstractArithmeticOperationProcessPluginFactory& pluginFactory( )
    {
        return _private::factory;
    }

    namespace addImage
    {
        namespace _private
        {
            medAbstractAddImageProcessPluginFactory factory;
        }

        medAbstractAddImageProcessPluginFactory& pluginFactory( )
        {
            return _private::factory;
        }
    }
    namespace subtractImage
    {
        namespace _private
        {
            medAbstractSubtractImageProcessPluginFactory factory;
        }
        medAbstractSubtractImageProcessPluginFactory& pluginFactory( )
        {
            return _private::factory;
        }
    }
    namespace multiplyImage
    {
        namespace _private
        {
            medAbstractMultiplyImageProcessPluginFactory factory;
        }
        medAbstractMultiplyImageProcessPluginFactory& pluginFactory( )
        {
            return _private::factory;
        }
    }
    namespace divideImage
    {
        namespace _private
        {
            medAbstractDivideImageProcessPluginFactory factory;
        }

        medAbstractDivideImageProcessPluginFactory& pluginFactory( )
        {
            return _private::factory;
        }
    }
}

// single filter
namespace singleFilterOperation
{
    namespace _private
    {
        medAbstractSingleFilterOperationProcessPluginManager manager;
    }

    medAbstractSingleFilterOperationProcessPluginManager& pluginManager( )
    {
        return _private::manager;
    }

    namespace addFilter
    {
        namespace _private
        {
            medAbstractAddFilterProcessPluginFactory factory;
        }

        medAbstractAddFilterProcessPluginFactory& pluginFactory( )
        {
            return _private::factory;
        }
    }

    namespace divideFilter
    {
        namespace _private
        {
            medAbstractDivideFilterProcessPluginFactory factory;
        }

        medAbstractDivideFilterProcessPluginFactory& pluginFactory( )
        {
            return _private::factory;
        }
    }

    namespace gaussianFilter
    {
        namespace _private
        {
            medAbstractGaussianFilterProcessPluginFactory factory;
        }

        medAbstractGaussianFilterProcessPluginFactory& pluginFactory( )
        {
            return _private::factory;
        }
    }

    namespace invertFilter
    {
        namespace _private
        {
            medAbstractInvertFilterProcessPluginFactory factory;
        }

        medAbstractInvertFilterProcessPluginFactory& pluginFactory( )
        {
            return _private::factory;
        }
    }

    namespace medianFilter
    {
        namespace _private
        {
            medAbstractMedianFilterProcessPluginFactory factory;
        }

        medAbstractMedianFilterProcessPluginFactory& pluginFactory( )
        {
            return _private::factory;
        }
    }

    namespace multiplyFilter
    {
        namespace _private
        {
            medAbstractMultiplyFilterProcessPluginFactory factory;
        }

        medAbstractMultiplyFilterProcessPluginFactory& pluginFactory( )
        {
            return _private::factory;
        }
    }

    namespace normalizeFilter
    {
        namespace _private
        {
            medAbstractNormalizeFilterProcessPluginFactory factory;
        }

        medAbstractNormalizeFilterProcessPluginFactory& pluginFactory( )
        {
            return _private::factory;
        }
    }

    namespace shrinkFilter
    {
        namespace _private
        {
            medAbstractShrinkFilterProcessPluginFactory factory;
        }

        medAbstractShrinkFilterProcessPluginFactory& pluginFactory( )
        {
            return _private::factory;
        }
    }

    namespace subtractFilter
    {
        namespace _private
        {
            medAbstractSubtractFilterProcessPluginFactory factory;
        }

        medAbstractSubtractFilterProcessPluginFactory& pluginFactory( )
        {
            return _private::factory;
        }
    }

    namespace windowingFilter
    {
        namespace _private
        {
            medAbstractWindowingFilterProcessPluginFactory factory;
        }

        medAbstractWindowingFilterProcessPluginFactory& pluginFactory( )
        {
            return _private::factory;
        }
    }

    namespace imageDenoising
    {
        namespace _private
        {
            medAbstractImageDenoisingProcessPluginFactory factory;
        }

        medAbstractImageDenoisingProcessPluginFactory& pluginFactory( )
        {
            return _private::factory;
        }
    }

    namespace relaxometryEstimation
    {
        namespace _private
        {
            medAbstractRelaxometryEstimationProcessPluginFactory factory;
        }

        medAbstractRelaxometryEstimationProcessPluginFactory& pluginFactory( )
        {
            return _private::factory;
        }
    }

    namespace symmetryAlignment
    {
        namespace _private
        {
            medAbstractSymmetryPlaneAlignmentProcessPluginFactory factory;
        }

        medAbstractSymmetryPlaneAlignmentProcessPluginFactory& pluginFactory( )
        {
            return _private::factory;
        }
    }

    namespace biasCorrection
    {
       namespace _private
       {
          medAbstractBiasCorrectionProcessPluginFactory factory;
       }

       medAbstractBiasCorrectionProcessPluginFactory& pluginFactory( )
       {
          return _private::factory;
       }
    }
}

// morphomath
namespace morphomathOperation
{

    namespace _private
    {
        medAbstractMorphomathOperationProcessPluginManager manager;
        medAbstractMorphomathOperationProcessPluginFactory factory;
    }

    medAbstractMorphomathOperationProcessPluginFactory& pluginFactory( )
    {
        return _private::factory;
    }

    medAbstractMorphomathOperationProcessPluginManager& pluginManager( )
    {
        return _private::manager;
    }

    namespace erodeImage
    {
        namespace _private
        {
            medAbstractErodeImageProcessPluginFactory factory;
        }
        medAbstractErodeImageProcessPluginFactory& pluginFactory( )
        {
            return _private::factory;
        }
    }

    namespace dilateImage
    {
        namespace _private
        {
            medAbstractDilateImageProcessPluginFactory factory;
        }

        medAbstractDilateImageProcessPluginFactory& pluginFactory( )
        {
            return _private::factory;
        }
    }

    namespace openingImage
    {
        namespace _private
        {
            medAbstractOpeningImageProcessPluginFactory factory;
        }

        medAbstractOpeningImageProcessPluginFactory& pluginFactory( )
        {
            return _private::factory;
        }
    }

    namespace closingImage
    {
        namespace _private
        {
            medAbstractClosingImageProcessPluginFactory factory;
        }
        medAbstractClosingImageProcessPluginFactory& pluginFactory( )
        {
            return _private::factory;
        }
    }
}

} // end of medCore

