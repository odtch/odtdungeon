#pragma once

#include "vulkan/VulkanCore.h"
#include "vulkan/device/VulkanDeviceExtension.h"

class VulkanRaytracingPipelineProperties  {
    DEBUGCOUNTER( VulkanRaytracingPipelineProperties );
private:
    VkPhysicalDeviceRayTracingPipelinePropertiesKHR _properties;
private:
    VkDeviceSize _sbtStride;
public:
	explicit VulkanRaytracingPipelineProperties();
	virtual ~VulkanRaytracingPipelineProperties();
public:
    void get( VulkanPhysicalDevice* physicalDevice );
public:
    const VkDeviceSize sbtHeaderSize() const;
    const VkDeviceSize sbtStride() const;
};
