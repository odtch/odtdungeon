#pragma once

#include "vulkan/VulkanCore.h"

class VulkanPipelineRaytracerExtension
{
	DEBUGCOUNTER( VulkanPipelineRaytracerExtension );
private:
public:
	explicit VulkanPipelineRaytracerExtension();
	virtual ~VulkanPipelineRaytracerExtension();
public:
	virtual void create( VulkanDevice* device ) = 0;
	virtual void destroy() = 0;
public:
	virtual void addBindings( VulkanDescriptorSetLayout& descriptorSetLayout ) = 0;
	virtual void bind( VulkanDescriptorSet& descriptorSet ) = 0;
	virtual void updatePipeline( VulkanPipelineLayout* pipelineLayout, VulkanRaytracingPipelineProperties& properties ) = 0;
	virtual void update( VulkanCommandPool& commandpool ) = 0;
	virtual void cmd( VulkanCommandBuffer& commandBuffer, VulkanPipelineLayout& pipelineLayout, VulkanDescriptorSet& descriptorSet ) = 0;
	virtual void cmd2( VulkanCommandBuffer& commandBuffer ) = 0;
};

