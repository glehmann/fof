#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkIterativeImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkBinaryContourImageFilter.h"
#include "itkSubtractImageFilter.h"


int main(int argc, char * argv[])
{

  if( argc != 4 )
    {
    std::cerr << "usage: " << argv[0] << " intput output nb" << std::endl;
    std::cerr << " input: the input image" << std::endl;
    std::cerr << " output: the output image" << std::endl;
    // std::cerr << "  : " << std::endl;
    exit(1);
    }

  const int dim = 3;
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::CastImageFilter< IType, IType > CastType;
  CastType::Pointer cast = CastType::New();
  cast->SetInPlace( true );
  
  typedef itk::BinaryContourImageFilter< IType, IType > ContourType;
  ContourType::Pointer contour = ContourType::New();
  contour->SetInput( cast->GetOutput() );
  contour->SetFullyConnected( true );
  contour->SetForegroundValue( 200 );
  
  typedef itk::SubtractImageFilter< IType, IType, IType > SubtractType;
  SubtractType::Pointer subtract = SubtractType::New();
  subtract->SetInput( 0, cast->GetOutput() );
  subtract->SetInput( 1, contour->GetOutput() );
  
  typedef itk::IterativeImageFilter< IType, IType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  filter->SetInputFilter( cast );
  filter->SetOutputFilter( subtract );
  filter->SetNumberOfIterations( atoi(argv[3]) );
  
  itk::SimpleFilterWatcher watcher(filter, "filter");

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();
  
  return 0;
}

