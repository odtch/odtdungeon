#include "stdodt.h"
#include "VulkanRaytracerTarget.h"
//#include "vulkan/OdtVulkan.h"
#include "vulkan/command/VulkanCommandPool.h"

VulkanRaytracerTarget::VulkanRaytracerTarget(){
	_size.width = 0;
	_size.height = 0;
}
VulkanRaytracerTarget::~VulkanRaytracerTarget(){
}
void VulkanRaytracerTarget::update( const VkExtent2D& size, VulkanCommandPool& commandPool ){
	if( _size.width == size.width && _size.height == size.height ){
		return;
	}
	resize( size, commandPool );
}
void VulkanRaytracerTarget::resize( const VkExtent2D& size, VulkanCommandPool& commandPool ){
	_size = size;
	destroy();
	image.create( _size.width, _size.height, VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, commandPool.device() );
	image.transiteLayout(
		VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
		0, VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT,
		VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL,
		commandPool
	);
	imageView.create( &image, VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_ASPECT_COLOR_BIT );
	_imageInitial = true;
}
void VulkanRaytracerTarget::destroy(){
	imageView.destroy();
	image.destroy();
}
