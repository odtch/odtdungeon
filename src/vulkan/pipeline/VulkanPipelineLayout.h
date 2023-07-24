#pragma once

#include "vulkan/device/VulkanDeviceObject.h"

class VulkanPipelineLayout : public VulkanDeviceObject
{
private:
	enum { Max_Set_Count = 10 };
	int _set_count;
	VkDescriptorSetLayout _sets[ Max_Set_Count ];
private:
	uint32_t _push_constant_count;
	VkPushConstantRange _push_constant_range;
private:
	VkPipelineLayout _vkPipelineLayout;
public:
	explicit VulkanPipelineLayout();
	~VulkanPipelineLayout();
public:
	void addSet( VulkanDescriptorSetLayout* set );
	void addPushConstant( VkShaderStageFlags flags, size_t size );
public:
	void create( VulkanDevice* device );
	bool isCreated() const{ return _vkPipelineLayout != nullptr; }
	void destroy();
public:
	const VkPipelineLayout& vkPipelineLayout() const{ assert( _vkPipelineLayout ); return _vkPipelineLayout; }
};

