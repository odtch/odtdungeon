#pragma once

#include "vulkan/VulkanCore.h"

class VulkanFenceData : public SharedPointerData {
	DEBUGCOUNTER(VulkanFenceData);
private:
	VkDevice _vkDevice = null;
	VkFence _vkFence = null;
	String _debugname;
public:
	friend class VulkanFence;
};

class VulkanFence : public SharedPointer<VulkanFenceData>
{
	DEBUGCOUNTER(VulkanFence);
public:
	explicit VulkanFence();
	explicit VulkanFence( VulkanFence& data );
	virtual ~VulkanFence() override;
public:
	void create( const String& debugName, VkFenceCreateFlags flags, VulkanDevice* device );
public:
	bool isSignaled();
	void waitFor( uint64_t timeout_ns = 1000 * 1000 * 1000 );
	bool tryWaitFor( uint64_t timeout_ns = 1000 * 1000 * 1000 );
	void reset();
public:
	VkFence vkFence() const{ assert( !isNull() ); assert( data()->_vkFence ); return data()->_vkFence; }
protected:
	virtual void destroyData( VulkanFenceData* data ) override;
};
