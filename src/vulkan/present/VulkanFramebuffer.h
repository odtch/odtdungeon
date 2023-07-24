#pragma once

#include "vulkan/device/VulkanDeviceObject.h"

class VulkanFramebuffer : VulkanDeviceObject
{
private:
	VkFramebuffer _vkFramebuffer;
public:
	explicit VulkanFramebuffer();
	~VulkanFramebuffer();
public:
	void create( VulkanRenderPass* renderPass, const VkExtent2D& extent, const std::vector<VkImageView>& attachments );
	virtual void destroy() override;
public:
	VkFramebuffer vkFramebuffer() const{ assert( _vkFramebuffer ); return _vkFramebuffer; }
};
