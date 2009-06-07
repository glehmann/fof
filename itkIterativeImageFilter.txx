/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkIterativeImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2008-12-04 18:37:10 $
  Version:   $Revision: 1.9 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkIterativeImageFilter_txx
#define __itkIterativeImageFilter_txx

#include "itkIterativeImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkProgressReporter.h"

namespace itk
{

template <class TInputImage, class TOutputImage, class TInputFilter, class TOutputFilter>
IterativeImageFilter<TInputImage, TOutputImage, TInputFilter, TOutputFilter>
::IterativeImageFilter()
{
  m_InputFilter = NULL;
  m_OutputFilter = NULL;
  this->m_NumberOfIterations = 1;
  m_Iteration = 0;
}


template <class TInputImage, class TOutputImage, class TInputFilter, class TOutputFilter>
void
IterativeImageFilter<TInputImage, TOutputImage, TInputFilter, TOutputFilter>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  typename InputImageType::Pointer  inputPtr =
    const_cast< InputImageType * >( this->GetInput() );

  if ( !inputPtr )
    { return; }

  inputPtr->SetRequestedRegion(inputPtr->GetLargestPossibleRegion());
}


template <class TInputImage, class TOutputImage, class TInputFilter, class TOutputFilter>
void
IterativeImageFilter<TInputImage, TOutputImage, TInputFilter, TOutputFilter>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template <class TInputImage, class TOutputImage, class TInputFilter, class TOutputFilter>
void
IterativeImageFilter<TInputImage, TOutputImage, TInputFilter, TOutputFilter>
::SetFilter( InputFilterType * filter )
{
  OutputFilterType * outputFilter = dynamic_cast< OutputFilterType * >( filter );
  if( outputFilter == NULL && filter != NULL )
    {
    // TODO: can it be replaced by a concept check ?
    itkExceptionMacro("Wrong output filter type. Use SetOutputFilter() and SetInputFilter() instead of SetFilter() when input and output filter types are different.");
    }
  this->SetInputFilter( filter );
  this->SetOutputFilter( outputFilter );
}


template <class TInputImage, class TOutputImage, class TInputFilter, class TOutputFilter>
void
IterativeImageFilter<TInputImage, TOutputImage, TInputFilter, TOutputFilter>
::SetInputFilter( InputFilterType * filter )
{
  if( m_InputFilter.GetPointer() != filter )
    {
    this->Modified();
    m_InputFilter = filter;
    }
}


template <class TInputImage, class TOutputImage, class TInputFilter, class TOutputFilter>
void
IterativeImageFilter<TInputImage, TOutputImage, TInputFilter, TOutputFilter>
::SetOutputFilter( OutputFilterType * filter )
{
  if( m_OutputFilter.GetPointer() != filter )
    {
    this->Modified();
    m_OutputFilter = filter;
    }
}


template <class TInputImage, class TOutputImage, class TInputFilter, class TOutputFilter>
void
IterativeImageFilter<TInputImage, TOutputImage, TInputFilter, TOutputFilter>
::GenerateData()
{
  if( !m_InputFilter)
    {
    itkExceptionMacro("InputFilter must be set.");
    }

  if( !m_OutputFilter )
    {
    itkExceptionMacro("OutputFilter must be set.");
    }

  for( unsigned int i = 1; i < this->GetNumberOfInputs(); i++ )
    {
    if ( this->GetInput()->GetRequestedRegion().GetSize() != this->GetInput( i )->GetRequestedRegion().GetSize() )
      {
      itkExceptionMacro( << "Inputs must have the same size." );
      }
    }

  this->AllocateOutputs();
  
  // copy the input image to be sure that the internal filters won't destroy it by
  // running in place
  typedef itk::CastImageFilter< InputImageType, InputImageType> CopyType;
  typename CopyType::Pointer copy = CopyType::New();
  copy->SetInput( this->GetInput() );
  copy->SetInPlace( false );
  copy->SetReleaseDataFlag( true );
  
  // the input image for the iteration. The first iteration is done with the input image,
  // the others with the output of the cast filter
  typename InputImageType::Pointer img = copy->GetOutput();

  typedef itk::CastImageFilter< OutputImageType, InputImageType> CastType;
  typename CastType::Pointer cast = CastType::New();
  cast->SetInput( m_OutputFilter->GetOutput() );
  // avoid copying something, if possible.
  cast->SetInPlace( true );

  m_OutputFilter->GraftOutput( this->GetOutput() );

  // Create a process accumulator for tracking the progress of this minipipeline
  ProgressReporter progress( this, 0, m_NumberOfIterations );

  for( m_Iteration=0; m_Iteration<m_NumberOfIterations; m_Iteration++ )
   {
   m_InputFilter->SetInput( img );
   if( m_Iteration != m_NumberOfIterations-1)
     {
     cast->Update();
     img = cast->GetOutput();
     img->DisconnectPipeline();
     progress.CompletedPixel();
     }
   }

  m_OutputFilter->Update();
  progress.CompletedPixel();
  this->GraftOutput( m_OutputFilter->GetOutput() );
}


template <class TInputImage, class TOutputImage, class TInputFilter, class TOutputFilter>
void
IterativeImageFilter<TInputImage, TOutputImage, TInputFilter, TOutputFilter>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "NumberOfIterations: " << this->m_NumberOfIterations << std::endl;
  os << indent << "InputFilter: " << this->m_InputFilter->GetNameOfClass() 
     << " " << this->m_InputFilter.GetPointer() << std::endl;
  os << indent << "OutputFilter: " << this->m_OutputFilter->GetNameOfClass() 
     << " " << this->m_OutputFilter.GetPointer() << std::endl;
  os << indent << "Iteration: " << m_Iteration << std::endl;
}

}


#endif
