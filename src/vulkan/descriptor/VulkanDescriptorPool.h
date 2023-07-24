#pragma once

#include "vulkan/VulkanCore.h"
#include "vulkan/device/VulkanDeviceObject.h"

class VulkanDescriptorPool : public VulkanDeviceObject
{
private:
//	enum { Max_Binding_Count = 10 };
//	int _bindings_count;
//	VkDescriptorPoolBinding _bindings[ Max_Binding_Count ];
private:
	VkDescriptorPool _vkDescriptorPool;
private:
	VulkanDescriptorSetLayout* _setlayout;
public:
	explicit VulkanDescriptorPool();
	~VulkanDescriptorPool();
//public:
//	void addBinding( uint32_t binding, VkDescriptorType type,  uint32_t count, VkShaderStageFlags stageFlags, const VkSampler* pImmutableSampler = nullptr );
public:
//	void create( const VulkanWindowRenderTarget& renderTarget, VulkanDescriptorSetLayout* setlayout );
	void create( uint32_t descriptorCount, uint32_t maxSets, VulkanDescriptorSetLayout* setlayout );
	void destroy();
//public:
//	void waitFor( uint64_t timeout_ns = 1000 * 1000 * 1000 );
//	void reset();
public:
	const VkDescriptorPool& vkDescriptorPool() const{ assert( _vkDescriptorPool ); return _vkDescriptorPool; }
	VulkanDescriptorSetLayout* setlayout() const{ assert( _setlayout ); return _setlayout; }
};

