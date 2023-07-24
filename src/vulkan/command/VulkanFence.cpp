#include "stdodt.h"

#include "VulkanFence.h"
#include "vulkan/device/VulkanDevice.h"
#include "utils/DebugCounterTable.h"

#ifdef ODTDEBUG
DebugCounterTable vfc( "VulkanFences" );
#endif

VulkanFence::VulkanFence()
	:SharedPointer<VulkanFenceData>( new VulkanFenceData() )
{
}
VulkanFence::VulkanFence( VulkanFence& data )
	:SharedPointer<VulkanFenceData>( data )
{
}
VulkanFence::~VulkanFence(){
	clear();
}
void VulkanFence::create( const String& debugName, VkFenceCreateFlags flags, VulkanDevice* device ){
	assert( device );
	assert( !isNull() );
	//logDebug( "VulkanFence", debugName );
#ifdef ODTDEBUG
vfc.inc( debugName );
#endif
	if( data()->_vkFence != nullptr )
		throw std::runtime_error( "VulkanFence already created" );	
	data()->_debugname = debugName;
	assert( data()->_vkDevice == null );
	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = flags;
	Vulkan::check( vkCreateFence( device->vkDevice(), &fenceInfo, nullptr, &data()->_vkFence ), "vkCreateFence failed" );
	assert( data()->_vkFence );
	data()->_vkDevice = device->vkDevice();
}
bool VulkanFence::isSignaled(){
	VkResult result = vkGetFenceStatus( data()->_vkDevice, data()->_vkFence );
	if( result == VK_SUCCESS ){
		return true;
	} else if( result == VK_NOT_READY ){
		return false;
	}
	Vulkan::check( result, "vkGetFenceStatus failed" );
	return false;
}
void VulkanFence::waitFor( uint64_t timeout_ns ){
	assert( data()->_vkFence );
	VkResult result = vkWaitForFences( data()->_vkDevice, 1, &data()->_vkFence, VK_TRUE, timeout_ns );
	Vulkan::check( result, "vkWaitForFences failed" );
}
bool VulkanFence::tryWaitFor(uint64_t timeout_ns){
	assert( data()->_vkFence );
	VkResult result = vkWaitForFences( data()->_vkDevice, 1, &data()->_vkFence, VK_TRUE, timeout_ns );
	if( result == VK_SUCCESS ){
		return true;
	} else if( result == VK_NOT_READY ){
		return false;
	}
	Vulkan::check( result, "VulkanFence::tryWaitFor failed" );
	return false;
}
void VulkanFence::reset(){
	assert( data()->_vkFence );
	Vulkan::check( vkResetFences( data()->_vkDevice, 1, &data()->_vkFence ), "vkResetFences failed" );
}
void VulkanFence::destroyData( VulkanFenceData* data ){
	assert( data );
	if( data->_vkFence ){
#ifdef ODTDEBUG
vfc.decr( data->_debugname );
#endif
		vkDestroyFence( data->_vkDevice, data->_vkFence, null );
		data->_vkFence = null;
		data->_vkDevice = null;
	}
	delete data;
}



