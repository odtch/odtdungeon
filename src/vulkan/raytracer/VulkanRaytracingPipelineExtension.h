#pragma once

#include "vulkan/VulkanCore.h"
#include "vulkan/device/VulkanDeviceExtension.h"

class VulkanRaytracingPipelineExtension : public VulkanDeviceExtension {
public:
	static const char* Name;
private:
	VkPhysicalDeviceRayTracingPipelineFeaturesKHR _features;
public:
	explicit VulkanRaytracingPipelineExtension();
	virtual ~VulkanRaytracingPipelineExtension() override;
public:
	virtual const char* name() const override;
public:
	virtual void installRequirements( VulkanRequirements& requirements ) override;
public:
	virtual bool hasFeature() const override { return true; }
	virtual void* resetFeature() override;
public:
	virtual void checkSupportedFeatures( const VkPhysicalDeviceFeatures& features ) override;
};
