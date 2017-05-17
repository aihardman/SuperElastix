/*=========================================================================
 *
 *  Copyright Leiden University Medical Center, Erasmus University Medical
 *  Center and contributors
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include "selxItkToNiftiImageSourceReferenceComponent.h"
#include "selxCheckTemplateProperties.h"

namespace selx
{
template< class TPixel >
ItkToNiftiImageSourceReferenceComponent< TPixel >::ItkToNiftiImageSourceReferenceComponent( const std::string & name, const LoggerInterface & logger ) : Superclass( name,
  logger), m_Image(nullptr)
{
}


template< class TPixel >
ItkToNiftiImageSourceReferenceComponent< TPixel >::~ItkToNiftiImageSourceReferenceComponent()
{
}



template< class TPixel >
std::shared_ptr<nifti_image>
ItkToNiftiImageSourceReferenceComponent<  TPixel >::GetReferenceNiftiImage()
{
  if( this->m_Image == nullptr )
  {
    throw std::runtime_error( "SourceComponent needs to be initialized by SetMiniPipelineInput()" );
  }
  // TODO
  ItkToNiftiImage<ItkImageType, TPixel>* converter = new ItkToNiftiImage<ItkImageType, TPixel>;
  this->m_Image->GetSource()->UpdateLargestPossibleRegion();
  std::shared_ptr<nifti_image> ptr(converter->Convert(this->m_Image), nifti_image_free);
  return ptr;
}

/*
template< class TPixel >
std::shared_ptr<nifti_image>
ItkToNiftiImageSourceReferenceComponent<  TPixel >::GetFloatingNiftiImage()
{
  std::shared_ptr<nifti_image> ptr(reg_io_ReadImageFile(this->m_ImageFileName.c_str()), nifti_image_free);
  return ptr;
}
*/


template< class TPixel >
std::shared_ptr<nifti_image>
ItkToNiftiImageSourceReferenceComponent<  TPixel >::GetWarpedNiftiImage()
{
  return this->GetReferenceNiftiImage();
}


template< class TPixel >
void
ItkToNiftiImageSourceReferenceComponent< TPixel >
::SetMiniPipelineInput( itk::DataObject::Pointer object )
{
  this->m_Image = dynamic_cast< ItkImageType * >( object.GetPointer() );
  if( this->m_Image == nullptr )
  {
    throw std::runtime_error( "DataObject passed by the NetworkBuilder is not of the right ImageType or not at all an ImageType" );
  }
  return;
}


template< class TPixel >
typename AnyFileReader::Pointer
ItkToNiftiImageSourceReferenceComponent< TPixel >::GetInputFileReader()
{
  // Instanstiate an image file reader, decorated such that it can be implicitly cast to an AnyFileReaderType
  return DecoratedReaderType::New().GetPointer();
}


template< class TPixel >
bool
ItkToNiftiImageSourceReferenceComponent<  TPixel >::MeetsCriterion( const ComponentBase::CriterionType & criterion )
{
  bool hasUndefinedCriteria( false );
  bool meetsCriteria( false );
  auto status = CheckTemplateProperties( this->TemplateProperties(), criterion );
  if( status == CriterionStatus::Satisfied )
  {
    return true;
  }
  else if( status == CriterionStatus::Failed )
  {
    return false;
  } // else: CriterionStatus::Unknown

  return meetsCriteria;
}
} //end namespace selx
