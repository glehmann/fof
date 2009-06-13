/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSeparableBoxImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-23 03:43:42 $
  Version:   $Revision: 1.9 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkSeparableBoxImageFilter_h
#define __itkSeparableBoxImageFilter_h

#include "itkBoxImageFilter.h"


namespace itk {

/**
 * \class SeparableBoxImageFilter
 * \brief Apply the same box filter on all the dimensions with a radius of user defined size on the
 * current dimension and a radius of 0 on all the other dimension.
 *
 * \author Gaetan Lehmann
 *
 */

template<class TImage,
  class TFilter=BoxImageFilter< TImage, TImage > >
class ITK_EXPORT SeparableBoxImageFilter : 
public BoxImageFilter<TImage, TImage>
{
public:
  /** Standard class typedefs. */
  typedef SeparableBoxImageFilter                         Self;
  typedef BoxImageFilter<TImage,TImage>                   Superclass;
  typedef SmartPointer<Self>                              Pointer;
  typedef SmartPointer<const Self>                        ConstPointer;
  
  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(SeparableBoxImageFilter, BoxImageFilter);
 
  /** Image related typedefs. */
  typedef TImage                           ImageType;
  typedef typename TImage::RegionType      RegionType;
  typedef typename TImage::SizeType        SizeType;
  typedef typename TImage::IndexType       IndexType;
  typedef typename TImage::PixelType       PixelType;
  typedef typename TImage::OffsetType      OffsetType;
  
  typedef TFilter                          FilterType;
  
  /** Image related typedefs. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TImage::ImageDimension);

  void SetFilter( FilterType * filter );
  itkGetObjectMacro( Filter, FilterType );

protected:
  SeparableBoxImageFilter();
  ~SeparableBoxImageFilter() {};

  void GenerateData();

  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  SeparableBoxImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  typename FilterType::Pointer       m_Filter;
};

}

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSeparableBoxImageFilter.txx"
#endif

#endif
