#include "lwLocalZonal.h"
#include "itkProgressReporter.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include <queue>
#include <math.h>
#include <map>

template <class TI, class TO>
void lwLocalZonal<TI, TO>::GenerateInputRequestedRegion() {
  Superclass::GenerateInputRequestedRegion();

  InputImagePointerType  inputPtr = const_cast<TI *>(this->GetInput(0));
  OutputImagePointerType outputPtr = this->GetOutput();
  if (!inputPtr || !outputPtr)
    return;

  // input region = output + a pad area
  InputImageRegionType inputRequestedRegion = inputPtr->GetRequestedRegion();
  InputImageSizeType maxRad;
  maxRad[0] = 2*m_radius+1;
  maxRad[1] = 2*m_radius+1;
  inputRequestedRegion.PadByRadius(maxRad);

  // crop the input requested region at the input's largest possible region
  if (inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion())) {
    inputPtr->SetRequestedRegion(inputRequestedRegion);
  } else {
    // Couldn't crop the region (requested region is outside the largest possible region).
    inputPtr->SetRequestedRegion(inputRequestedRegion);
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    std::ostringstream msg;
    msg << this->GetNameOfClass()
        << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str().c_str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
  }
}

template <class TI,  class TO>
void lwLocalZonal<TI, TO>::GenerateOutputInformation()
{
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();

  this->GetOutput()->SetNumberOfComponentsPerPixel( 5 );
  
}

template<class TI, class TO>
void lwLocalZonal<TI, TO>::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) 
{

 

  InputImagePointerType inputPtr = const_cast<TI *>(this->GetInput(0));
  typename itk::ConstNeighborhoodIterator<TI>::RadiusType radius;
  radius[0] = m_radius; radius[1] = m_radius;
  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);
  

  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  itk::Offset<2> offset;
  typename TI::InternalPixelType pixelC; // class value
  typename TI::InternalPixelType pixelZCenter; 
  typename TI::InternalPixelType pixelZ; //zone value
  typename TO::InternalPixelType pixelO;
  typename std::map<typename TO::InternalPixelType, unsigned int> count;
  typename std::map<typename TO::InternalPixelType, unsigned int>::const_iterator cIt;

  const typename TI::InternalPixelType nan = 0; //NAN
  
  itk::ConstNeighborhoodIterator<TI> classIt(radius, inputPtr, inputRegionForThread);
  itk::ConstNeighborhoodIterator<TI> zIt(radius, this->GetInput(1), inputRegionForThread);
  itk::ImageRegionIterator<TO> oIt(this->GetOutput(), outputRegionForThread);
  
  // Loop on the pixels
  for (classIt.GoToBegin(), oIt.GoToBegin(), zIt.GoToBegin(); !classIt.IsAtEnd(); ++oIt, ++classIt, ++zIt) 
  {

		offset.Fill(0);
		pixelZCenter = zIt.GetPixel(offset);
	
	typename TO::PixelType pixelO;
   		pixelO.SetSize(5);
      int totCount=0;
      for(offset[0] = -m_radius; offset[0] <= m_radius; offset[0]++) 
	  {
        for(offset[1] = -m_radius; offset[1] <= m_radius; offset[1]++) 
		{
          pixelZ = zIt.GetPixel(offset);
		  if (pixelZ==pixelZCenter)
		  {
		    pixelC = classIt.GetPixel(offset);
			if( count.find( pixelC ) == count.end() ) 	count[pixelC] = 0;
			count[pixelC]++;
			totCount++;
		  }
        }
      }
      count.erase(nan);
      unsigned int max = 0;
	  unsigned int diversity = 0;
	  unsigned int valid = 0;
	  unsigned int secBest=0;
	  unsigned int classMax(0), classSec(0);
	  
      for(cIt = count.begin(); cIt != count.end(); ++cIt) 
	  {
	    diversity++;
		valid+=cIt->second;
        if(cIt->second > max) 
		{
			max = cIt->second;
			classMax = cIt->first;
			secBest = max;
        }
		else if (cIt->second >=secBest)
		{
			secBest=cIt->second;
			classSec = cIt->first;
		}
      }
		//double EDI = log(max/totCount)-SumEntropyPond/(1-(max/likelyTotPond));
		//double ERP = 100*exp(EDI)/(exp(EDI)+m_numClasses-1);
		pixelO[0] = static_cast<OutputInternalPixelType>(classMax);
		pixelO[1] = static_cast<OutputInternalPixelType>(max);
		pixelO[2] = static_cast<OutputInternalPixelType>(secBest);
		pixelO[3] = static_cast<OutputInternalPixelType>(diversity);
		pixelO[4] = static_cast<OutputInternalPixelType>(valid);
		
		count.clear();
    
	

    // store output
    oIt.Set( pixelO );
  }

}

