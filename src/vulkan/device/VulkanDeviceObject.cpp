#include "stdodt.h"

#include "VulkanDeviceObject.h"
#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"

VulkanDeviceObject::VulkanDeviceObject()
	:_device( nullptr )
{
}
VulkanDeviceObject::~VulkanDeviceObject(){
	ASSERT( _device == nullptr );
}
void VulkanDeviceObject::bindDevice( VulkanDevice* device ){
	assert( device );
	if( _device != nullptr ){
		throw std::runtime_error( "VulkanDeviceObject already binded" );
	}
	_device = device;
}
void VulkanDeviceObject::destroy(){
	_device = nullptr;
}
VulkanDevice* VulkanDeviceObject::device() const {
	if( _device == nullptr ){
		throw std::runtime_error( "VulkanDeviceObject not binded" );
	}
	return _device;
}
VkDevice VulkanDeviceObject::vkDevice() const {
	return device()->vkDevice();
}
VulkanPhysicalDevice* VulkanDeviceObject::physicalDevice() const {
	return device()->physicalDevice();
}
