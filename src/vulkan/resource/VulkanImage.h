#pragma once

#include "vulkan/VulkanCore.h"
#include "vulkan/device/VulkanDeviceObject.h"
#include "resource/Texture.h"

class VulkanImage : public VulkanDeviceObject
{
private:
	uint32_t _width;
	uint32_t _height;
	VkImage _vkImage;
	VkDeviceMemory _vkDeviceMemory;
public:
	explicit VulkanImage();
	~VulkanImage();
public:
	void load( VulkanCommandPool& commandPool, Texture& texture );
	void load( VulkanCommandPool& commandPool, Image& image );
	//void loadFile( VulkanCommandPool& commandPool, const String& filename );
	void create( uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VulkanDevice* device );
	void loadBuffer( VulkanCommandPool& commandPool, VulkanBuffer& buffer );
	void getData( std::function<void( void* data )> getter );
	void destroy();
public:
	VkImage vkImage() const{ assert( _vkImage ); return _vkImage; }
public:
	void transiteLayout(
		VkPipelineStageFlags srcStages, VkPipelineStageFlags dstStages,
		VkAccessFlags        srcAccess, VkAccessFlags        dstAccess,
		VkImageLayout        srcLayout, VkImageLayout        dstLayout,
		VulkanCommandPool& commandPool
	);
private:
	void transitionImageLayout( VulkanCommandPool& commandPool, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout );
public:
	void setImageMemoryBarrier( VkImageMemoryBarrier& barrier,
								VkAccessFlags      srcAccess,
								VkAccessFlags      dstAccess,
								VkImageLayout      oldLayout,
								VkImageLayout      newLayout,
								VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT );

};
