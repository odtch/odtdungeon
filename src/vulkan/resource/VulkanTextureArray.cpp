#include "stdodt.h"
#include "VulkanTextureArray.h"
#include "resource/Texture.h"
#include "VulkanSampler.h"
#include "VulkanImage.h"
#include "VulkanImageView.h"
#include "vulkan/command/VulkanCommandPool.h"

VulkanTextureArray::VulkanTextureArray()
	:_commandPool( null )
	,_imageArray( 200 )
{
}
VulkanTextureArray::~VulkanTextureArray(){
	if( _commandPool ){
		std::cout << "~VulkanTextureArray not destroyed\n";
	}
	destroy();
	ASSERT( _commandPool == null );
}
void VulkanTextureArray::create( VulkanCommandPool* commandPool ){
	assert( _commandPool == null );
	assert( commandPool );
	_commandPool = commandPool;
}
void VulkanTextureArray::destroy(){
	_imageArray.reset();
	_commandPool = null;
	while( !_textureSamplers.isEmpty() ){
		VulkanSampler* s = _textureSamplers.takeLast();
		s->destroy();
		delete s;
	}
	while( !_textureImageViews.isEmpty() ){
		VulkanImageView* s = _textureImageViews.takeLast();
		s->destroy();
		delete s;
	}
	while( !_textureImages.isEmpty() ){
		VulkanImage* s = _textureImages.takeLast();
		s->destroy();
		delete s;
	}
	_textures.removeAll();
}
uint VulkanTextureArray::getTextureIndex( Texture* texture ){
	assert( texture );
	int textureIndex = _textures.indexOf( texture );
	if( 0 <= textureIndex )
		return textureIndex;
	assert( _commandPool );
	textureIndex = _textures.size();
	{
		VulkanImage* textureImage = new VulkanImage();
		VulkanImageView* textureImageView = new VulkanImageView();
		VulkanSampler* textureSampler = new VulkanSampler();
		textureImage->load( *_commandPool, *texture );
		textureImageView->create( textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT );
		textureSampler->create( _commandPool->device() );

		_textureImages.add( textureImage );
		_textureImageViews.add( textureImageView );
		_textureSamplers.add( textureSampler );
		_imageArray.add( textureImageView, textureSampler );
		_textures.add( texture );
	}
//	{
////		Box3D bb;
////		mesh->calcBoundingBox( bb );
////		std::cout << "   mesh " << meshIndex << "  bb	" << glm::to_string( bb.min() ) << "-" << glm::to_string( bb.max() ) << "	c=" << glm::to_string( bb.center() ) << "	s=" << glm::to_string( bb.size() ) << " \n";
//	}
//	VulkanMeshBuffer* meshBuffer = new VulkanMeshBuffer();
//	const VkBufferUsageFlags usage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
//									 | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR
//			| VK_BUFFER_USAGE_TRANSFER_DST_BIT;
//	meshBuffer->load( *_commandPool, *texture, usage );
//	vulkan_meshes.add( meshBuffer );
//	vertexBufferArray.add( &meshBuffer->vertexBuffer() );
//	indexBufferArray.add( &meshBuffer->indexBuffer() );
//	VulkanBottomLevelAccelerationStructure* blas = new VulkanBottomLevelAccelerationStructure();
//	blases.add( blas );
//	blas->create( *meshBuffer, *_commandPool );
//	blasesArray.push_back( blas );

	return textureIndex;
}
