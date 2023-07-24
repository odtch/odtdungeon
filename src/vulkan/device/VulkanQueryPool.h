#pragma once

#include "vulkan/VulkanCore.h"

class VulkanQueryPool
{
	ODTNOCOPY(VulkanQueryPool);
	DEBUGCOUNTER(VulkanQueryPool);
private:
	VulkanDevice* _device = null;
	VkQueryPool _vkQueryPool = null;
	uint _count = 0;
private:
	float _duration_to_ms = 0;
	float _duration_to_s = 0;
//	VulkanQueue* _queue;
//private:
//	VkCommandPool _vkCommandPool;
//	VkDeviceSize _size;
//	VkBuffer _vkBuffer;
//	VkDeviceMemory _vkDeviceMemory;
public:
	explicit VulkanQueryPool();
	~VulkanQueryPool();
public:
	void create( uint count, VulkanDevice* device );
	void reset();
	void destroy();
public:
	VkQueryPool vkQueryPool() const{ assert( _vkQueryPool ); return _vkQueryPool; };
public:
	float duration_to_ms( uint64_t duration ) const{ return duration * _duration_to_ms; }
//	VkCommandPool vkCommandPool() const{ assert( _vkCommandPool ); return _vkCommandPool; }
//	VulkanQueue* queue() const{ assert( _queue ); return _queue; }
//	VulkanDevice* device() const;
//public:
//	VkDeviceSize size() const { assertIsCreated(); return _size; }
//	const VkBuffer& vkBuffer() const { assertIsCreated(); return _vkBuffer; }
//public:
//	void copyFrom( VulkanCommander& commander, void* data, const size_t& size );
//private:
//	void assertIsCreated() const;
};
