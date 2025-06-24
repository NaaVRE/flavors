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
#ifndef __otbTabulatePersistentFilter_h
#define __otbTabulatePersistentFilter_h

#include "otbPersistentImageFilter.h"
#include <vnl/vnl_matrix.h>
#include <map>
#include <itkImageRegionConstIteratorWithIndex.h>
#include <itkProgressReporter.h>

namespace otb
{

template<class TInputImage > 
  class ITK_EXPORT TabulatePersistentFilter : 
	  public otb::PersistentImageFilter<TInputImage, TInputImage>
{


private: 
  // Temporary results container 
  std::vector< vnl_matrix<unsigned long> > m_TemporaryTables; 
 
  // Final result member 
  vnl_matrix< unsigned long> m_FinalTable;

  std::map<unsigned long, unsigned long> m_MapA;	
  std::map<unsigned long,unsigned  long> m_MapB;	
  unsigned long m_sizeA;
  unsigned long m_sizeB;


public:

      /** Standard typedefs */
  typedef TabulatePersistentFilter								Self;
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

  itkSetMacro(sizeA, unsigned long);
  itkSetMacro(sizeB, unsigned long);

  void SetMapA(std::map<unsigned long,unsigned long> MapA)
  {
      m_MapA= MapA;
	  if (!(m_sizeA > 0)) m_sizeA = m_MapA.size();
  } 

  void SetMapB(std::map<unsigned long,unsigned long> MapB)
  {
      m_MapB= MapB;
	  if (!(m_sizeB > 0)) m_sizeB = m_MapB.size();
  } 
	//itkSetMacro(MapA, std::map<int,int> );
	//itkSetMacro(MapB, std::map<int,int> );

  vnl_matrix<unsigned long> GetTabulateMatrix()
  {
      return m_FinalTable;
  } 

   virtual void Reset() 
  { 
  // Retrieve the number of threads 
   int numberOfThreads = this->GetNumberOfThreads(); 
  // Reset the temporary results container 
  m_TemporaryTables = std::vector<vnl_matrix<unsigned long> >(  numberOfThreads, vnl_matrix<unsigned long>( m_MapA.size(),m_MapB.size()+1,0 )  );
  // Reset the final result 
  m_FinalTable = vnl_matrix<unsigned long>(m_sizeA, m_sizeB+1, 0 ); 
  }

    virtual void Synthetize() 
  { 

  // For each thread 
  for( int threadId = 0; threadId <this->GetNumberOfThreads();++threadId) 
    { 
    // Update final result 
		std::cout << threadId ;
    m_FinalTable+=m_TemporaryTables[threadId]; 
    }  
  }


protected: 

  virtual void ThreadedGenerateData(const RegionType& outputRegionForThread, //int threadId) 
	  itk::ThreadIdType threadId)
  { 
  //// Enable progress reporting 
  //itk::ProgressReporter(this,threadId,outputRegionForThread.GetNumberOfPixels()); 
 
  // Retrieve the input pointer 
ConstPointerType inputPtr = const_cast<ImageType *>(this->GetInput());
 
  // Declare an iterator on the region 
  itk::ImageRegionConstIteratorWithIndex<ImageType> it(inputPtr, outputRegionForThread); 


  // Walk the region of the image with the iterator 
  //for (it.GoToBegin(); !it.IsAtEnd(); ++it, progress.CompletedPixel()) 
  for (it.GoToBegin(); !it.IsAtEnd(); ++it) 
    { 
    // Retrieve pixel value 
    const PixelType& value = it.Get(); 
    unsigned long codeA, codeB;
	bool bOK=true;

    m_MapA.find(value[0]) != m_MapA.end()  ?  codeA=m_MapA.find(value[0])->second  :   bOK=false ; 
    m_MapB.find(value[1]) != m_MapB.end()  ? codeB=m_MapB.find(value[1])->second   :   bOK=false;
  
    // Update temporary results for the current thread 
    if (bOK and (value[0]>0)) m_TemporaryTables[threadId](codeA,codeB)++; 

    }


  }


};

}
#endif
