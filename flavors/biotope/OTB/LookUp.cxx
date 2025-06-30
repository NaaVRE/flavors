#include <cstdlib> /* putenv */
#include <vector>
#include <map>
#include "lwTime.h"
#include "lwString.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"
#include "engeLookupFilter.h"

#define SCALAR_INPUT unsigned long int
#define SCALAR_OUTPUT unsigned int

int main(int argc, char * argv[])
{
  putenv((char*)"GDAL_CACHEMAX=2000");

  // input arguments
  if(argc < 3+1)
  {
    printf("Apply Look up table to alter file.");
    printf("Three args are mandatory: new output file, input file, csv file with rules.\n");
	printf("Three args are optional: Background value (default=0), key index (default=0), output index (default = 1).\n");
    printf("The csv file must contain at least 2 columns separated by semicolon: one with input values and one with output values. \n");
    printf("Rem: input values without a match in the LUT will be set to the background value (0 by default).\n");
    exit(EXIT_FAILURE);
  }
  char outputFilename[500];
  sprintf(outputFilename, "%s?&gdal:co:TILED=YES&gdal:co:INTERLEAVE=BAND&gdal:co:BIGTIFF=YES&gdal:co:COMPRESS=LZW", argv[1]);
  std::string inputFilename = argv[2];
  std::string csvFilename = argv[3];
  //define background value
  SCALAR_OUTPUT background(0);
  if(argc > 4)
  {
	  background = lwString::toNum<SCALAR_OUTPUT>(argv[4]);
  }
  
  //define index value for the fields
  unsigned int index_input(0), index_output(1);
   if(argc > 5)
  {
	  index_input = lwString::toNum<unsigned int>(argv[5]);
  }
   if(argc > 6)
  {
	  index_output = lwString::toNum<unsigned int>(argv[6]);
  }
  
  // fill the rules based on csv
  std::map<SCALAR_INPUT, SCALAR_OUTPUT> rules;
  
  // get info from text file 
  std::string line;
  std::ifstream myfile (csvFilename);
  //std::ifstream myfile ("file.csv");

    if (myfile.is_open())
	{
	//first line for field names
		std::getline (myfile,line);
		std::vector<std::string> elems;
		boost::split(elems,line,boost::is_any_of(";"));
		for (int i = 0; i < elems.size(); i++)
		{
			std::cout << elems[i] << " with index " << i;
			if (i==index_input) 
			{
				std::cout << " used as key" << std::endl;
			}
			else if (i==index_output) 
				{
				std::cout << " used as destination" << std::endl;
			}
			else
			{
				std::cout << " not used" << std::endl;
			}
		}
		int numCols = elems.size();
		
        //other lines for the training sample
		std::vector<std::string> elemsb;		
		int skippedLines(0);
		while ( myfile.good() )
		{
			std::getline (myfile,line);
			boost::split(elemsb,line,boost::is_any_of(";"));
			if (elemsb.size()== numCols)
			{
				SCALAR_INPUT keyCode = lwString::toNum<SCALAR_INPUT>(elemsb[index_input]);
				SCALAR_OUTPUT outCode = lwString::toNum<SCALAR_OUTPUT>(elemsb[index_output]);
				rules[ keyCode ] = outCode;
			}
			else
			{
				skippedLines++;
			}
		}
		
		std::cout << skippedLines << " line(s) skipped because of wrong column numbers " << std::endl;
	}
	myfile.close();

  double timeBegin = lwTime::get();

  // define the pipeline
  typedef otb::Image<SCALAR_INPUT, 2> inputImageType;
  typedef otb::Image<SCALAR_OUTPUT, 2> outputImageType;
  typedef engeLookupFilter<inputImageType, outputImageType> LookupFilter;
  otb::ImageFileReader<inputImageType>::Pointer readerF = otb::ImageFileReader<inputImageType>::New();
  LookupFilter::Pointer LookupF = LookupFilter::New();
  otb::ImageFileWriter<outputImageType>::Pointer writerF = otb::ImageFileWriter<outputImageType>::New();

  readerF->SetFileName(inputFilename);
  LookupF->SetInput(readerF->GetOutput() );
  LookupF->setRules( rules );
  LookupF->setBackground( background );
  
  writerF->SetInput( LookupF->GetOutput() );
  writerF->SetFileName( outputFilename );

  // execute the pipeline
  writerF->Update();
  
  double totalTime = lwTime::get() - timeBegin;
  printf("\n\nThe end !   Time elapsed = %s\n\n", lwTime::toString(totalTime).c_str() );

}


