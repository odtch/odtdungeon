#pragma once

#include "vulkan/pipeline/VulkanPipeline.h"

class VulkanComputePipeline : public VulkanPipeline
{
private:
public:
	explicit VulkanComputePipeline();
	virtual ~VulkanComputePipeline() override;
public:
	void create( VulkanShaderModule* shaderModule, VulkanPipelineLayout* layout );
};


