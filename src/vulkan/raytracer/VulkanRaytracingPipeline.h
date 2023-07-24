#pragma once

#include "vulkan/pipeline/VulkanPipeline.h"
#include "vulkan/resource/VulkanBuffer.h"
#include "vulkan/resource/VulkanShaderModule.h"

class VulkanRaytracingPipeline : public VulkanPipeline
{
private:
    VulkanRaytracingShaderBindingTable* _sbt = null;
    VulkanBuffer _sbtBuffer; // The buffer for the Shader Binding Table
    VkStridedDeviceAddressRegionKHR sbtRayGenRegion, sbtMissRegion, sbtHitRegion, sbtCallableRegion;
private:
    PFN_vkCmdTraceRaysKHR _cmdTraceRaysKHR = null;
public:
	explicit VulkanRaytracingPipeline();
	virtual ~VulkanRaytracingPipeline() override;
public:
	void create( VulkanRaytracingShaderBindingTable* sbt, VulkanPipelineLayout* layout, VulkanRaytracingPipelineProperties& properties );
public:
    void cmdTraceRays( VulkanCommandBuffer& commandBuffer, uint32_t width, uint32_t height, uint32_t depth );
public:
	virtual void destroy() override;
};


