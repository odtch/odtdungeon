#pragma once

#include "vulkan/VulkanCore.h"

class VulkanSemaphoreData : public SharedPointerData {
	DEBUGCOUNTER(VulkanSemaphoreData);
private:
	VkDevice _vkDevice = null;
	VkSemaphore _vkSemaphore = null;
public:
	friend class VulkanSemaphore;
};

class VulkanSemaphore : private SharedPointer<VulkanSemaphoreData>
{
	DEBUGCOUNTER(VulkanSemaphore);
public:
	explicit VulkanSemaphore();
	explicit VulkanSemaphore( VulkanSemaphore& semaphore );
	virtual ~VulkanSemaphore() override;
public:
	bool isNull() const{ return SharedPointer<VulkanSemaphoreData>::isNull(); }
	void create( VulkanDevice* device );
	//void destroy();
public:
	void operator=( const VulkanSemaphore& semaphore );
public:
	const VkSemaphore& vkSemaphore() const{ assert( !isNull() ); assert( data()->_vkSemaphore ); return data()->_vkSemaphore; }
protected:
	virtual void destroyData( VulkanSemaphoreData* data ) override;
};
