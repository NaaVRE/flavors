#include <cstdlib> /* putenv */
#include <vector>
#include "lwTime.h"
#include "lwString.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "lwLocalZonal.h"

#define SCALAR_INPUT unsigned long
#define SCALAR_OUTPUT unsigned short

int main(int argc, char * argv[])
{
  putenv((char*)"GDAL_CACHEMAX=3000");

  // input arguments
  if(argc < 3+1) {
    printf("Computes the maximum value below a threshold based on the central value of a neighborhood.\n");
    printf("Min three args are mandatory: new output file, input file, zone file.\n");
    printf("One arg is optional: radius of search (10 pixels by default)\n");
    exit(EXIT_FAILURE);
  }
  
  char outputFilename[200];
  sprintf(outputFilename, "%s?&gdal:co:TILED=YES&gdal:co:INTERLEAVE=BAND&gdal:co:BIGTIFF=YES&gdal:co:COMPRESS=LZW", argv[1]);
  std::string inputFilename = argv[2];
  std::string zoneFilename = argv[3];
 
  int radius = 10;
  if(argc > 4)
    radius = atoi(argv[4]);

  double timeBegin = lwTime::get();

  // define the pipeline
  typedef otb::Image<SCALAR_INPUT, 2> inputImageType;
  typedef otb::VectorImage<SCALAR_OUTPUT, 2> outputImageType;
  typedef lwLocalZonal<inputImageType, outputImageType> locZonalFilter;
  otb::ImageFileReader<inputImageType>::Pointer readerF = otb::ImageFileReader<inputImageType>::New();
  otb::ImageFileReader<inputImageType>::Pointer readerZoneF = otb::ImageFileReader<inputImageType>::New();
  locZonalFilter::Pointer majF = locZonalFilter::New();
  otb::ImageFileWriter<outputImageType>::Pointer writerF = otb::ImageFileWriter<outputImageType>::New();

  readerF->SetFileName(inputFilename);
  readerZoneF->SetFileName(zoneFilename);
  majF->SetInput(0, readerF->GetOutput() );
  majF->SetInput(1, readerZoneF->GetOutput() );
  majF->setRadius( radius );
  writerF->SetInput( majF->GetOutput() );
  writerF->SetFileName( outputFilename );

  // execute the pipeline
  writerF->Update();
  
  double totalTime = lwTime::get() - timeBegin;
  printf("\n\nThe end !   Time elapsed = %s\n\n", lwTime::toString(totalTime).c_str() );

}



