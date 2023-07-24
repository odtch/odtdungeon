#include "stdodt.h"

#include "VulkanCommandPool.h"
#include "VulkanQueue.h"
#include "vulkan/device/VulkanDevice.h"
#include "vulkan/device/VulkanPhysicalDevice.h"

VulkanCommandPool::VulkanCommandPool()
	:_queue( nullptr )
	,_vkCommandPool( nullptr )
{
}
VulkanCommandPool::~VulkanCommandPool(){
	if( _vkCommandPool ){
		std::cout << "~VulkanCommandPool not destroyed\n";
	}
	destroy();
	ASSERT( _vkCommandPool== nullptr && _queue == nullptr );
}
void VulkanCommandPool::create( VulkanQueue* queue ){
	assert( queue );
	if( _queue != nullptr || _vkCommandPool != nullptr )
		throw std::runtime_error( "VulkanCommandPool already created" );
	_queue = queue;
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = _queue->family()->index();
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	Vulkan::check( vkCreateCommandPool( _queue->device()->vkDevice(), &poolInfo, nullptr, &_vkCommandPool ), "vkCreateCommandPool failed" );
}
void VulkanCommandPool::destroy(){
	if( _vkCommandPool ){
		vkDestroyCommandPool( _queue->device()->vkDevice(), _vkCommandPool, nullptr);
		_vkCommandPool = nullptr;
	}
	_queue = nullptr;
}
VulkanDevice* VulkanCommandPool::device() const {	
	return queue()->device();
}
