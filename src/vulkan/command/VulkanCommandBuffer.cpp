#include "stdodt.h"

#include "VulkanCommandBuffer.h"
#include "VulkanCommandPool.h"
#include "vulkan/device/VulkanDevice.h"
#include "vulkan/device/VulkanPhysicalDevice.h"
#include "VulkanQueue.h"
#include "VulkanFence.h"
#include "VulkanSemaphore.h"
#include "vulkan/resource/VulkanBuffer.h"
#include "vulkan/resource/VulkanImage.h"
#include "vulkan/pipeline/VulkanPipeline.h"
#include "vulkan/pipeline/VulkanPipelineLayout.h"
#include "vulkan/descriptor/VulkanDescriptorSet.h"

VulkanCommandBuffer::VulkanCommandBuffer()
	:_pool( nullptr )
	,_vkCommandBuffer( nullptr )
{
}
VulkanCommandBuffer::~VulkanCommandBuffer(){
	ASSERT( _vkCommandBuffer == nullptr && _pool == nullptr );
	destroy();
}
void VulkanCommandBuffer::begin( VkCommandBufferUsageFlags usageFlags, VulkanCommandPool* pool ){
	assert( pool );
	_pool = pool;
	_waitCount = 0;
	memset( &_submitInfo, 0, sizeof( _submitInfo ) );
	_submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	if( _vkCommandBuffer == null ){
		VkCommandBufferAllocateInfo allocInfo;
		memset( &allocInfo, 0, sizeof( allocInfo ) );
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = _pool->vkCommandPool();
		allocInfo.commandBufferCount = 1;
		assert( _vkCommandBuffer == nullptr );
		Vulkan::check( vkAllocateCommandBuffers( _pool->queue()->device()->vkDevice(), &allocInfo, &_vkCommandBuffer ), "vkAllocateCommandBuffers failed" );
	} else {
		assert( usageFlags & VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT );
	}
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = usageFlags;
	Vulkan::check( vkBeginCommandBuffer( _vkCommandBuffer, &beginInfo ), "vkBeginCommandBuffer failed" );
}
void VulkanCommandBuffer::reset(){
	if( _vkCommandBuffer == null ){
		// reseting a unintialized commandbuffer is ok
	} else {
		Vulkan::check( vkResetCommandBuffer( vkCommandBuffer(), 0 ), "vkResetCommandBuffer failed" );
	}
}
void VulkanCommandBuffer::end(){
	Vulkan::check( vkEndCommandBuffer( vkCommandBuffer() ), "vkEndCommandBuffer failed" );
}
void VulkanCommandBuffer::bindPipeline( VkPipelineBindPoint pipelineBindPoint, VulkanPipeline* pipeline ){
	assert( pipeline );
	vkCmdBindPipeline( vkCommandBuffer(), pipelineBindPoint, pipeline->vkPipeline() );
}
void VulkanCommandBuffer::bindDescriptorSet( VkPipelineBindPoint pipelineBindPoint, VulkanPipelineLayout* pipelineLayout, VulkanDescriptorSet* descriptorSet ){
	assert( pipelineLayout );
	assert( descriptorSet );
	vkCmdBindDescriptorSets( vkCommandBuffer(), pipelineBindPoint,
							 pipelineLayout->vkPipelineLayout(),
							 0, 1, &descriptorSet->vkDescriptorSet(),
							 0, nullptr );
}
//void VulkanCommandBuffer::resetWaits(){
//	_waitCount = 0;
//}
void VulkanCommandBuffer::addWait( VulkanSemaphore& semaphore, VkPipelineStageFlags stage ){
	assert( !semaphore.isNull() );
	assert( _waitCount < Max_Wait_Count );
	_waitSemaphores[ _waitCount ] = semaphore;
	_waitStages[ _waitCount ] = stage;
	_waitCount++;
}
void VulkanCommandBuffer::submit( VulkanFence* fence ){
	submit( null, fence );
}
void VulkanCommandBuffer::submit( VulkanSemaphore* signalSemaphoreOnComplete, VulkanFence* fence ){
	if( _pool == nullptr )
		throw std::runtime_error( "VulkanBuffer not started" );
	//logDebug( "VulkanCommandBuffer.submit", _waitCount );
	VkSemaphore vkWaitSemaphores[ Max_Wait_Count ];
	memset( &vkWaitSemaphores, 0, sizeof( vkWaitSemaphores ) );
	for( int s = 0; s < _waitCount; s++ ){
		vkWaitSemaphores[ s ] = _waitSemaphores[s].vkSemaphore();
	}
	_submitInfo.waitSemaphoreCount = _waitCount;
	_submitInfo.pWaitSemaphores = &vkWaitSemaphores[0];
	_submitInfo.pWaitDstStageMask = _waitStages;
	auto queue = _pool->queue();
	_submitInfo.commandBufferCount = 1;
	_submitInfo.pCommandBuffers = &vkCommandBuffer();
	VkFence vkFence;
	if( fence ){
		vkFence = fence->vkFence();
	} else {
		vkFence = VK_NULL_HANDLE;
	}
	if( signalSemaphoreOnComplete == null ){
		_submitInfo.signalSemaphoreCount = 0;
		_submitInfo.pSignalSemaphores = null;
		//_signalSemaphoreOnComplete = null;
	} else {
		_submitInfo.signalSemaphoreCount = 1;
		_signalSemaphoreOnComplete = *signalSemaphoreOnComplete;
		_submitInfo.pSignalSemaphores = &_signalSemaphoreOnComplete.vkSemaphore();
	}
	Vulkan::check( vkQueueSubmit( queue->vkQueue(), 1, &_submitInfo, vkFence ), "vkQueueSubmit failed" );
}
void VulkanCommandBuffer::execute(){
	if( _pool == nullptr )
		throw std::runtime_error( "VulkanBuffer not started" );
	_pool->queue()->execute( *this );
}
void VulkanCommandBuffer::destroy(){
	if( _vkCommandBuffer ){
		vkFreeCommandBuffers( _pool->queue()->device()->vkDevice(), _pool->vkCommandPool(), 1, &_vkCommandBuffer );
		_vkCommandBuffer = nullptr;
	}
	_pool = nullptr;
}
void VulkanCommandBuffer::copyBuffer( VulkanBuffer& source, VulkanBuffer& target ){
	assert( source.size() <= target.size() );
	VkBufferCopy copyRegion{};
	copyRegion.size = source.size();
	vkCmdCopyBuffer( vkCommandBuffer(), source.vkBuffer(), target.vkBuffer(), 1, &copyRegion );
}
void VulkanCommandBuffer::copyBufferToImage(VulkanBuffer& source, VulkanImage& target, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions ){
	vkCmdCopyBufferToImage( vkCommandBuffer(), source.vkBuffer(), target.vkImage(), dstImageLayout, regionCount, pRegions );
}
void VulkanCommandBuffer::pipelineBarrier( VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers ){
	vkCmdPipelineBarrier( vkCommandBuffer(), srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount,pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers );
}
//void VulkanCommandBuffer::create( VulkanQueueFamily* queueFamily, VulkanDevice* device ){
//	assert( queueFamily );
//	assert( device );
//	if( _device != nullptr || _vkCommandBuffer != nullptr )
//		throw std::runtime_error( "VulkanCommandBuffer already created" );
//	_device = device;
//	VkCommandBufferCreateInfo BufferInfo{};
//	BufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_Buffer_CREATE_INFO;
//	BufferInfo.queueFamilyIndex = queueFamily->index();
//	Vulkan::check( vkCreateCommandBuffer( _device->vkDevice(), &BufferInfo, nullptr, &_vkCommandBuffer ), "vkCreateCommandBuffer failed" );
//}
//void VulkanCommandBuffer::destroy(){
//	if( _vkCommandBuffer ){
//		vkDestroyCommandBuffer( _device->vkDevice(), _vkCommandBuffer, nullptr);
//		_vkCommandBuffer = nullptr;
//	}
//	_device = nullptr;
//}
