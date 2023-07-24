#include "stdodt.h"
#include "VulkanQueue.h"
#include "vulkan/device/VulkanDevice.h"
#include "vulkan/device/VulkanPhysicalDevice.h"
#include "VulkanCommandBuffer.h"
#include "VulkanCommandPool.h"
#include "VulkanFence.h"
#include "VulkanTask.h"

VulkanQueue::VulkanQueue( VkQueue vkQueue, VulkanQueueFamily* family, VulkanDevice* device )
	:_device( device )
	,_vkQueue( vkQueue )
	,_family( family )
{
	assert( _device );
	assert( _vkQueue );
	assert( _family );
}
VulkanQueue::~VulkanQueue(){
	while( !_pendingtasks.isEmpty() ){
		auto task = _pendingtasks.takeFirst();
		delete task;
	}
	_device = null;
	_vkQueue = null;
	_family = null;
}
//bool VulkanQueue::isPhysicalDeviceSupported( VulkanPhysicalDevice& physicalDevice ) const {
//	if( nullptr == findQueueFamilyInPhysicalDevice( physicalDevice ) )
//		return false;
//	return true;
//}
//void VulkanQueue::destroy(){
//	_vkQueue = nullptr;
//	_family = nullptr;
//	_device = nullptr;
//}
void VulkanQueue::submit( uint32_t count, const VkSubmitInfo* submits, VulkanFence* fence ){
	assert( 0 < count );
	assert( submits );
	VkFence vkFence;
	if( fence ){
		vkFence = fence->vkFence();
	} else {
		vkFence = VK_NULL_HANDLE;
	}
//	MutexLocker locker;
//	if( _threaded )
//		locker.lock( _mutex );
	Vulkan::check( vkQueueSubmit( vkQueue(), count, submits, vkFence ), "vkQueueSubmit failed" );
}
void VulkanQueue::submit( VulkanCommandBuffer& commandBuffer, VulkanFence* fence ){
	assert( commandBuffer.pool()->queue() == this );
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer.vkCommandBuffer();
	submit( 1, &submitInfo, fence );
}
void VulkanQueue::execute( VulkanCommandBuffer& commandBuffer ){
	//logDebug( "VulkanQueue::execute" );
	VulkanFence fence;
	//logDebug( "VQ:cf" );
	fence.create( "VulkanQueue", 0, _device );
	//logDebug( "VQ:sub" );
	submit( commandBuffer, &fence );
	//logDebug( "VQ:wait" );
	if( !fence.tryWaitFor() ){
		logDebug( "VQ:wait failed" );
	}
	//logDebug( "VQ:is", fence.isSignaled() );
	ASSERT( fence.isSignaled() );
	//logDebug( "VQ:fd" );
	fence.clear();
}
void VulkanQueue::waitIdle(){
	//logDebug( "VulkanQueue::waitIdle" );
	int fc = 0;
	while( true ){
		VkResult result = vkQueueWaitIdle( vkQueue() );
		if( Vulkan::isSuccess( result ) )
			break;
		std::cout << "VulkanQueue::waitIdle failed " << result << "\n";
		std::cout.flush();
		std::this_thread::sleep_for(std::chrono::milliseconds( 1000 ));
		if( 100 < fc ){
			Vulkan::check( result, "vkQueueWaitIdle failed" );
			return;
		}
		fc++;
	}
	onIdle(); // to refresh tasks
}
void VulkanQueue::onTaskCreated( VulkanTask& task ){
	assert( !task.isNull() );
	_pendingtasks.add( new VulkanTask( task ) );
	//logDebug( "VulkanQueue", task.name(), "created" );
}
void VulkanQueue::onIdle(){
	if( _pendingtasks.size() > 10 ){
		logDebug( "VulkanQueue.onIdle ", _pendingtasks.size(), "tasks pending" );
	}
	for( VulkanTask* task : _pendingtasks ){
		switch( task->refresh() ){
		case VulkanTask::RefreshContinue:
			//logDebug( "VulkanQueue", task->name(), "running" );
			break;
		case VulkanTask::RefreshCompleted:
			//logDebug( "VulkanQueue", task->name(), "completed" );
			_pendingtasks.remove( task );
			delete task;
			onIdle();
			return; // list modified
		default:
			logDebug( "VulkanQueue", task->name(), "refresh result unhandled" );
			ASSERT( false );
		}
	}
}
