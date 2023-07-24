#include "stdodt.h"

#include "VulkanBuffer.h"
#include "vulkan/device/VulkanDevice.h"
#include "vulkan/device/VulkanPhysicalDevice.h"
#include "vulkan/command/VulkanCommandPool.h"
#include "vulkan/command/VulkanCommandBuffer.h"

VulkanBuffer::VulkanBuffer(){
}
VulkanBuffer::VulkanBuffer(VulkanBuffer& buffer)
	:SharedPointer<VulkanBufferData>( buffer )
{
}
VulkanBuffer::~VulkanBuffer(){
	clear();
}
void VulkanBuffer::create( VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VulkanDevice* device ){
	assert( device );
	assert( 0 < size );
	if( isCreated() ){
		assert( false );
		throw std::runtime_error( "VulkanBuffer already created" );
	}
	VulkanBufferData* data = new VulkanBufferData();
	SharedPointer<VulkanBufferData>::operator=( data );
	data->_size = size;
	data->_device = device;
	VkBufferCreateInfo bufferInfo;
	memset( &bufferInfo, 0, sizeof( bufferInfo ) );
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	Vulkan::check( vkCreateBuffer( data->_device->vkDevice(), &bufferInfo, nullptr, &data->_vkBuffer ), "vkCreateBuffer failed" );
	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements( data->_device->vkDevice(), data->_vkBuffer, &memRequirements );
	VkMemoryAllocateFlagsInfo memFlagInfo;
	memset( &memFlagInfo, 0, sizeof( memFlagInfo ) );
	memFlagInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
	if( usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT ){
		memFlagInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT;
	}
	VkMemoryAllocateInfo allocInfo;
	memset( &allocInfo, 0, sizeof( allocInfo ) );
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.pNext = &memFlagInfo;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = data->_device->physicalDevice()->getMemoryTypeIndex( memRequirements.memoryTypeBits, properties );
//	if( usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT ){
//		allocInfo.memoryTypeIndex |= VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT;
//	}
	Vulkan::check( vkAllocateMemory( data->_device->vkDevice(), &allocInfo, nullptr, &data->_vkDeviceMemory ), "vkAllocateMemory failed" );
    Vulkan::check( vkBindBufferMemory( data->_device->vkDevice(), data->_vkBuffer, data->_vkDeviceMemory, 0 ), "vkBindBufferMemory failed" );
//#ifdef ODTDEBUG
//    if( usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT ){
//        static int no = 0;
//        no++;
//        VkDeviceAddress address = getBufferDeviceAddress();
//        std::cout << "VulkanBuffer " << no << " "  << address << " " << ((uint64_t)address);
//        printf( " 0x%llx\n", address );
//    }
//    {
//        static int no = 0;
//        no++;
//        std::cout << "VulkanBuffer " << no << " " << data->_vkBuffer << " " << size;
//        printf( ": 0x%llx\n", data->_vkBuffer );
//    }
//#endif
}
void VulkanBuffer::createStaging( const void* data, const size_t& size, VulkanDevice* device ){
	assert( data );
	assert( 0 < size );
	assert( device );
	create( size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, device );
	loadStaging( data, size );
}
void VulkanBuffer::loadStaging( const void* memory, const size_t& size ){
	assert( memory );
	assert( 0 < size );
	auto data = this->data();
	assert( data->_size == size );
	void* staging_data = nullptr;
	Vulkan::check( vkMapMemory( device()->vkDevice(), data->_vkDeviceMemory, 0, data->_size, 0, &staging_data ), "vkMapMemory failed" );
	assert( staging_data );
	memcpy( staging_data, memory, size );
	vkUnmapMemory( data->_device->vkDevice(), data->_vkDeviceMemory );
}
void VulkanBuffer::clear(){
	SharedPointer<VulkanBufferData>::clear();
}
void VulkanBuffer::load( VulkanCommandPool& commandPool, const void* memory, const size_t& size ){
	assert( memory );
	auto data = this->data();
	if( size != data->_size ){
		//std::cout << "VulkanBuffer::load size mismatch is " << _size << " new " << size << "\n"; std::cout.flush();
		if( size <= data->_size ){
			// ok?
		} else {
			assert( false );
			throw std::runtime_error( "VulkanBuffer::load size mismatch" );
		}
	}
	assertIsCreated();
	VulkanBuffer staging;
	staging.createStaging( memory, size, commandPool.device() );
	VulkanCommandBuffer commandBuffer;
	commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, &commandPool );
	commandBuffer.copyBuffer( staging, *this );
	commandBuffer.end();
	commandBuffer.execute();
	commandBuffer.destroy();
	staging.clear();
}
void VulkanBuffer::assertIsCreated() const {
	auto data = this->data();
	if( data->_device == nullptr || data->_vkBuffer == nullptr || data->_vkDeviceMemory == nullptr ){
		assert( false );
		throw std::runtime_error( "VulkanBuffer not created" );
	}
}
VkDeviceAddress VulkanBuffer::getBufferDeviceAddress(){
	VkBufferDeviceAddressInfo bdai;
	memset( &bdai, 0, sizeof( bdai ) );
	bdai.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO_KHR;
	bdai.buffer = vkBuffer();
	//auto d = device();
	VkDeviceAddress vertexAddress = vkGetBufferDeviceAddress( device()->vkDevice(), &bdai );
	assert( vertexAddress );
	return vertexAddress;
}
void VulkanBuffer::destroyData( VulkanBufferData* data ){
	assert( data );
	if( data->_vkBuffer ){
		vkDestroyBuffer( data->_device->vkDevice(), data->_vkBuffer, nullptr);
		data->_vkBuffer = nullptr;
	}
	if( data->_vkDeviceMemory ){
		vkFreeMemory( data->_device->vkDevice(), data->_vkDeviceMemory, nullptr);
		data->_vkDeviceMemory = nullptr;
	}
	data->_device = nullptr;
	data->_size = 0;
	delete data;
}

VulkanLoadBufferCommand::VulkanLoadBufferCommand( VulkanBuffer& buffer, const void* data, const size_t& size)
	:_buffer( buffer )
{
	assert( !_buffer.isNull() );
	_staging.createStaging( data, size, _buffer.device() );
}
VulkanLoadBufferCommand::~VulkanLoadBufferCommand(){
}
void VulkanLoadBufferCommand::bind( VulkanCommandBuffer& commandBuffer ){
	ASSERT( !_buffer.data()->_uploading );
	ASSERT( !_staging.data()->_uploading );
	_buffer.data()->_uploading = true;
	_staging.data()->_uploading = true;
	commandBuffer.copyBuffer( _staging, _buffer );
}
void VulkanLoadBufferCommand::onCompleted(){
	ASSERT( _buffer.data()->_uploading );
	ASSERT( _staging.data()->_uploading );
	_buffer.data()->_uploading = false;
	_staging.data()->_uploading = false;
	_buffer.clear();
	_staging.clear();
}
