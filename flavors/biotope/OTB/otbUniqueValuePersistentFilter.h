/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbUniqueValuePersistentFilter_h
#define __otbUniqueValuePersistentFilter_h

#include "otbPersistentImageFilter.h"
#include <vnl/vnl_matrix.h>
#include <map>
#include <itkImageRegionConstIteratorWithIndex.h>
#include <itkProgressReporter.h>

namespace otb
{

template<class TInputImage > 
  class ITK_EXPORT UniqueValuePersistentFilter : 
	  public otb::PersistentImageFilter<TInputImage, TInputImage>
{




public:

      /** Standard typedefs */
  typedef UniqueValuePersistentFilter								Self;
  typedef otb::PersistentImageFilter<TInputImage, TInputImage>	Superclass;
  typedef itk::SmartPointer<Self>								Pointer;
  typedef itk::SmartPointer<const Self>							ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(TabulatePersistentFilter, otb::PersistentImageFilter);

  /** Template parameters typedefs */
  typedef TInputImage							ImageType;
  typedef typename ImageType::ConstPointer		ConstPointerType;
  typedef typename ImageType::RegionType		RegionType;
  typedef typename ImageType::PixelType			PixelType;



  std::set<PixelType >GetMap()
  {
      return m_Set;
  } 

   virtual void Reset() 
  { 
  // Retrieve the number of threads 
   int numberOfThreads = this->GetNumberOfThreads(); 
    // Reset the final result 
  m_Set.clear() ; 
  // Reset the temporary results container 
  m_TemporarySets = std::vector<std::set<PixelType> >(  numberOfThreads );
  }

    virtual void Synthetize() 
  { 
  // For each thread 
  for( int threadId = 0; threadId < this->GetNumberOfThreads();++threadId) 
    { 
		m_Set.insert(  m_TemporarySets[threadId].begin(),m_TemporarySets[threadId].end()  );

    } 
    std::cout << "elements in set " << m_Set.size() << std::endl;
  }
  
private: 
  // Temporary results container 
  std::vector< std::set<PixelType> > m_TemporarySets; 
 
  // Final result member 
  std::set< PixelType> m_Set;

protected: 

  virtual void ThreadedGenerateData(const RegionType& outputRegionForThread, // int threadId) 
	  itk::ThreadIdType threadId)
  { 
  //// Enable progress reporting 
  //itk::ProgressReporter(this,threadId,outputRegionForThread.GetNumberOfPixels()); 
  
  // Retrieve the input pointer 
 typename ImageType::Pointer inputPtr = const_cast<ImageType *>(this->GetInput()); 
  itk::ImageRegionConstIteratorWithIndex<ImageType> it(inputPtr, outputRegionForThread); 

  // Walk the region of the image with the iterator   
  for (it.GoToBegin(); !it.IsAtEnd(); ++it) 
    { 
    // Retrieve pixel value 
    const PixelType value = it.Get(); 
    m_TemporarySets[threadId].insert(value);
    }

  }
};

}
#endif
