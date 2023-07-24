#pragma once

#include "vulkan/VulkanCore.h"

class VulkanSampler
{
	DEBUGCOUNTER( VulkanSampler )
private:
	VulkanDevice* _device;
private:
	VkSampler _vkSampler;
public:
	explicit VulkanSampler();
	~VulkanSampler();
public:
	void create( VulkanDevice* device );
	void destroy();
public:
	VulkanDevice* device() const{ assert( _device ); return _device; }
	VkSampler vkSampler() const{ assert( _vkSampler ); return _vkSampler; }
};
