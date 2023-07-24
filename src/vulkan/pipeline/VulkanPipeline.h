#pragma once

#include "vulkan/device/VulkanDeviceObject.h"

class VulkanPipeline : public VulkanDeviceObject
{
private:
	VkPipeline _vkPipeline;
public:
	explicit VulkanPipeline();
	virtual ~VulkanPipeline() override;
protected:
	void bindPipeline( VkPipeline vkPipeline );
public:
	virtual void destroy() override;
public:
	bool isCreated() const{ return _vkPipeline != nullptr; }
	const VkPipeline& vkPipeline() const{ assert( _vkPipeline ); return _vkPipeline; }
};


class VulkanGraphicsPipeline : public VulkanPipeline
{
private:
	enum { Max_ShaderStage_Count = 10 };
	int _shaderstage_count;
	VkPipelineShaderStageCreateInfo _shaderstages[ Max_ShaderStage_Count ];
private:
public:
	explicit VulkanGraphicsPipeline();
	virtual ~VulkanGraphicsPipeline() override;
public:
	void addShaderStage( VkShaderStageFlagBits stage, VulkanShaderModule* shader );
public:
	void create( const VkVertexInputBindingDescription& vertexInputBindingDescription,
				 const VkVertexInputAttributeDescription* vertexInputAttributeDescription,
				 int vertexInputAttributeCount,
				 VkPrimitiveTopology topology,
				 VulkanRenderPass& renderPass,
				 const VkExtent2D& renderTargetSize,
				 VulkanPipelineLayout* layout );
	virtual void destroy() override;
};

