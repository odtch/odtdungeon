#pragma once

#include "vulkan/VulkanCore.h"
//#include "VulkanInstance.h"
//#include "VulkanDevice.h"

class VulkanDeviceExtension
{
    DEBUGCOUNTER( VulkanDeviceExtension );
public:
	explicit VulkanDeviceExtension();
	virtual ~VulkanDeviceExtension();
public:
	virtual const char* name() const = 0;
public:
	virtual void installRequirements( VulkanRequirements& requirements );
public:
	virtual bool hasFeature() const{ return false; }
	virtual void* resetFeature(){ assert( false ); return null; }
public:
	virtual void checkSupportedFeatures( const VkPhysicalDeviceFeatures& features );
public:
	virtual void bindDevice( VulkanDevice* device );
};

