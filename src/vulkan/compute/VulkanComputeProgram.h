#pragma once

#include "vulkan/VulkanCore.h"
#include "vulkan/device/VulkanDeviceObject.h"
#include "vulkan/descriptor/VulkanDescriptorSetLayout.h"
#include "vulkan/descriptor/VulkanDescriptorPool.h"
#include "vulkan/pipeline/VulkanPipelineLayout.h"
#include "vulkan/resource/VulkanShaderModule.h"
#include "vulkan/command/VulkanCommandPool.h"

class VulkanComputeProgram : public VulkanDeviceObject
{
private:
	VulkanDescriptorSetLayout _descriptorSetLayout;
	VulkanDescriptorPool _descriptorPool;
	VulkanPipelineLayout _pipelineLayout;
	VulkanShaderModule _shaderModule;
	VulkanCommandPool _commandPool;
public:
	explicit VulkanComputeProgram();
	virtual ~VulkanComputeProgram() override;
public:
	void create( const String& shaderFilename, uint32_t maxExecutorCount, VulkanDevice* device );
	virtual void destroy() override;
protected:
	virtual void addBindings( VulkanDescriptorSetLayout& descriptorSetLayout ) = 0;
public:
	VulkanDescriptorPool& descriptorPool(){ return _descriptorPool; }
	VulkanCommandPool& commandPool(){ return _commandPool; }
	VulkanShaderModule& shaderModule(){ return _shaderModule; }
	VulkanPipelineLayout& pipelineLayout(){ return _pipelineLayout; }
};
