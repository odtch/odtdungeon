#include "stdodt.h"

#include "VulkanQueryPool.h"
#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"

VulkanQueryPool::VulkanQueryPool(){
}
VulkanQueryPool::~VulkanQueryPool(){
	destroy();
	//	ASSERT( _vkCommandPool== nullptr && _queue == nullptr );
}
void VulkanQueryPool::create( uint count, VulkanDevice* device ){
	assert( 0 < count );
	assert( device );
	assert( _device == null );
	if( device->physicalDevice()->properties().limits.timestampComputeAndGraphics != VK_TRUE ){
		throw std::runtime_error( "VulkanDevice doesnt support timestamp for QueryPool" );
	}
	_duration_to_s = device->physicalDevice()->properties().limits.timestampPeriod;
	_duration_to_s /= 1000000000;
	_duration_to_ms = device->physicalDevice()->properties().limits.timestampPeriod;
	_duration_to_ms /= 1000000;
	VkQueryPoolCreateInfo createInfo;
	memset( &createInfo, 0, sizeof( createInfo ) );
	createInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.queryType = VK_QUERY_TYPE_TIMESTAMP;
	createInfo.queryCount = count;
	assert( _vkQueryPool == null );
	Vulkan::check( vkCreateQueryPool( device->vkDevice(), &createInfo, nullptr, &_vkQueryPool ), "vkCreateQueryPool failed" );
	assert( _vkQueryPool );
	_device = device;
	_count = count;
	reset();
}
void VulkanQueryPool::reset(){
	vkResetQueryPool( _device->vkDevice(), _vkQueryPool, 0, _count );
	// see in Task via cmdReset
}
void VulkanQueryPool::destroy(){
	if( _vkQueryPool ){
		vkDestroyQueryPool( _device->vkDevice(), _vkQueryPool, null );
		_vkQueryPool = null;
	}
	_device = null;
}
