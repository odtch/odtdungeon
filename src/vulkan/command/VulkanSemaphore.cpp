#include "stdodt.h"

#include "VulkanSemaphore.h"
#include "vulkan/device/VulkanDevice.h"

VulkanSemaphore::VulkanSemaphore()
	:SharedPointer<VulkanSemaphoreData>( new VulkanSemaphoreData() )
{
}
VulkanSemaphore::VulkanSemaphore(VulkanSemaphore& data)
	:SharedPointer<VulkanSemaphoreData>( data )
{
}
VulkanSemaphore::~VulkanSemaphore(){
	clear();
}
void VulkanSemaphore::create( VulkanDevice* device ){
	assert( device );
	assert( !isNull() );
	if( data()->_vkSemaphore != nullptr )
		throw std::runtime_error( "VulkanSemaphore already created" );
	assert( data()->_vkDevice == null );
	VkSemaphoreCreateInfo semaphoreInfo;
	memset( &semaphoreInfo, 0, sizeof( semaphoreInfo ) );
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	//logDebug( "VulkanSemaphore.create" );
	Vulkan::check( vkCreateSemaphore( device->vkDevice(), &semaphoreInfo, nullptr, &data()->_vkSemaphore ), "vkCreateSemaphore failed" );
	assert( data()->_vkSemaphore );
	data()->_vkDevice = device->vkDevice();
}
void VulkanSemaphore::operator=( const VulkanSemaphore& semaphore ){
	SharedPointer<VulkanSemaphoreData>::operator=( semaphore );
}
void VulkanSemaphore::destroyData( VulkanSemaphoreData* data ){
	assert( data );
	if( data->_vkSemaphore ){
		//logDebug( "VulkanSemaphore.destroy" );
		assert( data->_vkDevice );
		vkDestroySemaphore( data->_vkDevice, data->_vkSemaphore, nullptr );
		data->_vkDevice = null;
		data->_vkSemaphore = null;
	}
	delete data;
}
