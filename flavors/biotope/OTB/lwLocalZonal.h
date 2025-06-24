#ifndef __lwLocalZonal_h
#define __lwLocalZonal_h
 
#include "itkConnectedComponentAlgorithm.h"
#include "itkConditionalConstIterator.h"

template <class TI, class TO>
class ITK_EXPORT lwLocalZonal: public itk::ImageToImageFilter<TI, TO> {
 public:
  typedef lwLocalZonal Self;
  typedef itk::ImageToImageFilter<TI, TO> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;
  itkNewMacro(Self);
  itkTypeMacro(lwLocalZonal, itk::ImageToImageFilter);
  typedef typename TI::ConstPointer InputImageConstPointerType;
  typedef typename TI::Pointer InputImagePointerType;
  typedef typename TI::RegionType   InputImageRegionType;
  typedef typename TI::PixelType    InputPixelType;
  typedef typename TI::SizeType     InputImageSizeType;
  typedef typename TO::Pointer     OutputImagePointerType;
  typedef typename TO::RegionType  OutputImageRegionType;
  typedef typename TO::PixelType   OutputPixelType;
  typedef typename TO::InternalPixelType   OutputInternalPixelType;
  typedef itk::Image< unsigned char, 2 > TTempImage;

  void setRadius(int radius) { m_radius = radius; }

 protected:
  lwLocalZonal() : m_radius(10) {}
  virtual ~lwLocalZonal() {}
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);
  void GenerateInputRequestedRegion();
  void GenerateOutputInformation();

 private:
  lwLocalZonal(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  int m_radius;
};

#ifndef OTB_MANUAL_INSTANTIATION
#include "lwLocalZonal.txx"
#endif

#endif
