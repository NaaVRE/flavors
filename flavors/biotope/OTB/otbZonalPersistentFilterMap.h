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
#ifndef __otbZonalPersistentFilterMap_h
#define __otbZonalPersistentFilterMap_h

#include "otbPersistentImageFilter.h"
#include <vnl/vnl_matrix.h>
#include <map>
#include <itkImageRegionConstIteratorWithIndex.h>
#include <itkProgressReporter.h>
#include <utility>
#include "lwScalar.h"

namespace otb
{

  template<class TInputImage > 
    class ITK_EXPORT ZonalPersistentFilterMap : 
      public otb::PersistentImageFilter<TInputImage, TInputImage>
  {


    private: 
      // Temporary results container 
      //std::vector< vnl_matrix<int> > m_TemporaryTables; 
      std::vector< std::map< long, std::map<std::string, double> > > m_TemporaryTables;
      // Final result member 
      //vnl_matrix<int> m_FinalTable;
      std::map< long, std::map<std::string, double> > m_FinalTable;

      std::set<long> m_lookupA;
      float m_minRange, m_maxRange;
      int nbResults;

    public:

      /** Standard typedefs */
      typedef ZonalPersistentFilterMap								Self;
      typedef otb::PersistentImageFilter<TInputImage, TInputImage>	Superclass;
      typedef itk::SmartPointer<Self>								Pointer;
      typedef itk::SmartPointer<const Self>							ConstPointer;

      /** Type macro */
      itkNewMacro(Self);

      /** Creation through object factory macro */
      itkTypeMacro(ZonalPersistentFilterMap, otb::PersistentImageFilter);

      /** Template parameters typedefs */
      typedef TInputImage							ImageType;
      typedef typename ImageType::ConstPointer		ConstPointerType;
      typedef typename ImageType::RegionType		RegionType;
      typedef typename ImageType::PixelType			PixelType;

      void SetLookupA(std::set<long > l)
      {
        m_lookupA = l;
      }

      void SetMinRange(float minRange)
      {
        m_minRange= minRange;
      } 
      void SetMaxRange(float maxRange)
      {
        m_maxRange= maxRange;
      } 

      std::map< long, std::map<std::string, double> > GetFinalTable()
      {
        return m_FinalTable;
      } 

      virtual void Reset() 
      { 
        nbResults = 5; // 0: nb, 1: mean, 2: std, 3: min; 4: max
        // Retrieve the number of threads 
        unsigned int  numberOfThreads = this->GetNumberOfThreads(); 
        // Reset the temporary results container 
        //m_TemporaryTables = std::vector<vnl_matrix<int> >(  numberOfThreads, vnl_matrix<int>( m_MapA.size()+1,4,0 )  );
        m_TemporaryTables = std::vector< std::map< long, std::map<std::string,double> > >(  numberOfThreads );//, std::map< std:: >( m_MapA.size()+1, 4, 0 )  );
        for(int iT = 0; iT < numberOfThreads; ++iT) {
          for(std::set<long >::const_iterator itA = m_lookupA.begin(); itA != m_lookupA.end(); ++itA) {
                          long id = *itA;
              m_TemporaryTables[iT][id]["occ"] = 0.;
              m_TemporaryTables[iT][id]["mean"] = 0.;
              m_TemporaryTables[iT][id]["std"] = 0.;
              m_TemporaryTables[iT][id]["min"] = lwScalar::max<typename TInputImage::InternalPixelType>();
              m_TemporaryTables[iT][id]["max"] = lwScalar::min<typename TInputImage::InternalPixelType>();

          }
        }
        // Reset the final result 
        m_FinalTable = std::map< long, std::map<std::string,double> >();
      }

      virtual void Synthetize() 
      { 

        // Update final result 
        //std::cout << threadId ;
        for(std::set<long >::const_iterator itA = m_lookupA.begin(); itA != m_lookupA.end(); ++itA) {
            long id = *itA;
            for(unsigned int threadId = 0; threadId < this->GetNumberOfThreads(); ++threadId)
            {
              std::map<std::string, double> & vars = m_TemporaryTables[threadId][id];
              if(m_FinalTable[id].find("occ") == m_FinalTable[id].end()) { // initialization
                m_FinalTable[id]["occ"]  = vars["occ"];
                m_FinalTable[id]["min"]  = vars["min"];
                m_FinalTable[id]["max"]  = vars["max"];
                m_FinalTable[id]["mean"] = vars["mean"];
                m_FinalTable[id]["std"]   = vars["std"];
              } else { // aggregation
			    if (vars["occ"] >0){
                double old_occ = m_FinalTable[id]["occ"];
                double old_mean = m_FinalTable[id]["mean"];
				double old_var = m_FinalTable[id]["std"];
                m_FinalTable[id]["occ"] += vars["occ"];
                m_FinalTable[id]["min"] = std::min(vars["min"], m_FinalTable[id]["min"]);
                m_FinalTable[id]["max"] = std::max(vars["max"], m_FinalTable[id]["max"]);
                double delta_mean = vars["mean"] - old_mean;
                m_FinalTable[id]["mean"] += delta_mean * vars["occ"] / m_FinalTable[id]["occ"];
                //m_FinalTable[id]["std"] += vars["std"] + delta_mean*delta_mean * vars["occ"] * old_occ / m_FinalTable[id]["occ"];
				m_FinalTable[id]["std"] =  ((old_occ-1)*old_var + (vars["occ"]-1)*vars["std"] )/(old_occ+vars["occ"]-1) + (old_occ*vars["occ"]*delta_mean*delta_mean)/((old_occ+vars["occ"]-1)*(old_occ+vars["occ"]));
				
				}
			  }
			  
            }
            if(m_FinalTable[id]["occ"] < 2)
              m_FinalTable[id]["std"] = 0.;
            else
              m_FinalTable[id]["std"] = sqrt(m_FinalTable[id]["std"] / (m_FinalTable[id]["occ"] - 1));
        
        }

      }


    protected: 

      virtual void ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
      { 
        ConstPointerType inputPtr = const_cast<ImageType *>(this->GetInput());
        itk::ImageRegionConstIteratorWithIndex<ImageType> it(inputPtr, outputRegionForThread); 

        for (it.GoToBegin(); !it.IsAtEnd(); ++it) 
        { 
          const PixelType& value = it.Get(); 
          //std::pair<long int,long int> id = std::make_pair(value[0], value[2]);
          std::map<std::string, double> & vars = m_TemporaryTables[threadId][value[0]];
          if ( (value[1] >= m_minRange) && (value[1] <= m_maxRange) ) {
            vars["occ"] += 1;
            vars["min"] = std::min( (double)value[1], vars["min"] );
            vars["max"] = std::max( (double)value[1], vars["max"] );

            // one-pass variance

			if (vars["occ"]==1){
			    vars["std"]=0;
				vars["mean"]=(double)value[1];
			}
			else{
			    double old_mean = vars["mean"];
                vars["mean"] += (value[1] - old_mean) / vars["occ"];
                vars["std"] += (value[1] - old_mean) * (value[1] - vars["mean"]);
			 }
          }

        }
      }

  };

}
#endif
