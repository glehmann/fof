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
#include "itkCastImageFilter.h"
#include "itkExtractImageFilter.h"
#include "itkProgressAccumulator.h"

namespace itk {

template <class TInputImage, class TOutputImage, class TFilter, bool TFilterIsThreaded>
void
GenericSeparableBoxImageFilter<TInputImage, TOutputImage, TFilter, TFilterIsThreaded>
::GenerateData()
{

  if( !TFilterIsThreaded )
    {
    // call threaded implementation
    Superclass::GenerateData();
    return;
    }

  this->AllocateOutputs();
  
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
  filters[0]->SetInput( this->GetInput() );

  filters[ImageDimension-1]->GraftOutput( this->GetOutput() ); 
  filters[ImageDimension-1]->Update();
  this->GraftOutput( filters[ImageDimension-1]->GetOutput() );

}


template <class TInputImage, class TOutputImage, class TFilter, bool TFilterIsThreaded>
void
GenericSeparableBoxImageFilter<TInputImage, TOutputImage, TFilter, TFilterIsThreaded>
::ThreadedGenerateData(const RegionType& outputRegionForThread, int threadId )
{
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);
  
  // extract the input region required for this thread. It is enlarged by the radius
  // of the requested box to ensure that the filters have enough input to work properly
  RegionType inputRegion = outputRegionForThread;
  inputRegion.PadByRadius( this->GetRadius() );
  inputRegion.Crop(this->GetOutput()->GetLargestPossibleRegion() );
  
  typedef itk::ExtractImageFilter< InputImageType, InputImageType> CopyType;
  typename CopyType::Pointer copy = CopyType::New();
  copy->SetInput( this->GetInput() );
  copy->SetReleaseDataFlag( true );
  copy->SetNumberOfThreads( 1 );
  copy->SetExtractionRegion( inputRegion );
  if( threadId == 0 )
    {
    progress->RegisterInternalFilter( copy, 1.0/(ImageDimension+2) );
    }
    
  typename FilterType::Pointer filters[ImageDimension];
  for( unsigned i = 0; i < ImageDimension; i++ )
    {
    filters[i] = FilterType::New();
    filters[i]->ReleaseDataFlagOn();
    filters[i]->SetNumberOfThreads( 1 );
    RadiusType rad;
    rad.Fill(0);
    rad[i] = this->GetRadius()[i];
    filters[i]->SetRadius( rad );
    if( i > 0 ) 
      {
      filters[i]->SetInput( filters[i-1]->GetOutput() );
      if( threadId == 0 )
        {
        progress->RegisterInternalFilter( filters[i], 1.0/(ImageDimension+2) );
        }
      }
    }
  filters[0]->SetInput( copy->GetOutput() );
  filters[ImageDimension-1]->GetOutput()->SetRequestedRegion( outputRegionForThread );
  filters[ImageDimension-1]->Update();

  ImageRegionConstIterator<OutputImageType> inIt
    = ImageRegionConstIterator<OutputImageType>( filters[ImageDimension-1]->GetOutput(), outputRegionForThread );
  ImageRegionIterator<OutputImageType> outIt
    = ImageRegionIterator<OutputImageType>( this->GetOutput(), outputRegionForThread );
  outIt.GoToBegin(); 
  inIt.GoToBegin(); 

  float lastPart = 1.0/(ImageDimension+2);
  ProgressReporter progress2(this, threadId, outputRegionForThread.GetNumberOfPixels(), 20, 1-lastPart, lastPart);
  while( !outIt.IsAtEnd() )
    {
    outIt.Set( inIt.Get() );
    ++outIt;
    ++inIt;
    progress2.CompletedPixel();
    }
}

}


#endif
