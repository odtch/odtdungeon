#pragma once

#include "vulkan/VulkanCore.h"

class VulkanBufferArray
{
private:
//	VulkanDevice* _device;
//private:
//	VkDeviceSize _size;
//	VkBuffer _vkBuffer;
//	VkDeviceMemory _vkDeviceMemory;
	uint32_t _maxSize;
	uint32_t _size;
	Array<VkDescriptorBufferInfo> _bufferinfos;
public:
	explicit VulkanBufferArray( uint32_t maxSize );
	~VulkanBufferArray();
public:
	uint32_t size() const{ return _size; }
	void reset();
	void add( VulkanBuffer* buffer );
	void set( uint32_t i, VulkanBuffer* buffer );
public:
	const VkDescriptorBufferInfo* descriptorBufferInfos() const{ assert( 0 < size() ); return &_bufferinfos.first(); }
//	bool isCreated() const{ return _vkBuffer != null; }
//	void create( VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VulkanDevice* device );
//	void createStaging( const void* data, const size_t& size, VulkanDevice* device );
//	void loadStaging( const void* data, const size_t& size );
//	void destroy();
//public:
//	VkDeviceSize size() const { assertIsCreated(); return _size; }
//	VulkanDevice* device() const{ assert( _device ); return _device; }
//	const VkBuffer& vkBuffer() const { assertIsCreated(); return _vkBuffer; }
//public:
//	void load( VulkanCommandPool& commandPool, const void* data, const size_t& size );
//private:
//	void assertIsCreated() const;
//public:
//	VkDeviceAddress getBufferDeviceAddress();
};
