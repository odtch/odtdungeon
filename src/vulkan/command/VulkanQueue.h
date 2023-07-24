#pragma once

#include "vulkan/VulkanCore.h"

class VulkanQueue
{
private:
	VulkanDevice* _device;
private:
	VkQueue _vkQueue;
private:
	VulkanQueueFamily* _family;
private:
	List<VulkanTask*> _pendingtasks;
//private:
//	bool _threaded = false;
//	std::timed_mutex _mutex;
public:
	explicit VulkanQueue( VkQueue vkQueue, VulkanQueueFamily* family, VulkanDevice* device );
	virtual ~VulkanQueue();
public:
//	virtual bool isPhysicalDeviceSupported( VulkanPhysicalDevice& physicalDevice ) const;
//	virtual VulkanQueueFamily* findQueueFamilyInPhysicalDevice( VulkanPhysicalDevice& physicalDevice ) const = 0;
public:
	// const VkQueue& vkQueue, VulkanQueueFamily* family,
	VkQueue vkQueue() const{ assert( _vkQueue ); return _vkQueue; }
public:
	const VulkanQueueFamily* family() const{ return _family; }
	VulkanDevice* device() const{ return _device; }
public:
	void submit( uint32_t count, const VkSubmitInfo* submits, VulkanFence* fence );
	void submit( VulkanCommandBuffer& commandBuffer, VulkanFence* fence );
	void execute( VulkanCommandBuffer& commandBuffer );
	void waitIdle();
public:
	void onTaskCreated( VulkanTask& task );
public:
	void onIdle();
public:
	friend class VulkanBackgroundThread;
};
