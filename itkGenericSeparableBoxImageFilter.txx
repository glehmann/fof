/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGenericSeparableBoxImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2008-08-08 13:54:09 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkGenericSeparableBoxImageFilter_txx
#define __itkGenericSeparableBoxImageFilter_txx

#include "itkGenericSeparableBoxImageFilter.h"
#include "itkProgressAccumulator.h"

namespace itk {

template <class TInputImage, class TOutputImage, class TFilter>
void
GenericSeparableBoxImageFilter<TInputImage, TOutputImage, TFilter>
::GenerateData()
{
  this->AllocateOutputs();
  
  // copy the input image to be sure that the internal filters won't destroy it by
  // running in place
  typedef itk::CastImageFilter< InputImageType, InputImageType> CopyType;
  typename CopyType::Pointer copy = CopyType::New();
  copy->SetInput( this->GetInput() );
  copy->SetInPlace( false );
  copy->SetReleaseDataFlag( true );

  // create the pipeline
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);
  
  typename FilterType::Pointer filters[ImageDimension];
  for( unsigned i = 0; i < ImageDimension; i++ )
    {
    filters[i] = FilterType::New();
    filters[i]->ReleaseDataFlagOn();
    filters[i]->SetNumberOfThreads( this->GetNumberOfThreads() );
    RadiusType rad;
    rad.Fill(0);
    rad[i] = this->GetRadius()[i];
    filters[i]->SetRadius( rad );
    if( i > 0 ) 
      {
      filters[i]->SetInput( filters[i-1]->GetOutput() );
      progress->RegisterInternalFilter( filters[i], 1.0/ImageDimension );
      }
    }
  filters[0]->SetInput( copy->GetOutput() );

  filters[ImageDimension-1]->GraftOutput( this->GetOutput() ); 
  filters[ImageDimension-1]->Update();
  this->GraftOutput( filters[ImageDimension-1]->GetOutput() );

}

}


#endif
