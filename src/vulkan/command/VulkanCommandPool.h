#pragma once

#include "vulkan/VulkanCore.h"

class VulkanCommandPool
{
private:
	VulkanQueue* _queue;
private:
	VkCommandPool _vkCommandPool;
//	VkDeviceSize _size;
//	VkBuffer _vkBuffer;
//	VkDeviceMemory _vkDeviceMemory;
public:
	explicit VulkanCommandPool();
	~VulkanCommandPool();
public:
	void create( VulkanQueue* queue );
	void destroy();
public:
	VkCommandPool vkCommandPool() const{ assert( _vkCommandPool ); return _vkCommandPool; }
	VulkanQueue* queue() const{ assert( _queue ); return _queue; }
	VulkanDevice* device() const;
//public:
//	VkDeviceSize size() const { assertIsCreated(); return _size; }
//	const VkBuffer& vkBuffer() const { assertIsCreated(); return _vkBuffer; }
//public:
//	void copyFrom( VulkanCommander& commander, void* data, const size_t& size );
//private:
//	void assertIsCreated() const;
};
