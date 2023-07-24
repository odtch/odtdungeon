#include "stdodt.h"

#include "VulkanImage.h"
#include "vulkan/device/VulkanDevice.h"
#include "vulkan/device/VulkanPhysicalDevice.h"
#include "VulkanBuffer.h"
#include "vulkan/command/VulkanCommandPool.h"
#include "vulkan/command/VulkanCommandBuffer.h"
//#include "resource/Texture.h"

VulkanImage::VulkanImage()
	:_vkImage( nullptr )
	,_vkDeviceMemory( nullptr )
{
}
VulkanImage::~VulkanImage(){
	ASSERT( _vkImage == nullptr && _vkDeviceMemory == nullptr );
}
void VulkanImage::load( VulkanCommandPool& commandPool, Texture& texture ){
	load( commandPool, *texture.image() );
}
void VulkanImage::load( VulkanCommandPool& commandPool, Image& image ){
	VkDeviceSize imageSize = image.width() * image.height() * 4;
	VulkanBuffer stagingBuffer;
	stagingBuffer.createStaging( image.pixels(), imageSize, commandPool.device() );
	create( image.width(), image.height(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, commandPool.device() );
	transitionImageLayout( commandPool, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL );
	loadBuffer( commandPool, stagingBuffer );
	transitionImageLayout( commandPool, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

//	} else if( texture.isFile() ){
//		loadFile( commandPool, texture.filename() );
//	} else {
//		assert( false );
//	}
}
//void VulkanImage::loadFile( VulkanCommandPool& commandPool, const String& filename ){
//	if( !File::Exists( filename ) ){
//		logError( "VulkanImage::loadFile", filename, "not exists" );
//	}
//	int todo_same_in_texture_or_image;
//	BinaryFileReader reader( filename );
//	uint texWidth = reader.read_uint32();
//	uint texHeight = reader.read_uint32();
//	uint pixel_size = reader.read_uint32();
//	assert( texWidth * texHeight * 4 == pixel_size );
//	char* pixels = new char[ pixel_size ];
//	reader.read_block( pixels, pixel_size );
//	VkDeviceSize imageSize = pixel_size;
//	VulkanBuffer stagingBuffer;
//	stagingBuffer.createStaging( pixels, imageSize, commandPool.device() );
//	delete[] pixels;
//	create( texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, commandPool.device() );
//	transitionImageLayout( commandPool, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL );
//	loadBuffer( commandPool, stagingBuffer );
//	transitionImageLayout( commandPool, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
//}
void VulkanImage::create( uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VulkanDevice* device ){
	assert( 0 < width && 0 < height );
	assert( device );
	bindDevice( device );
	if( _vkImage != nullptr )
		throw std::runtime_error( "VulkanImage already created" );
	_width = width;
	_height = height;
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	Vulkan::check( vkCreateImage( vkDevice(), &imageInfo, nullptr, &_vkImage ), "vkCreateImage failed" );
	assert( _vkImage );
	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements( vkDevice(), _vkImage, &memRequirements );
	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = physicalDevice()->getMemoryTypeIndex( memRequirements.memoryTypeBits, properties );
	Vulkan::check( vkAllocateMemory( vkDevice(), &allocInfo, nullptr, &_vkDeviceMemory ), "vkAllocateMemory failed" );
	assert( _vkDeviceMemory );
	vkBindImageMemory( vkDevice(), _vkImage, _vkDeviceMemory, 0 );
}
void VulkanImage::loadBuffer( VulkanCommandPool& commandPool, VulkanBuffer& buffer ){
	VulkanCommandBuffer commandBuffer;
	commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, &commandPool );
	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = {0, 0, 0};
	region.imageExtent = { _width, _height, 1 };
	commandBuffer.copyBufferToImage( buffer, *this, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region );
	commandBuffer.end();
	commandBuffer.execute();
	commandBuffer.destroy();
}
void VulkanImage::getData( std::function<void (void *)> getter ){
	void* data = null;
	Vulkan::check( vkMapMemory( device()->vkDevice(), _vkDeviceMemory, 0, VK_WHOLE_SIZE, 0, &data ), "vkMapMemory failed" );
	assert( data );
	getter( data );
	vkUnmapMemory( device()->vkDevice(), _vkDeviceMemory );
}
void VulkanImage::destroy(){
	if( _vkImage ){
		vkDestroyImage( vkDevice(), _vkImage, nullptr );
		_vkImage = nullptr;
	}
	if( _vkDeviceMemory ){
		vkFreeMemory( vkDevice(), _vkDeviceMemory, nullptr );
		_vkDeviceMemory = nullptr;
	}
	VulkanDeviceObject::destroy();
}
void VulkanImage::transiteLayout( VkPipelineStageFlags srcStages, VkPipelineStageFlags dstStages, VkAccessFlags srcAccess, VkAccessFlags dstAccess, VkImageLayout srcLayout, VkImageLayout dstLayout, VulkanCommandPool &commandPool ){
	VulkanCommandBuffer commandBuffer;
	commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, &commandPool );
	VkImageMemoryBarrier imageBarrier;
	setImageMemoryBarrier( imageBarrier, srcAccess,  dstAccess, srcLayout, dstLayout );
	vkCmdPipelineBarrier( commandBuffer.vkCommandBuffer(),
					   srcStages, dstStages,  // Src and dst pipeline stages
					   0,                     // Flags for memory dependencies
					   0, nullptr,            // Global memory barrier objects
					   0, nullptr,            // Buffer memory barrier objects
					   1, &imageBarrier );     // Image barrier objects
	commandBuffer.end();
	commandBuffer.execute();
	commandBuffer.destroy();

}
void VulkanImage::transitionImageLayout( VulkanCommandPool& commandPool, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout ){
	VulkanCommandBuffer commandBuffer;
	commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, &commandPool );
	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = vkImage();
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;
	if( oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL ){
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	} else if( oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
			   && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ){
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	} else {
		throw std::invalid_argument("unsupported layout transition!");
	}
	commandBuffer.pipelineBarrier(
		sourceStage, destinationStage,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);
	commandBuffer.end();
	commandBuffer.execute();
	commandBuffer.destroy();
}
void VulkanImage::setImageMemoryBarrier( VkImageMemoryBarrier& barrier, VkAccessFlags srcAccess, VkAccessFlags dstAccess, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspectMask ){
	memset( &barrier, 0, sizeof( barrier ) );
	barrier.sType = {VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
	barrier.srcAccessMask = srcAccess;
	barrier.dstAccessMask = dstAccess;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = vkImage();
	barrier.subresourceRange.aspectMask = aspectMask;
	barrier.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
	barrier.subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;
}


