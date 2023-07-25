#pragma once

#include "vulkan/VulkanCore.h"
#include "vulkan/resource/VulkanImage.h"
#include "vulkan/resource/VulkanImageView.h"

class VulkanRaytracerTarget
{
private:
	VkExtent2D _size;
	VulkanImage image;
	VulkanImageView imageView;
	bool _imageInitial;
public:
	explicit VulkanRaytracerTarget();
	~VulkanRaytracerTarget();
public:
	void update( const VkExtent2D& size, VulkanCommandPool& commandPool );
	void resize( const VkExtent2D& size, VulkanCommandPool& commandPool );
	void destroy();
public:
    const VkExtent2D& size() const{ return _size; }
public:
	friend class VulkanRaytracer;
};
