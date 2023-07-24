#pragma once

#include "vulkan/VulkanCore.h"
#include "vulkan/device/VulkanDeviceExtension.h"

class VulkanAccelerationStructureExtension : public VulkanDeviceExtension {
public:
	static const char* Name;
private:
	VkPhysicalDeviceAccelerationStructureFeaturesKHR _features;
private:
	PFN_vkCreateAccelerationStructureKHR _createAccelerationStructureKHR;
	PFN_vkGetAccelerationStructureBuildSizesKHR _getAccelerationStructureBuildSizesKHR;
	PFN_vkCmdBuildAccelerationStructuresKHR _cmdBuildAccelerationStructuresKHR;
	//PFN_vkGetAccelerationStructureMemoryRequirementsKHR _getAccelerationStructureMemoryRequirementsKHR;
public:
	explicit VulkanAccelerationStructureExtension();
	virtual ~VulkanAccelerationStructureExtension() override;
public:
	virtual const char* name() const override;
public:
	virtual void installRequirements( VulkanRequirements& requirements ) override;
public:
	virtual bool hasFeature() const override { return true; }
	virtual void* resetFeature() override;
public:
	virtual void checkSupportedFeatures( const VkPhysicalDeviceFeatures& features ) override;
public:
	virtual void bindDevice( VulkanDevice* device ) override;
public:
	PFN_vkCreateAccelerationStructureKHR createAccelerationStructureKHR() const{ assert( _createAccelerationStructureKHR ); return _createAccelerationStructureKHR; }
	PFN_vkGetAccelerationStructureBuildSizesKHR getAccelerationStructureBuildSizesKHR() const{ assert( _getAccelerationStructureBuildSizesKHR ); return _getAccelerationStructureBuildSizesKHR; }
	PFN_vkCmdBuildAccelerationStructuresKHR cmdBuildAccelerationStructuresKHR() const{ assert( _cmdBuildAccelerationStructuresKHR ); return _cmdBuildAccelerationStructuresKHR; }
};
