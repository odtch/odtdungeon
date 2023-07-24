#include "stdodt.h"

#include "VulkanDescriptorSet.h"
#include "VulkanDescriptorPool.h"
#include "VulkanDescriptorSetLayout.h"
#include "vulkan/device/VulkanDevice.h"
#include "vulkan/resource/VulkanBuffer.h"
#include "vulkan/resource/VulkanBufferArray.h"
#include "vulkan/resource/VulkanImageView.h"
#include "vulkan/resource/VulkanImageArray.h"
#include "vulkan/resource/VulkanSampler.h"
#include "vulkan/resource/VulkanTextureArray.h"
//#include "vulkan/raytracing/VulkanAccelerationStructure.h"

VulkanDescriptorSet::VulkanDescriptorSet()
	:_descriptorPool( nullptr )
	,_vkDescriptorSet( nullptr )
{
	memset( &_buffer_infos, 0, sizeof( _buffer_infos ) );
	memset( &_image_infos, 0, sizeof( _image_infos ) );
	memset( &_writes, 0, sizeof( _writes ) );
	memset( &_writeDescriptorSetAccelerationStructure, 0, sizeof( _writeDescriptorSetAccelerationStructure ) );
}
VulkanDescriptorSet::~VulkanDescriptorSet(){
	ASSERT( _vkDescriptorSet == nullptr );
	VulkanDescriptorSet::destroy();
	ASSERT( _descriptorPool == nullptr );
}
void VulkanDescriptorSet::create( VulkanDescriptorPool* descriptorPool ){
	//std::cout << "VulkanDescriptorSet::create\n";
	bindDevice( descriptorPool->device() );
	if( _vkDescriptorSet != nullptr )
		throw std::runtime_error( "VulkanDescriptorSet already created" );
	assert( descriptorPool );
	assert( _descriptorPool == nullptr );
	_descriptorPool = descriptorPool;
	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo;
	memset( &descriptorSetAllocateInfo, 0, sizeof( descriptorSetAllocateInfo ) );
	descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorSetAllocateInfo.descriptorPool = descriptorPool->vkDescriptorPool();
	descriptorSetAllocateInfo.descriptorSetCount = 1; // allocate a single descriptor set.
	descriptorSetAllocateInfo.pSetLayouts = &_descriptorPool->setlayout()->vkDescriptorSetLayout();	
	Vulkan::check( vkAllocateDescriptorSets( vkDevice(), &descriptorSetAllocateInfo, &_vkDescriptorSet ), "vkAllocateDescriptorSets failed" );
	assert( _vkDescriptorSet );
	assert( setlayout()->bindingCount() <= Max_Bind_Count );
	reset();
}
void VulkanDescriptorSet::destroy(){
	if( _vkDescriptorSet ){
		Vulkan::check( vkFreeDescriptorSets( vkDevice(), _descriptorPool->vkDescriptorPool(), 1, &_vkDescriptorSet ), "vkFreeDescriptorSets failed" );
		_vkDescriptorSet = nullptr;
	}
	_descriptorPool = nullptr;
	VulkanDeviceObject::destroy();
}
void VulkanDescriptorSet::reset(){
	memset( &_writes, 0, sizeof( _writes ) );
	memset( &_writeDescriptorSetAccelerationStructure, 0, sizeof( _writeDescriptorSetAccelerationStructure ) );
}
void VulkanDescriptorSet::bindStorageBuffer( int index, VulkanBuffer& buffer ){
	bindBuffer( index, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, buffer );
}
void VulkanDescriptorSet::bindUniformBuffer( int index, VulkanBuffer& buffer ){
	bindBuffer( index, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, buffer );
}
void VulkanDescriptorSet::bindBuffer( int index, VkDescriptorType descriptorType, VulkanBuffer& buffer ){
	assert( 0 <= index && index < setlayout()->bindingCount() );
	assert( setlayout()->descriptorType( index ) == descriptorType );
	assert( setlayout()->descriptorSize( index ) == 1 );
	VkDescriptorBufferInfo* bufferinfo = &_buffer_infos[ index ];
	bufferinfo->offset = 0;
	if( !buffer.isCreated() ){
		logDebug( "VulkanDescriptorSet.bindBuffer not created" );
		bufferinfo->range = 0;
		bufferinfo->buffer = null;
	} else {
		bufferinfo->range = buffer.size();
		if( bufferinfo->range == 0 ){
			logDebug( "VulkanDescriptorSet.bindBuffer is empty" );
			bufferinfo->buffer = null;
		} else {
			bufferinfo->buffer = buffer.vkBuffer();
		}
	}
	_writes[ index ].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	_writes[ index ].dstBinding = index;
	_writes[ index ].dstArrayElement = 0;
	_writes[ index ].descriptorType = descriptorType;
	_writes[ index ].descriptorCount = 1;
	_writes[ index ].pBufferInfo = bufferinfo;
}
void VulkanDescriptorSet::bindBufferArray( int index, VkDescriptorType descriptorType, VulkanBufferArray* bufferArray ){
	assert( 0 <= index && index < setlayout()->bindingCount() );
	assert( bufferArray );
	assert( setlayout()->descriptorType( index ) == descriptorType );
	uint bufferArraySize = bufferArray->size();
	if( bufferArraySize != setlayout()->descriptorSize( index ) ){
		logError( "VulkanDescriptorSet::bindBufferArray", index, descriptorType, "size mismatch", bufferArraySize, setlayout()->descriptorSize( index ) );
		assert( false );
	}
	_writes[ index ].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	_writes[ index ].dstBinding = index;
	_writes[ index ].dstArrayElement = 0;
	_writes[ index ].descriptorType = descriptorType;
	_writes[ index ].descriptorCount = bufferArray->size();
	if( _writes[ index ].descriptorCount == 0 ){
		logDebug( "VulkanDescriptorSet.bindBufferArray is empty" );
		_writes[ index ].pBufferInfo = null;
	} else {
		_writes[ index ].pBufferInfo = bufferArray->descriptorBufferInfos();
	}
	//std::cout << "VulkanDescriptorSet::bindBufferArray " << index << " size=" << bufferArray->size() << "\n";
}
void VulkanDescriptorSet::bindTexture( int index, VulkanImageView* textureImageView, VulkanSampler* textureSampler ){
	assert( 0 <= index && index < setlayout()->bindingCount() );
	assert( textureImageView ); assert( textureSampler );
	assert( setlayout()->descriptorType( index ) == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER );
	assert( setlayout()->descriptorSize( index ) == 1 );
	VkDescriptorImageInfo* imageinfo = &_image_infos[ index ];
	imageinfo->imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageinfo->imageView = textureImageView->vkImageView();
	imageinfo->sampler = textureSampler->vkSampler();
	_writes[ index ].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	_writes[ index ].dstBinding = index;
	_writes[ index ].dstArrayElement = 0;
	_writes[ index ].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	_writes[ index ].descriptorCount = 1;
	_writes[ index ].pImageInfo = imageinfo;
}
void VulkanDescriptorSet::bindTextureArray( int index, VulkanImageArray* textures ){
	assert( 0 <= index && index < setlayout()->bindingCount() );
	assert( textures ); assert( textures ); assert( 0 < textures->size() );
	assert( setlayout()->descriptorType( index ) == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER );
	assert( setlayout()->descriptorSize( index ) == textures->size() );
	_writes[ index ].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	_writes[ index ].dstBinding = index;
	_writes[ index ].dstArrayElement = 0;
	_writes[ index ].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	_writes[ index ].descriptorCount = textures->size();
	if( _writes[ index ].descriptorCount == 0 ){
		logDebug( "VulkanDescriptorSet::bindTextureArray is empty" );
		_writes[ index ].pImageInfo = null;
	} else {
		_writes[ index ].pImageInfo = textures->descriptorBufferInfos();
	}
}
void VulkanDescriptorSet::bindTextureArray( int index, VulkanTextureArray* textures ){
	assert( textures );
	bindTextureArray( index, &textures->imageArray() );
}
void VulkanDescriptorSet::bindStorageImage( int index, VulkanImageView* imageView ){
	assert( imageView );
	bindStorageImage( index, imageView->vkImageView() );
}
void VulkanDescriptorSet::bindStorageImage(int index, VkImageView imageView ){
	assert( 0 <= index && index < setlayout()->bindingCount() );
	assert( imageView );
	assert( setlayout()->descriptorType( index ) == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE );
	assert( setlayout()->descriptorSize( index ) == 1 );
	VkDescriptorImageInfo* imageinfo = &_image_infos[ index ];
	imageinfo->imageLayout = VK_IMAGE_LAYOUT_GENERAL;
	imageinfo->imageView = imageView;
	_writes[ index ].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	_writes[ index ].dstBinding = index;
	_writes[ index ].dstArrayElement = 0;
	_writes[ index ].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	_writes[ index ].descriptorCount = 1;
	_writes[ index ].pImageInfo = imageinfo;
}
//void VulkanDescriptorSet::bindAccelerationStructure( int index, VulkanAccelerationStructure* structure ){
//	assert( 0 <= index && index < setlayout()->bindingCount() );
//	assert( structure );
//	assert( setlayout()->descriptorType( index ) == VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR );
//	assert( setlayout()->descriptorSize( index ) == 1 );
//	assert( _writeDescriptorSetAccelerationStructure.accelerationStructureCount == 0 );
//	_writeDescriptorSetAccelerationStructure.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR;
//	_writeDescriptorSetAccelerationStructure.accelerationStructureCount = 1;
//	_writeDescriptorSetAccelerationStructure.pAccelerationStructures = &structure->vkAccelerationStructure();
//	_writes[ index ].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
//	_writes[ index ].dstBinding = index;
//	_writes[ index ].dstArrayElement = 0;
//	_writes[ index ].descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
//	_writes[ index ].descriptorCount = 1;
//	_writes[ index ].pNext = &_writeDescriptorSetAccelerationStructure;
//}
void VulkanDescriptorSet::bindAccelerations( int index, uint32_t count, VkWriteDescriptorSetAccelerationStructureKHR* accelerations ){
	assert( 0 <= index && index < setlayout()->bindingCount() );
	assert( accelerations );
	assert( setlayout()->descriptorType( index ) == VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR );
	assert( setlayout()->descriptorSize( index ) == count ); // accelerations->accelerationStructureCount );
	assert( 0 < count );
	_writes[ index ].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	_writes[ index ].dstBinding = index;
	_writes[ index ].dstArrayElement = 0;
	_writes[ index ].descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
	_writes[ index ].descriptorCount = count;
	_writes[ index ].pNext = accelerations;
}
void VulkanDescriptorSet::update(){
	uint32_t count = setlayout()->bindingCount();
	for( uint32_t w = 0; w < count; w++ ){
		assert( _writes[ w ].sType == VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET );
		assert( _writes[ w ].dstBinding == w );
		_writes[ w ].dstSet = vkDescriptorSet();
		ASSERT( 0 < _writes[ w ].descriptorCount );
	}
	vkUpdateDescriptorSets( vkDevice(), count, _writes, 0, nullptr );
}
VulkanDescriptorSetLayout* VulkanDescriptorSet::setlayout() const {
	return pool()->setlayout();
}
