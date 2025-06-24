#include <cstdlib> /* putenv */
#include <vector>
#include "lwTime.h"
#include "lwString.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"
#include "engeMajorityFilter.h"

#define SCALAR_INPUT unsigned char
#define SCALAR_OUTPUT unsigned char

int main(int argc, char * argv[])
{
  putenv((char*)"GDAL_CACHEMAX=3000");

  // input arguments
  if(argc < 2+1) {
    printf("Fix pixel value to the more present value in the neighbourhood (window of a given radius around the pixel).\n");
    printf("Min two args are mandatory: input file, new output file.\n");
    printf("Two args are optional: radius of search (2 pixels by default = 5x5 window), no-data value (default 0; value ignored by the algo)\n");
    exit(EXIT_FAILURE);
  }

  std::string inputFilename = argv[1];
  char outputFilename[200];
  sprintf(outputFilename, "%s?&gdal:co:TILED=YES&gdal:co:INTERLEAVE=BAND&gdal:co:BIGTIFF=YES&gdal:co:COMPRESS=LZW", argv[2]);
  unsigned int radius = 2;
  SCALAR_INPUT nodata = 0;
  if(argc > 3)
    radius = atoi(argv[3]);
  if(argc > 4)
    nodata = atoi(argv[4]);

  printf("Starting with radius = %d and nodata = %d\n", radius, nodata);

  double timeBegin = lwTime::get();

  // define the pipeline
  typedef otb::Image<SCALAR_INPUT, 2> inputImageType;
  typedef otb::Image<SCALAR_OUTPUT, 2> outputImageType;
  typedef engeMajorityFilter<inputImageType, outputImageType> majFilter;
  otb::ImageFileReader<inputImageType>::Pointer readerF = otb::ImageFileReader<inputImageType>::New();
  majFilter::Pointer majF = majFilter::New();
  otb::ImageFileWriter<outputImageType>::Pointer writerF = otb::ImageFileWriter<outputImageType>::New();

  readerF->SetFileName(inputFilename);
  majF->SetInput(0, readerF->GetOutput() );
  majF->setRadius( radius );
  majF->setNaN( nodata );
  writerF->SetInput( majF->GetOutput() );
  writerF->SetFileName( outputFilename );

  // execute the pipeline
  writerF->Update();
  
  double totalTime = lwTime::get() - timeBegin;
  printf("\n\nThe end !   Time elapsed = %s\n\n", lwTime::toString(totalTime).c_str() );

}



