/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkIterativeImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-23 03:43:42 $
  Version:   $Revision: 1.9 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkIterativeImageFilter_h
#define __itkIterativeImageFilter_h

#include "itkImageToImageFilter.h"


namespace itk {

/**
 * \class IterativeImageFilter
 * \brief Apply a filter or a pipeline N times iteratively on an image
 *
 *
 * \author Gaetan Lehmann
 *
 */

template<class TInputImage,
  class TOutputImage,
  class TInputFilter=ImageToImageFilter< TInputImage, TOutputImage >,
  class TOutputFilter=ITK_TYPENAME TInputFilter::Superclass >
class ITK_EXPORT IterativeImageFilter : 
public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef IterativeImageFilter                            Self;
  typedef ImageToImageFilter<TInputImage,TOutputImage>    Superclass;
  typedef SmartPointer<Self>                              Pointer;
  typedef SmartPointer<const Self>                        ConstPointer;
  
  /** Superclass typedefs. */
  typedef typename Superclass::InputImagePointer InputImagePointer;

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(IterativeImageFilter, ImageToImageFilter);
 
  /** Image related typedefs. */
  typedef TInputImage                           InputImageType;
  typedef typename TInputImage::RegionType      RegionType;
  typedef typename TInputImage::SizeType        SizeType;
  typedef typename TInputImage::IndexType       IndexType;
  typedef typename TInputImage::PixelType       PixelType;
  typedef typename TInputImage::OffsetType      OffsetType;
  
  typedef TOutputImage                          OutputImageType;
  typedef typename TOutputImage::PixelType      OutputPixelType;

  typedef TInputFilter                          InputFilterType;
  typedef TOutputFilter                         OutputFilterType;
  
  /** Image related typedefs. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  itkSetMacro(NumberOfIterations, unsigned int);
  itkGetConstMacro(NumberOfIterations, unsigned int);


  void SetFilter(InputFilterType * filter);
  InputFilterType * GetFilter()
    {
    return this->m_InputFilter;
    }

  const InputFilterType * GetFilter() const
    {
    return this->m_InputFilter;
    }

  void SetInputFilter( InputFilterType * filter );
  itkGetObjectMacro( InputFilter, InputFilterType );

  void SetOutputFilter( OutputFilterType * filter );
  itkGetObjectMacro( OutputFilter, OutputFilterType );

  /** The current iteration number processed by the filter. This is intended to be
   * used with the IterationEvent sent before the processing of each object. It contains
   * a relevant value only during the filter update.
   */
  itkGetConstMacro(Iteration, unsigned int);

protected:
  IterativeImageFilter();
  ~IterativeImageFilter() {};

  void GenerateData();

  void PrintSelf(std::ostream& os, Indent indent) const;

  void GenerateInputRequestedRegion();
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));

private:
  IterativeImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  typename InputFilterType::Pointer       m_InputFilter;
  typename OutputFilterType::Pointer      m_OutputFilter;
  unsigned int                            m_NumberOfIterations;
  unsigned int                            m_Iteration;
};

}

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkIterativeImageFilter.txx"
#endif

#endif
