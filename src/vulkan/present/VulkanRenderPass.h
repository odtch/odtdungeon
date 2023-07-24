#pragma once

#include "vulkan/VulkanCore.h"
#include "vulkan/device/VulkanDeviceObject.h"

class VulkanRenderPass : public VulkanDeviceObject
{
private:
//	enum { Max_Binding_Count = 10 };
//	int _bindings_count;
//	VkRenderPassBinding _bindings[ Max_Binding_Count ];
private:
	VkRenderPass _vkRenderPass;
public:
	explicit VulkanRenderPass();
	~VulkanRenderPass();
//public:
//	void addBinding( uint32_t binding, VkDescriptorType type,  uint32_t count, VkShaderStageFlags stageFlags, const VkSampler* pImmutableSampler = nullptr );
//	int bindingCount() const{ return _bindings_count; }
//	const VkDescriptorType& descriptorType( int i ) const;
public:
	void create( VkFormat imageFormat, VkFormat depthFormat, VulkanDevice* device );
	void destroy();
//public:
//	void waitFor( uint64_t timeout_ns = 1000 * 1000 * 1000 );
//	void reset();
public:
	const VkRenderPass& vkRenderPass() const{ assert( _vkRenderPass ); return _vkRenderPass; }
};

