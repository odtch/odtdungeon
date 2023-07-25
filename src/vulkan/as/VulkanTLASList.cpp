#include "stdodt.h"
#include "VulkanTLASList.h"
#include "VulkanTLAS.h"
//#include "vulkan/OdtVulkan.h"
//#include "resource/Texture.h"

VulkanTLASList::VulkanTLASList(){
	memset( &_descriptor_info, 0, sizeof( _descriptor_info ) );
	memset( &_accelerations, 0, sizeof( _accelerations ) );
}
VulkanTLASList::~VulkanTLASList(){
	ASSERT( _commandPool == null );
}
void VulkanTLASList::create( VulkanCommandPool* commandPool ){
	assert( _commandPool == null );
	assert( commandPool );
	_commandPool = commandPool;
	_next_tlas_index = 0;
}
void VulkanTLASList::destroy(){
	while( !_tlases.isEmpty() ){
		auto tlas = _tlases.takeLast();
		tlas->destroy();
		delete tlas;
	}
	_commandPool = null;
}
void VulkanTLASList::reset(){
	_next_tlas_index = 0;
}
VulkanTLAS* VulkanTLASList::get( int index ){
	assert( 0 <= index && index < (int) _next_tlas_index );
	return _tlases.get( index );
}
VulkanTLAS* VulkanTLASList::next(){
	VulkanTLAS* tlas;
	if( _next_tlas_index == (uint32_t) _tlases.size() ){
		assert( _next_tlas_index < MaxTlasCount );
		tlas = new VulkanTLAS( _next_tlas_index );
		_tlases.add( tlas );
	} else {
		tlas = _tlases.get( _next_tlas_index );
	}
	_next_tlas_index++;
	tlas->reset();
	return tlas;
}
void VulkanTLASList::update(){
	uint32_t i = 0;
	for( auto tlas : _tlases ){
		assert( i == tlas->index() );
		if( tlas->isEmpty() ){
			_accelerations[ i ] = null;
		} else {
			_accelerations[ i ] = tlas->vkAccelerationStructure();
		}
		i++;
	}
	_descriptor_info.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR;
	_descriptor_info.accelerationStructureCount = _next_tlas_index;
	_descriptor_info.pAccelerationStructures = &_accelerations[ 0 ];
}
//	_texturesArray.reset();
//	while( !_textureSamplers.isEmpty() ){
//		VulkanSampler* s = _textureSamplers.takeLast();
//		s->destroy();
//		delete s;
//	}
//	while( !_textureImageViews.isEmpty() ){
//		VulkanImageView* s = _textureImageViews.takeLast();
//		s->destroy();
//		delete s;
//	}
//	while( !_textureImages.isEmpty() ){
//		VulkanImage* s = _textureImages.takeLast();
//		s->destroy();
//		delete s;
//	}
//	_textures.removeAll();
//uint VulkanTLASList::getTextureIndex( Texture* texture ){
//	assert( texture );
//	int textureIndex = _textures.indexOf( texture );
//	if( 0 <= textureIndex )
//		return textureIndex;
//	assert( _commandPool );
//	textureIndex = _textures.size();
//	{
//		VulkanImage* textureImage = new VulkanImage();
//		VulkanImageView* textureImageView = new VulkanImageView();
//		VulkanSampler* textureSampler = new VulkanSampler();
//		textureImage->loadFile( *_commandPool, texture->filename() );
//		textureImageView->create( textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT );
//		textureSampler->create( _commandPool->device() );

//		_textureImages.add( textureImage );
//		_textureImageViews.add( textureImageView );
//		_textureSamplers.add( textureSampler );
//		_texturesArray.add( textureImageView, textureSampler );
//		_textures.add( texture );
//	}
////	{
//////		Box3D bb;
//////		mesh->calcBoundingBox( bb );
//////		std::cout << "   mesh " << meshIndex << "  bb	" << glm::to_string( bb.min() ) << "-" << glm::to_string( bb.max() ) << "	c=" << glm::to_string( bb.center() ) << "	s=" << glm::to_string( bb.size() ) << " \n";
////	}
////	VulkanMeshBuffer* meshBuffer = new VulkanMeshBuffer();
////	const VkBufferUsageFlags usage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
////									 | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR
////			| VK_BUFFER_USAGE_TRANSFER_DST_BIT;
////	meshBuffer->load( *_commandPool, *texture, usage );
////	vulkan_meshes.add( meshBuffer );
////	vertexBufferArray.add( &meshBuffer->vertexBuffer() );
////	indexBufferArray.add( &meshBuffer->indexBuffer() );
////	VulkanBottomLevelAccelerationStructure* blas = new VulkanBottomLevelAccelerationStructure();
////	blases.add( blas );
////	blas->create( *meshBuffer, *_commandPool );
////	blasesArray.push_back( blas );

//	return textureIndex;
//}
