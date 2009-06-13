/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGenericSeparableBoxImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2008-08-08 13:19:21 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkGenericSeparableBoxImageFilter_h
#define __itkGenericSeparableBoxImageFilter_h

#include "itkBoxImageFilter.h"
#include "itkCastImageFilter.h"


namespace itk {

/**
 * \class GenericSeparableBoxImageFilter
 * \brief A separable filter for filter which are using radius
 *
 * This filter takes a non separable implementation of a neighborhood
 * filter, and run it several times (one per dimension) to implement
 * the same separable transform.
 * This filter can be used with the filter for which the neighborhood is
 * defined by the SetRadius() method, like the BoxImageFilter and its
 * subcalsses.
 *
 * \author Gaetan Lehmann
 * \author Richard Beare
 */

template<class TInputImage, class TOutputImage, class TFilter, bool TFilterIsThreaded=false>
class ITK_EXPORT GenericSeparableBoxImageFilter : 
public BoxImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef GenericSeparableBoxImageFilter            Self;
  typedef BoxImageFilter<TInputImage,TOutputImage>  Superclass;
  typedef SmartPointer<Self>                        Pointer;
  typedef SmartPointer<const Self>                  ConstPointer;
  
  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(GenericSeparableBoxImageFilter,
               BoxImageFilter);
 
  /** Image related typedefs. */
  typedef TInputImage                               InputImageType;
  typedef typename TInputImage::RegionType          RegionType;
  typedef typename TInputImage::SizeType            SizeType;
  typedef typename TInputImage::IndexType           IndexType;
  typedef typename TInputImage::PixelType           PixelType;
  typedef typename TInputImage::OffsetType          OffsetType;
  
  typedef TOutputImage                              OutputImageType;
  typedef typename TOutputImage::PixelType          OutputPixelType;

  typedef TFilter FilterType;
  typedef CastImageFilter< InputImageType, OutputImageType >
                                                    CastType;
  
  /** Image related typedefs. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  /** n-dimensional Kernel radius. */
  typedef typename TInputImage::SizeType            RadiusType;

protected:
  GenericSeparableBoxImageFilter() {};
  ~GenericSeparableBoxImageFilter() {};

  void GenerateData();
  void ThreadedGenerateData(const RegionType& outputRegionForThread,
                            int threadId );

private:
  GenericSeparableBoxImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

}

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkGenericSeparableBoxImageFilter.txx"
#endif

#endif
