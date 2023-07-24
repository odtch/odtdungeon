#include "stdodt.h"

#include "VulkanImageView.h"
#include "VulkanImage.h"
#include "vulkan/device/VulkanDevice.h"
#include "vulkan/device/VulkanPhysicalDevice.h"

VulkanImageView::VulkanImageView()
	:_image( nullptr )
	,_vkImageView( nullptr )
{
}
VulkanImageView::~VulkanImageView(){
	ASSERT( _vkImageView == nullptr && _image == nullptr );
	destroy();
}
void VulkanImageView::create( VulkanImage* image, VkFormat format, VkImageAspectFlags aspectFlags ){
	assert( image );
	if( _image != nullptr )
		throw std::runtime_error( "VulkanImageView already created" );
	_image = image;
	assert( _vkImageView == nullptr );
	create( _image->device(), _image->vkImage(), format, aspectFlags, _vkImageView );
}
void VulkanImageView::create( VulkanDevice* device, const VkImage& vkImage, VkFormat format, VkImageAspectFlags aspectFlags, VkImageView& vkImageView ){
	assert( device );
	assert( vkImage );
	assert( vkImageView == nullptr );
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = vkImage;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspectFlags;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;
	Vulkan::check( vkCreateImageView( device->vkDevice(), &viewInfo, nullptr, &vkImageView ), "vkCreateImageView failed" );
}
void VulkanImageView::destroy(){
	if( _vkImageView ){
		vkDestroyImageView( image()->device()->vkDevice(), _vkImageView, nullptr );
		_vkImageView = nullptr;
	}
	_image = nullptr;
}
