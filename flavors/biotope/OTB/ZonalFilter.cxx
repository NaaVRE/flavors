/*
LIFEWATCH_ZONAL_STAT - Copyright (C) <2021>  Université catholique de Louvain (UCLouvain), Belgique,
									
List of the contributors to the development of LIFEWATCH_ZOBAL_STAT: see LICENSE file.
Description and complete License: see LICENSE file.
	
This program (LIFEWATCH_ZONAL_STAT) is free software: 
you can redistribute it and/or modify it under the terms of the 
GNU General Public License as published by the Free Software 
Foundation, either version 3 of the License, or (at your option) 
any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program (see COPYING file).  If not, 
see <http://www.gnu.org/licenses/>.

=========================================================================
  Program:   ORFEO Toolbox Zonal filter
  Language:  C++
  Date:      05/03/2021
  Version:   v4
  Author : Julien Radoux

=========================================================================*/

#include <stdlib.h>
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"
#include "iostream"
#include <map>
#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"
#include "otbZonalPersistentFilterMap.h" 
#include "otbUniqueValuePersistentFilter.h" 
#include "otbPersistentFilterStreamingDecorator.h"
#include <iomanip>

int main(int argc, char * argv[])
{

if (argc < 4)
{
	std::cout << "Zonal statistics filter: usage ZonalStat zone_raster input_raster output_csv (min) (max) (nodata_value)" << std::endl;
	return EXIT_FAILURE;
}

long  noData(0);
double fMaxT(99999999), fMinT(-99999999);

if (argc>=5) fMinT = std::stod(argv[4]);

if (argc>=6) fMaxT = std::stod(argv[5]);

if (argc==7) noData = std::stoi(argv[6]);

std::string outputFileName = argv[3];

  typedef  long												PixelTypeF;
  typedef  long												PixelType;
  typedef otb::VectorImage<PixelTypeF, 2>											VectorImageType;
  typedef otb::Image<PixelType, 2>													ImageType;
  typedef otb::Image<PixelTypeF, 2>													ImageTypeF;

  typedef otb::ImageList<ImageTypeF>													ImageListType;
  typedef otb::ImageListToVectorImageFilter< ImageListType, VectorImageType >		ImageListToVectorImageFilterType;
  
  typedef otb::ImageFileReader<ImageType>											ReaderType;
  typedef otb::ImageFileReader<ImageTypeF>											ReaderTypeF;

typedef otb::ZonalPersistentFilterMap<VectorImageType>								PersistentZonalFilterType; 
typedef otb::UniqueValuePersistentFilter<ImageType>									PersistentUniqueValueFilterType; 
typedef otb::PersistentFilterStreamingDecorator< PersistentZonalFilterType>		StreamingZonalFilterType;
typedef otb::PersistentFilterStreamingDecorator< PersistentUniqueValueFilterType>	StreamingUniqueValueFilterType;
 
  
  ReaderType::Pointer						lReader =		ReaderType::New();
  ReaderTypeF::Pointer						lReader2 =		ReaderTypeF::New();
  ReaderTypeF::Pointer						lReader3 =		ReaderTypeF::New();
  ImageListType::Pointer					imageList  =	ImageListType::New();
  ImageListToVectorImageFilterType::Pointer iL2VI  =		ImageListToVectorImageFilterType::New(); 


  StreamingZonalFilterType::Pointer lZonalFilter = StreamingZonalFilterType::New();  
  StreamingUniqueValueFilterType::Pointer lUniqueFilter = StreamingUniqueValueFilterType::New();  

  lReader->SetFileName(argv[1]); //zone
  lReader->UpdateOutputInformation();
  lReader2->SetFileName(argv[2]); //value
  lReader3->SetFileName(argv[1]); //zone ( une seconde fois pour gérer le type "non int"; nécessaire ? )
  
  lUniqueFilter->GetFilter()->SetInput(lReader->GetOutput());
  lUniqueFilter->Update();
  std::set<PixelType> lookUpA = lUniqueFilter->GetFilter()->GetMap();
  std::cout << "Unique Filter updated (size is " << lookUpA.size() << ")" <<std::endl;
  
  imageList->PushBack(lReader3->GetOutput());
  imageList->PushBack(lReader2->GetOutput());
  iL2VI->SetInput(imageList);
    
  lZonalFilter->GetFilter()->SetInput(iL2VI->GetOutput());
  lZonalFilter->GetFilter()->SetMinRange(fMinT);
  lZonalFilter->GetFilter()->SetMaxRange(fMaxT);
 // lZonalFilter->GetFilter()->SetNoData(noData);

  lZonalFilter->GetFilter()->SetLookupA(lookUpA);
  
  lZonalFilter->Update();
  std::cout << "Zonal Filter updated" <<std::endl;
 std::map< long, std::map<std::string, double> > outputTable = lZonalFilter->GetFilter()->GetFinalTable();

std::ofstream myfile(outputFileName.c_str()) ;

  //stores the names of the fields
  myfile << "zone;occ;mean;std;min;max" << std::endl;
  
std::map< long, std::map<std::string, double> >::const_iterator itCl;
  for(itCl = outputTable.begin(); itCl != outputTable.end(); ++itCl) {
    long zoneClass = itCl->first;
    std::map<std::string, double> values = itCl->second;
    std::ostringstream stLine;
    stLine << zoneClass << ";";
    stLine << values["occ"] << ";" << values["mean"] << ";" << values["std"] << ";";
    stLine << values["min"] << ";" << values["max"];
    myfile << stLine.str() << std::endl;
  }

  myfile.close();

  return EXIT_SUCCESS;
  
  
}
