#pragma once

#include "vulkan/VulkanCore.h"

class VulkanImageView
{
private:
	VulkanImage* _image;
	VkImageView _vkImageView;
public:
	explicit VulkanImageView();
	~VulkanImageView();
public:
	void create( VulkanImage* image, VkFormat format, VkImageAspectFlags aspectFlags );
	static void create( VulkanDevice* device, const VkImage& vkImage, VkFormat format, VkImageAspectFlags aspectFlags, VkImageView& vkImageView );
	void destroy();
public:
	VkImageView vkImageView() const{ assert( _vkImageView ); return _vkImageView; }
	VulkanImage* image() const{ assert( _image); return _image; }
};
