/*=========================================================================
  Program:   ORFEO Toolbox Tabulate filter
  Language:  C++
  Date:      9/10/2014
  Version:   v3
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
#include "otbTabulatePersistentFilter.h" 
#include "otbUniqueValuePersistentFilter.h" 
#include "otbPersistentFilterStreamingDecorator.h"

int main(int argc, char * argv[])
{

	
  typedef  unsigned long int															PixelType;
  typedef otb::VectorImage<PixelType, 2>											VectorImageType;
  typedef otb::Image<PixelType, 2>													ImageType;


  typedef otb::ImageList<ImageType>													ImageListType;
  typedef otb::ImageListToVectorImageFilter< ImageListType, VectorImageType >		ImageListToVectorImageFilterType;
  
  typedef otb::ImageFileReader<ImageType>											ReaderType;

typedef otb::TabulatePersistentFilter<VectorImageType>								PersistentTabulateFilterType; 
typedef otb::UniqueValuePersistentFilter<ImageType>									PersistentUniqueValueFilterType; 
typedef otb::PersistentFilterStreamingDecorator< PersistentTabulateFilterType>		StreamingTabulateFilterType;
typedef otb::PersistentFilterStreamingDecorator< PersistentUniqueValueFilterType>	StreamingUniqueValueFilterType;
  
  typedef otb::ImageFileWriter<ImageType>									WriterType;   
  
  


  ReaderType::Pointer						lReader =		ReaderType::New();
  ReaderType::Pointer						lReader2 =		ReaderType::New();
  ImageListType::Pointer					imageList  =	ImageListType::New();
  ImageListToVectorImageFilterType::Pointer iL2VI  =		ImageListToVectorImageFilterType::New(); 


  StreamingTabulateFilterType::Pointer lTabulateFilter = StreamingTabulateFilterType::New();  
  StreamingUniqueValueFilterType::Pointer lUniqueFilter = StreamingUniqueValueFilterType::New();  
  StreamingUniqueValueFilterType::Pointer lUniqueFilter2 = StreamingUniqueValueFilterType::New();  

  WriterType::Pointer lWriter = WriterType::New();


  lReader->SetFileName(argv[1]);//(inputFileName);
  lReader->UpdateOutputInformation();

  
  lUniqueFilter->GetFilter()->SetInput(lReader->GetOutput());
  lUniqueFilter->Update();
  std::set<unsigned long> UniqueA = lUniqueFilter->GetFilter()->GetMap();
  std::cout << "Unique Filter updated (size = " << UniqueA.size() << ")" <<std::endl;


  lReader2->SetFileName(argv[2]);//(inputFileName2);
  lReader2->UpdateOutputInformation();

std::map<unsigned long,unsigned  long> lookUpB, lookUpA;
	
  lUniqueFilter2->GetFilter()->SetInput(lReader2->GetOutput());
  lUniqueFilter2->Update();
  std::set<unsigned long> UniqueB = lUniqueFilter2->GetFilter()->GetMap();
  std::cout << "Unique Filter 2 updated (size = " << UniqueB.size() << ")" <<std::endl;


  unsigned long iCountA(0), iCountB(0);
  std::set<unsigned long>::iterator itSetA, itSetB;
  for (itSetA=UniqueA.begin(); itSetA!=UniqueA.end(); itSetA++)  lookUpA[*itSetA]=iCountA++;
   std::cout << "Zonal map updated (size = " << lookUpA.size() << ")" <<std::endl;
  for (itSetB=UniqueB.begin(); itSetB!=UniqueB.end(); itSetB++)  lookUpB[*itSetB]=iCountB++;
 std::cout << "Value map updated (size = " << lookUpB.size() << ")" <<std::endl;

  imageList->PushBack(lReader->GetOutput());
  imageList->PushBack(lReader2->GetOutput());
  iL2VI->SetInput(imageList);
  //iL2VI->Update();
    
  lTabulateFilter->GetFilter()->SetInput(iL2VI->GetOutput());

  lTabulateFilter->GetFilter()->SetMapA(lookUpA);
  lTabulateFilter->GetFilter()->SetMapB(lookUpB);

 std::string outputFileName = argv[3];

 std::cout << "Tabulate Filter will start with zones from " << argv[1] << " values from " << argv[2] << "and output in " << outputFileName.c_str()  <<   std::endl; 
 


  lTabulateFilter->Update();
  vnl_matrix<unsigned long> outputMat = lTabulateFilter->GetFilter()->GetTabulateMatrix();
 

 
  std::cout << "start writing the file" << std::endl;
 
std::ofstream myfile(outputFileName.c_str()) ;

std::map<unsigned long,unsigned  long>::iterator itA = lookUpA.begin();
std::map< unsigned long,unsigned  long>::iterator itB = lookUpB.begin();
	
//stores the names of the fields
myfile << "_;zone_counter\\class_counter";
		for(std::map<unsigned long, unsigned long>::iterator itB = lookUpB.begin();itB!=lookUpB.end();itB++)
			{
				myfile << ";s" << itB->second ;
			}

		myfile << std::endl;

myfile << "zone_value\\class_value;_";
		for(std::map<unsigned long,unsigned  long>::iterator itB = lookUpB.begin();itB!=lookUpB.end();itB++)
			{
				myfile << ";f" << itB->first ;
			}

		myfile << std::endl;



		
		for (unsigned long i = 0; i< outputMat.rows(); i++)
		{
            std::ostringstream stLine;
			//get the zone value
			stLine << itA->first << ";" << itA->second << ";" ;
			itA++;
			//get the count for each class
			for (unsigned long j = 0; j < outputMat.cols()-1; j++)
			{
				stLine << outputMat.get(i,j) << ";";
			}
			//stores in a line
			myfile << stLine.str() << std::endl;
		}

	
    myfile.close();

  return EXIT_SUCCESS;
}
