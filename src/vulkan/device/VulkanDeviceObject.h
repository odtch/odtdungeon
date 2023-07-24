#pragma once

#include "vulkan/VulkanCore.h"

class VulkanDeviceObject
{
	DEBUGCOUNTER(VulkanDeviceObject);
	ODTNOCOPY(VulkanDeviceObject);
private:
	VulkanDevice* _device;
public:
	explicit VulkanDeviceObject();
	virtual ~VulkanDeviceObject();
protected:
	bool isBinded() const{ return _device != null; }
	void bindDevice( VulkanDevice* device );
public:	
	virtual void destroy();
public:
	VulkanDevice* device() const;
	VkDevice vkDevice() const;
	VulkanPhysicalDevice* physicalDevice() const;
};
