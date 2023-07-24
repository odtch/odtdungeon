#pragma once

#include "vulkan/VulkanCore.h"
#include "vulkan/command/VulkanCommand.h"

class VulkanBufferData : public SharedPointerData {
	DEBUGCOUNTER(VulkanBufferData);
private:
	VulkanDevice* _device = null;
private:
	VkDeviceSize _size = 0;
	VkBuffer _vkBuffer = null;
	VkDeviceMemory _vkDeviceMemory = null;
private:
	bool _uploading = false;
public:
	friend class VulkanBuffer;
	friend class VulkanLoadBufferCommand;
};

class VulkanBuffer : private SharedPointer<VulkanBufferData>
{
	DEBUGCOUNTER(VulkanBuffer);
	ODTNOCOPY(VulkanBuffer);
public:
	explicit VulkanBuffer();
	explicit VulkanBuffer( VulkanBuffer& buffer );
	virtual ~VulkanBuffer() override;
public:
	bool isNull() const{ return SharedPointer<VulkanBufferData>::isNull(); }
	bool isCreated() const{ if( isNull() )return false; return data()->_vkBuffer != null; }
	void create( VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VulkanDevice* device );
	void createStaging( const void* data, const size_t& size, VulkanDevice* device );
	void loadStaging( const void* data, const size_t& size );
	void clear();
public:
	VkDeviceSize size() const { assertIsCreated(); return data()->_size; }
	VulkanDevice* device() const{ assert( data()->_device ); return data()->_device; }
	const VkBuffer& vkBuffer() const { assertIsCreated(); return data()->_vkBuffer; }
	const VkDeviceMemory& vkDeviceMemory() const { assertIsCreated(); return data()->_vkDeviceMemory; }
public:
	void load( VulkanCommandPool& commandPool, const void* data, const size_t& size );
private:
	void assertIsCreated() const;
public:
	VkDeviceAddress getBufferDeviceAddress();
protected:
	virtual void destroyData( VulkanBufferData* data ) override;
public:
	friend class VulkanLoadBufferCommand;
};

class VulkanLoadBufferCommand : public VulkanCommand
{
private:
	VulkanBuffer _buffer;
	VulkanBuffer _staging;
public:
	explicit VulkanLoadBufferCommand( VulkanBuffer& buffer, const void* data, const size_t& size );
	virtual ~VulkanLoadBufferCommand() override;
public:
	virtual void bind( VulkanCommandBuffer& commandBuffer ) override;
	virtual void onCompleted() override;
};
