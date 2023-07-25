#include "stdodt.h"
#include "VulkanAccelerationStructure.h"
#include "VulkanAccelerationStructureExtension.h"
#include "vulkan/device/VulkanDevice.h"
#include "vulkan/resource/VulkanMeshBuffer.h"
#include "vulkan/command/VulkanCommandPool.h"
#include "vulkan/command/VulkanCommandBuffer.h"

VulkanAccelerationStructure::VulkanAccelerationStructure()
	:_vkAccelerationStructure( nullptr )
{
}
VulkanAccelerationStructure::~VulkanAccelerationStructure(){
	ASSERT( _vkAccelerationStructure == nullptr );
}

void VulkanAccelerationStructure::bindDevice(VulkanDevice *device)
{
	VulkanDeviceObject::bindDevice( device );
	_extension = device->getExtension<VulkanAccelerationStructureExtension>();
}
void VulkanAccelerationStructure::create( VkAccelerationStructureTypeKHR type, VkDeviceSize size ){
	assert( 0 < size );
	assert( _vkAccelerationStructure == nullptr );
	VkAccelerationStructureCreateInfoKHR createInfo;
	memset( &createInfo, 0, sizeof( createInfo ) );
	createInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
	createInfo.type = type;
	createInfo.size = size;
	// Actual allocation of buffer and acceleration structure. Note: This relies on createInfo.offset == 0
	// and fills in createInfo.buffer with the buffer allocated to store the BLAS. The underlying
	// vkCreateAccelerationStructureKHR call then consumes the buffer value.
	_buffer.create( createInfo.size, VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, device() );
	createInfo.buffer = _buffer.vkBuffer();
	VkAccelerationStructureKHR vkAccelerationStructure = null;
	Vulkan::check( extension()->createAccelerationStructureKHR()( device()->vkDevice(), &createInfo, nullptr, &vkAccelerationStructure ), "vkCreateAccelerationStructureKHR failed" );
	assert( vkAccelerationStructure );
	bind( vkAccelerationStructure );
}
void VulkanAccelerationStructure::bind( VkAccelerationStructureKHR vkAccelerationStructure ){
	assert( _vkAccelerationStructure == nullptr );
	assert( vkAccelerationStructure != nullptr );
	_vkAccelerationStructure = vkAccelerationStructure;
}
void VulkanAccelerationStructure::destroy(){
	if( _vkAccelerationStructure != nullptr ){
		PFN_vkDestroyAccelerationStructureKHR DestroyAccelerationStructureKHR = reinterpret_cast<PFN_vkDestroyAccelerationStructureKHR>(
		vkGetDeviceProcAddr( device()->vkDevice(), "vkDestroyAccelerationStructureKHR" ) );
		assert( DestroyAccelerationStructureKHR );
		DestroyAccelerationStructureKHR( vkDevice(), _vkAccelerationStructure, nullptr );
		_vkAccelerationStructure = nullptr;
	}
	_buffer.clear();
	VulkanDeviceObject::destroy();
}

