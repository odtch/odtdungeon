#include "stdodt.h"

#include "VulkanBufferArray.h"
#include "VulkanBuffer.h"

VulkanBufferArray::VulkanBufferArray( uint32_t maxSize )
    :_maxSize( maxSize )
    ,_size( 0 )
	,_bufferinfos( maxSize )
{
    assert( 0 < _maxSize );
  //  _bufferinfos = new VkDescriptorBufferInfo[ _maxSize ];
    reset();
}
VulkanBufferArray::~VulkanBufferArray(){
	//delete _bufferinfos;
    //_bufferinfos = null;
    _maxSize = 0;
}
void VulkanBufferArray::reset(){
    _size = 0;
	//memset( _bufferinfos, 0, _maxSize * sizeof( VkDescriptorBufferInfo ) );
}
void VulkanBufferArray::add( VulkanBuffer* buffer ){
    assert( buffer );
    assert( _size + 1 < _maxSize );
	assert( buffer->isCreated() );
	VkDescriptorBufferInfo* bufferinfo = &_bufferinfos[ _size ];
	memset( bufferinfo, 0, sizeof( *bufferinfo ) );
    _size++;
    bufferinfo->buffer = buffer->vkBuffer();
    bufferinfo->offset = 0;
	bufferinfo->range = buffer->size();
}
void VulkanBufferArray::set( uint32_t i, VulkanBuffer* buffer ){
	assert( i < _size );
	assert( buffer );
	VkDescriptorBufferInfo* bufferinfo = &_bufferinfos[ i ];
	bufferinfo->buffer = buffer->vkBuffer();
	bufferinfo->offset = 0;
	bufferinfo->range = buffer->size();
}
//void VulkanBufferArray::create( VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VulkanDevice* device ){
//	assert( device );
//	assert( 0 < size );
//	if( _device ){
//		assert( false );
//		throw std::runtime_error( "VulkanBufferArray already created" );
//	}
//	assert( _vkBuffer == nullptr );
//	assert( _vkDeviceMemory == nullptr );
//	_size = size;
//	_device = device;
//	VkBufferCreateInfo bufferInfo;
//	memset( &bufferInfo, 0, sizeof( bufferInfo ) );
//	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//	bufferInfo.size = size;
//	bufferInfo.usage = usage;
//	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
//	Vulkan::check( vkCreateBuffer( _device->vkDevice(), &bufferInfo, nullptr, &_vkBuffer ), "vkCreateBuffer failed" );
//	VkMemoryRequirements memRequirements;
//	vkGetBufferMemoryRequirements( _device->vkDevice(), _vkBuffer, &memRequirements );
//	VkMemoryAllocateFlagsInfo memFlagInfo;
//	memset( &memFlagInfo, 0, sizeof( memFlagInfo ) );
//	memFlagInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
//	if( usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT ){
//		memFlagInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT;
//	}
//	VkMemoryAllocateInfo allocInfo;
//	memset( &allocInfo, 0, sizeof( allocInfo ) );
//	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//	allocInfo.pNext = &memFlagInfo;
//	allocInfo.allocationSize = memRequirements.size;
//	allocInfo.memoryTypeIndex = _device->physicalDevice()->getMemoryTypeIndex( memRequirements.memoryTypeBits, properties );
////	if( usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT ){
////		allocInfo.memoryTypeIndex |= VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT;
////	}
//	Vulkan::check( vkAllocateMemory( _device->vkDevice(), &allocInfo, nullptr, &_vkDeviceMemory ), "vkAllocateMemory failed" );
//	Vulkan::check( vkBindBufferMemory( _device->vkDevice(), _vkBuffer, _vkDeviceMemory, 0 ), "vkBindBufferMemory failed" );
//}
//void VulkanBufferArray::createStaging( const void* data, const size_t& size, VulkanDevice* device ){
//	assert( data );
//	assert( 0 < size );
//	assert( device );
//	create( size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, device );
//	loadStaging( data, size );
//}
//void VulkanBufferArray::loadStaging( const void* data, const size_t& size ){
//	assert( data );
//	assert( 0 < size );
//	assert( _size == size );
//	void* staging_data = nullptr;
//	Vulkan::check( vkMapMemory( device()->vkDevice(), _vkDeviceMemory, 0, _size, 0, &staging_data ), "vkMapMemory failed" );
//	assert( staging_data );
//	memcpy( staging_data, data, size );
//	vkUnmapMemory( _device->vkDevice(), _vkDeviceMemory );

//}
//void VulkanBufferArray::destroy(){
//	if( _vkBuffer ){
//		vkDestroyBuffer( _device->vkDevice(), _vkBuffer, nullptr);
//		_vkBuffer = nullptr;
//	}
//	if( _vkDeviceMemory ){
//		vkFreeMemory( _device->vkDevice(), _vkDeviceMemory, nullptr);
//		_vkDeviceMemory = nullptr;
//	}
//	_device = nullptr;
//	_size = 0;
//}
//void VulkanBuffer::load( VulkanCommandPool& commandPool, const void* data, const size_t& size ){
//	assert( data );
//	if( size != _size ){
//		assert( false );
//		throw std::runtime_error( "VulkanBuffer::load size mismatch" );
//	}
//	assertIsCreated();
//	VulkanBuffer staging;
//	staging.createStaging( data, size, commandPool.device() );
//	VulkanCommandBuffer commandBuffer;
//	commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, &commandPool );
//	commandBuffer.copyBuffer( staging, *this );
//	commandBuffer.end();
//	commandBuffer.execute();
//	commandBuffer.destroy();
//	staging.destroy();
//}
//void VulkanBuffer::assertIsCreated() const {
//	if( _device == nullptr || _vkBuffer == nullptr || _vkDeviceMemory == nullptr ){
//		assert( false );
//		throw std::runtime_error( "VulkanBuffer not created" );
//	}
//}
//VkDeviceAddress VulkanBuffer::getBufferDeviceAddress(){
//	VkBufferDeviceAddressInfo bdai;
//	memset( &bdai, 0, sizeof( bdai ) );
//	bdai.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO_KHR;
//	bdai.buffer = vkBuffer();
//	//auto d = device();
//	VkDeviceAddress vertexAddress = vkGetBufferDeviceAddress( device()->vkDevice(), &bdai );
//	assert( vertexAddress );
//	return vertexAddress;
//}
