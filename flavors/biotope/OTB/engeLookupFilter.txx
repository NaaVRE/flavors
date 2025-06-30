#include "itkImageRegionIterator.h"
#include "engeLookupFilter.h"

template <class TI, class TO>
void engeLookupFilter<TI, TO>
::ThreadedGenerateData(const typename TO::RegionType& outputRegionForThread, itk::ThreadIdType threadId)
{

  // initialize iterators
  typename TI::RegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);
  itk::ImageRegionIterator     <TO> oIt(this->GetOutput(), outputRegionForThread);
  itk::ImageRegionConstIterator<TI> iIt(this->GetInput(), inputRegionForThread);

  typename TO::PixelType pixelO;
  typename TI::PixelType pixelI;

  typename std::map<typename TI::InternalPixelType, typename TO::InternalPixelType>::const_iterator it;

  // loop over pixels
  for(oIt.GoToBegin(), iIt.GoToBegin(); ! oIt.IsAtEnd(); ++oIt, ++iIt)
  {
    pixelI = iIt.Get();
    pixelO = m_background;

    it = m_rules.find(pixelI);
    if(it != m_rules.end())
      pixelO = it->second;

    oIt.Set( pixelO );
  }

}

