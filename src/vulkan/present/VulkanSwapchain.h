#pragma once

#include "vulkan/VulkanCore.h"

struct VulkanSwapchainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class VulkanSwapchain
{
private:
	VulkanSurface* _surface;
	VulkanDevice* _device;
private:
	VkSwapchainKHR _vkSwapchain;
	VkExtent2D _extent;
	VkFormat _swapchainImageFormat;
	std::vector<VkImage> _swapchainImages;
	std::vector<VkImageView> _swapchainImageViews;
public:
	explicit VulkanSwapchain( VulkanSurface* surface, VulkanDevice* device );
	~VulkanSwapchain();
public:
	void create( const VkExtent2D& extent, VkImageUsageFlags imageUsage );
	void destroy();
public:
	size_t size() const;
	const VkExtent2D& extent() const { assert( 0 < _extent.width && 0 < _extent.height ); return _extent; }
	uint32_t width() const{ return extent().width; }
	uint32_t height() const{ return extent().height; }
	const VkFormat& imageFormat() const{ assert( _swapchainImageFormat != VK_FORMAT_UNDEFINED ); return _swapchainImageFormat; };
	const VkImage& vkImage( uint32_t i ) const;
	const VkImageView& vkImageView( uint32_t i ) const;
public:
	VulkanSurface* surface() const{ assert( _surface ); return _surface; }
	VulkanDevice* device() const{ assert( _device ); return _device; }
	VkSwapchainKHR vkSwapchain() const{ assert( _vkSwapchain ); return _vkSwapchain; }
public:
	static VkSurfaceFormatKHR chooseSwapSurfaceFormat( const std::vector<VkSurfaceFormatKHR>& availableFormats );
	static VkPresentModeKHR chooseSwapPresentMode( const std::vector<VkPresentModeKHR>& availablePresentModes );
	VkExtent2D chooseSwapExtent( const VkSurfaceCapabilitiesKHR& capabilities );
};
