
/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSeparableBoxImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2008-12-04 18:37:10 $
  Version:   $Revision: 1.9 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkSeparableBoxImageFilter_txx
#define __itkSeparableBoxImageFilter_txx

#include "itkSeparableBoxImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkProgressReporter.h"

namespace itk
{

template <class TImage, class TFilter>
SeparableBoxImageFilter<TImage, TFilter>
::SeparableBoxImageFilter()
{
  m_Filter = NULL;
}


template <class TImage, class TFilter>
void
SeparableBoxImageFilter<TImage, TFilter>
::SetFilter( FilterType * filter )
{
  if( m_Filter.GetPointer() != filter )
    {
    this->Modified();
    m_Filter = filter;
    }
}


template <class TImage, class TFilter>
void
SeparableBoxImageFilter<TImage, TFilter>
::GenerateData()
{
  this->AllocateOutputs();
  
  typename ImageType::ConstPointer img = this->GetInput();

  // Create a process accumulator for tracking the progress of this minipipeline
  ProgressReporter progress( this, 0, ImageDimension );

  for( int d=0; d<ImageDimension; d++ )
   {
   m_Filter->SetInput( img );
   if( d != ImageDimension-1)
     {
     m_Filter->GetOutput()->SetRequestedRegion( this->GetInput()->GetRequestedRegion() );
     m_Filter->Update();
     typename ImageType::Pointer out = m_Filter->GetOutput();
     out->DisconnectPipeline();
     // required, because img is const and thus doesn't allow to run DisconnectPipeline()
     img = out;
     progress.CompletedPixel();
     }
   }

  m_Filter->Update();
  progress.CompletedPixel();
  this->GraftOutput( m_Filter->GetOutput() );
}


template <class TImage, class TFilter>
void
SeparableBoxImageFilter<TImage, TFilter>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Filter: " << this->m_Filter->GetNameOfClass() 
     << " " << this->m_Filter.GetPointer() << std::endl;
}

}


#endif
