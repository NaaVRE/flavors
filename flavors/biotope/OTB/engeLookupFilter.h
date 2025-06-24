#ifndef __engeLookupFilter_h
#define __engeLookupFilter_h



#include "otbMacro.h"
#include "itkImageToImageFilter.h"
#include <map>

template <class TI, class TO>
class ITK_EXPORT engeLookupFilter : public itk::ImageToImageFilter<TI, TO>
{
public:
  // Standard typedefs and macros
  typedef engeLookupFilter Self;
  typedef itk::ImageToImageFilter<TI, TO> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  itkNewMacro(Self);
  itkTypeMacro(engeLookupFilter, itk::ImageToImageFilter);


  void setRules(std::map<typename TI::InternalPixelType, typename TO::InternalPixelType> rules) { m_rules = rules; }
  void setBackground( typename TO::InternalPixelType background) { m_background = background; }

protected:
  engeLookupFilter() {}
  virtual ~engeLookupFilter() {}
  void ThreadedGenerateData(const typename TO::RegionType & outputRegionForThread, itk::ThreadIdType threadId);

private:
  // copy operators purposely not implemented
  engeLookupFilter(const Self &);
  void operator=(const Self&);

  std::map<typename TI::InternalPixelType, typename TO::InternalPixelType> m_rules;
  typename TO::InternalPixelType m_background;
  
};

#  ifndef ITK_MANUAL_INSTANTIATION
#    include "engeLookupFilter.txx"
#  endif



#endif
