#pragma once

#include "vulkan/VulkanCore.h"
#include "VulkanSemaphore.h"

class VulkanCommandBuffer
{
private:
	VulkanCommandPool* _pool;
private:
	VkCommandBuffer _vkCommandBuffer;
private:
	enum { Max_Wait_Count = 40 };
	uint32_t _waitCount = 0;
	VulkanSemaphore _waitSemaphores[ Max_Wait_Count ];
	VkPipelineStageFlags _waitStages[ Max_Wait_Count ];
private:
	VulkanSemaphore _signalSemaphoreOnComplete;
	VkSubmitInfo _submitInfo;
public:
	explicit VulkanCommandBuffer();
	~VulkanCommandBuffer();
public:
	void begin( VkCommandBufferUsageFlags usageFlags, VulkanCommandPool* pool );
	void reset();
	void end();
public:
	void bindPipeline( VkPipelineBindPoint pipelineBindPoint, VulkanPipeline* pipeline );
	void bindDescriptorSet( VkPipelineBindPoint pipelineBindPoint, VulkanPipelineLayout* pipelineLayout, VulkanDescriptorSet* descriptorSet );
public:
	//void resetWaits();
	void addWait( VulkanSemaphore& semaphore, VkPipelineStageFlags stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT );
	void submit( VulkanFence* fence );
	void submit( VulkanSemaphore* signalSemaphoreOnComplete, VulkanFence* fence );
	void execute();
//	void create( VulkanQueueFamily* queueFamily, VulkanDevice* device );
	void destroy();
public:
	VulkanCommandPool* pool(){ assert( _pool ); return _pool; }
	VkCommandBuffer& vkCommandBuffer(){ assert( _vkCommandBuffer ); return _vkCommandBuffer; }
public:
	void copyBuffer( VulkanBuffer& source, VulkanBuffer& target );
	void copyBufferToImage( VulkanBuffer& source, VulkanImage& target, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions );
public:
	void pipelineBarrier(
		VkPipelineStageFlags                        srcStageMask,
		VkPipelineStageFlags                        dstStageMask,
		VkDependencyFlags                           dependencyFlags,
		uint32_t                                    memoryBarrierCount,
		const VkMemoryBarrier*                      pMemoryBarriers,
		uint32_t                                    bufferMemoryBarrierCount,
		const VkBufferMemoryBarrier*                pBufferMemoryBarriers,
		uint32_t                                    imageMemoryBarrierCount,
		const VkImageMemoryBarrier*                 pImageMemoryBarriers
	);
//public:
//	VkDeviceSize size() const { assertIsCreated(); return _size; }
//	const VkBuffer& vkBuffer() const { assertIsCreated(); return _vkBuffer; }
//public:
//	void copyFrom( VulkanCommander& commander, void* data, const size_t& size );
//private:
//	void assertIsCreated() const;
};
