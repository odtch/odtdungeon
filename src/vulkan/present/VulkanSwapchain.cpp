#include "stdodt.h"

#include "VulkanSwapchain.h"
#include "vulkan/device/VulkanDevice.h"
#include "vulkan/device/VulkanPhysicalDevice.h"
#include "vulkan/present/VulkanSurface.h"
#include "vulkan/resource/VulkanBuffer.h"
#include "vulkan/resource/VulkanImageView.h"
#include "vulkan/command/VulkanCommandPool.h"
#include "vulkan/command/VulkanCommandBuffer.h"

VulkanSwapchain::VulkanSwapchain( VulkanSurface* surface, VulkanDevice* device )
	:_surface( surface )
	,_device( device )
	,_vkSwapchain( nullptr )
	,_swapchainImageFormat( VK_FORMAT_UNDEFINED )
{
	//std::cout << "VulkanSwapchain()\n";
	assert( _surface );
	assert( _device );
	_extent.width = 0;
	_extent.height = 0;
}
VulkanSwapchain::~VulkanSwapchain(){
	ASSERT( _vkSwapchain == nullptr );
	destroy();
	_device = nullptr;
	_surface = nullptr;
}
void VulkanSwapchain::create( const VkExtent2D& extent, VkImageUsageFlags imageUsage ){
	assert( 0 < extent.width && 0 < extent.height );
	if( _vkSwapchain != nullptr )
		throw std::runtime_error( "VulkanSwapchain already created" );
	assert( _vkSwapchain == nullptr );
	VulkanSwapchainSupportDetails swapChainSupport;
	_device->getSwapchainSupport( *_surface, swapChainSupport );
	VkSurfaceFormatKHR surfaceFormat = VulkanSwapchain::chooseSwapSurfaceFormat( swapChainSupport.formats );
	VkPresentModeKHR presentMode = chooseSwapPresentMode( swapChainSupport.presentModes );
	//	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);
	//	assert( 0 < extent.width && 0 < extent.height );
	_swapchainImageFormat = surfaceFormat.format;
	assert( _swapchainImageFormat != VK_FORMAT_UNDEFINED );
	_extent = extent;

	uint32_t graphicsQueueFamilyIndex = _device->physicalDevice()->getGraphicsQueueFamily()->index();
	uint32_t presentQueueFamilyIndex = _device->physicalDevice()->getPresentQueueFamily( *_surface )->index();
	uint32_t queueFamilyIndices[] = { graphicsQueueFamilyIndex, presentQueueFamilyIndex };


	{
		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
				imageCount = swapChainSupport.capabilities.maxImageCount;
		}
		VkSwapchainCreateInfoKHR createInfo;
		memset( &createInfo, 0, sizeof( createInfo ) );
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = _surface->vkSurface();
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = imageUsage | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;;
		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		if( graphicsQueueFamilyIndex != presentQueueFamilyIndex ) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		} else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}
		assert( _vkSwapchain == nullptr );
		//logDebug( "VulkanSwapchain create", createInfo.imageExtent.width, createInfo.imageExtent.height );
		Vulkan::check( vkCreateSwapchainKHR( _device->vkDevice(), &createInfo, nullptr, &_vkSwapchain ), "vkCreateSwapchainKHR failed" );
		assert( _vkSwapchain );
	}
	uint32_t imageCount = 0;
	Vulkan::check( vkGetSwapchainImagesKHR( _device->vkDevice(), _vkSwapchain, &imageCount, nullptr ), "vkGetSwapchainImagesKHR count failed" );
	assert( 0 < imageCount );
	_swapchainImages.resize( imageCount );
	Vulkan::check( vkGetSwapchainImagesKHR( _device->vkDevice(), _vkSwapchain, &imageCount, _swapchainImages.data() ), "vkGetSwapchainImagesKHR get failed" );
	_swapchainImageViews.resize( imageCount );
	for( uint32_t i = 0; i < _swapchainImageViews.size(); i++ ){
		VulkanImageView::create( _device, _swapchainImages[i], _swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, _swapchainImageViews[i] );
	}
}
void VulkanSwapchain::destroy(){
	for( uint32_t i = 0; i < _swapchainImageViews.size(); i++ ){
		vkDestroyImageView( device()->vkDevice(), _swapchainImageViews[i], nullptr );
	}
	_swapchainImageViews.clear();
	if( _vkSwapchain ){
		vkDestroySwapchainKHR( device()->vkDevice(), _vkSwapchain, nullptr );
		_vkSwapchain = nullptr;
	}
}
size_t VulkanSwapchain::size() const {
	assert( _vkSwapchain );
	return _swapchainImages.size();
}
const VkImage& VulkanSwapchain::vkImage( uint32_t i ) const {
	assert( i < _swapchainImages.size() );
	return _swapchainImages[ i ];
}
const VkImageView& VulkanSwapchain::vkImageView( uint32_t i ) const {
	assert( i < _swapchainImageViews.size() );
	return _swapchainImageViews[ i ];
}
VkSurfaceFormatKHR VulkanSwapchain::chooseSwapSurfaceFormat( const std::vector<VkSurfaceFormatKHR>& availableFormats ){
	for( const auto& availableFormat : availableFormats ){
		if( availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace ==
				//VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT
				VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
				){
			return availableFormat;
		}
	}
	return availableFormats[0];
}
VkPresentModeKHR VulkanSwapchain::chooseSwapPresentMode( const std::vector<VkPresentModeKHR>& availablePresentModes ){
	for( const auto& availablePresentMode : availablePresentModes ){
		if( availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR ){
			return availablePresentMode;
		}
	}
	return VK_PRESENT_MODE_FIFO_KHR;
}
