#include "stdodt.h"

#include "VulkanShaderModule.h"
#include "vulkan/device/VulkanDevice.h"
//#include "VulkanPhysicalDevice.h"
//#include "VulkanBuffer.h"
//#include "VulkanCommandPool.h"
//#include "VulkanCommandBuffer.h"

VulkanShaderModule::VulkanShaderModule()
	:_device( nullptr )
	,_vkShaderModule( nullptr )
//	,_vkDeviceMemory( nullptr )
{
}
VulkanShaderModule::~VulkanShaderModule(){
	ASSERT( _vkShaderModule == nullptr && _device == nullptr );
	destroy();
}
//void VulkanShaderModule::loadFile( VulkanCommandPool& commandPool, const std::string& filename ){
//	int texWidth, texHeight, texChannels;
//	stbi_uc* pixels = stbi_load( filename.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
//	VkDeviceSize ShaderModuleSize = texWidth * texHeight * 4;
//	if (!pixels) {
//		std::cout << filename.c_str();
//		throw std::runtime_error( "failed to load ShaderModule");
//	}
//	VulkanBuffer stagingBuffer;
//	stagingBuffer.createStaging( pixels, ShaderModuleSize, commandPool.device() );
//	stbi_ShaderModule_free(pixels);
//	create( texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_ShaderModule_TILING_OPTIMAL, VK_ShaderModule_USAGE_TRANSFER_DST_BIT | VK_ShaderModule_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, commandPool.device() );
//	transitionShaderModuleLayout( commandPool, VK_FORMAT_R8G8B8A8_SRGB, VK_ShaderModule_LAYOUT_UNDEFINED, VK_ShaderModule_LAYOUT_TRANSFER_DST_OPTIMAL );
//	loadBuffer( commandPool, stagingBuffer );
//	transitionShaderModuleLayout( commandPool, VK_FORMAT_R8G8B8A8_SRGB, VK_ShaderModule_LAYOUT_TRANSFER_DST_OPTIMAL, VK_ShaderModule_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
//	stagingBuffer.destroy();
//}
//void VulkanShaderModule::create( uint32_t width, uint32_t height, VkFormat format, VkShaderModuleTiling tiling, VkShaderModuleUsageFlags usage, VkMemoryPropertyFlags properties, VulkanDevice* device ){
//	assert( 0 < width && 0 < height );
//	assert( device );
//	if( _vkShaderModule != nullptr || _device != nullptr )
//		throw std::runtime_error( "VulkanShaderModule already created" );
//	_device = device;
//	_width = width;
//	_height = height;
//	VkShaderModuleCreateInfo ShaderModuleInfo{};
//	ShaderModuleInfo.sType = VK_STRUCTURE_TYPE_ShaderModule_CREATE_INFO;
//	ShaderModuleInfo.ShaderModuleType = VK_ShaderModule_TYPE_2D;
//	ShaderModuleInfo.extent.width = width;
//	ShaderModuleInfo.extent.height = height;
//	ShaderModuleInfo.extent.depth = 1;
//	ShaderModuleInfo.mipLevels = 1;
//	ShaderModuleInfo.arrayLayers = 1;
//	ShaderModuleInfo.format = format;
//	ShaderModuleInfo.tiling = tiling;
//	ShaderModuleInfo.initialLayout = VK_ShaderModule_LAYOUT_UNDEFINED;
//	ShaderModuleInfo.usage = usage;
//	ShaderModuleInfo.samples = VK_SAMPLE_COUNT_1_BIT;
//	ShaderModuleInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
//	Vulkan::check( vkCreateShaderModule( _device->vkDevice(), &ShaderModuleInfo, nullptr, &_vkShaderModule ), "vkCreateShaderModule failed" );
//	assert( _vkShaderModule );
//	VkMemoryRequirements memRequirements;
//	vkGetShaderModuleMemoryRequirements( _device->vkDevice(), _vkShaderModule, &memRequirements);
//	VkMemoryAllocateInfo allocInfo{};
//	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//	allocInfo.allocationSize = memRequirements.size;
//	allocInfo.memoryTypeIndex = _device->physicalDevice()->getMemoryTypeIndex( memRequirements.memoryTypeBits, properties );
//	Vulkan::check( vkAllocateMemory( _device->vkDevice(), &allocInfo, nullptr, &_vkDeviceMemory ), "vkAllocateMemory failed" );
//	assert( _vkDeviceMemory );
//	vkBindShaderModuleMemory( _device->vkDevice(), _vkShaderModule, _vkDeviceMemory, 0 );
//}
//void VulkanShaderModule::loadBuffer( VulkanCommandPool& commandPool, VulkanBuffer& buffer ){
//	VulkanCommandBuffer commandBuffer;
//	commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, &commandPool );
//	VkBufferShaderModuleCopy region{};
//	region.bufferOffset = 0;
//	region.bufferRowLength = 0;
//	region.bufferShaderModuleHeight = 0;
//	region.ShaderModuleSubresource.aspectMask = VK_ShaderModule_ASPECT_COLOR_BIT;
//	region.ShaderModuleSubresource.mipLevel = 0;
//	region.ShaderModuleSubresource.baseArrayLayer = 0;
//	region.ShaderModuleSubresource.layerCount = 1;
//	region.ShaderModuleOffset = {0, 0, 0};
//	region.ShaderModuleExtent = { _width, _height, 1 };
//	commandBuffer.copyBufferToShaderModule( buffer, *this, VK_ShaderModule_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region );
//	commandBuffer.end();
//	commandBuffer.execute();
//	commandBuffer.destroy();
//}
//void VulkanShaderModule::destroy(){
//	if( _vkShaderModule ){
//		vkDestroyShaderModule( device()->vkDevice(), _vkShaderModule, nullptr );
//		_vkShaderModule = nullptr;
//	}
//	if( _vkDeviceMemory ){
//		vkFreeMemory( device()->vkDevice(), _vkDeviceMemory, nullptr );
//		_vkDeviceMemory = nullptr;
//	}
//	_device = nullptr;
//}
//void VulkanShaderModule::transitionShaderModuleLayout( VulkanCommandPool& commandPool, VkFormat format, VkShaderModuleLayout oldLayout, VkShaderModuleLayout newLayout ){
//	VulkanCommandBuffer commandBuffer;
//	commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, &commandPool );
//	VkShaderModuleMemoryBarrier barrier{};
//	barrier.sType = VK_STRUCTURE_TYPE_ShaderModule_MEMORY_BARRIER;
//	barrier.oldLayout = oldLayout;
//	barrier.newLayout = newLayout;
//	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
//	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
//	barrier.ShaderModule = vkShaderModule();
//	barrier.subresourceRange.aspectMask = VK_ShaderModule_ASPECT_COLOR_BIT;
//	barrier.subresourceRange.baseMipLevel = 0;
//	barrier.subresourceRange.levelCount = 1;
//	barrier.subresourceRange.baseArrayLayer = 0;
//	barrier.subresourceRange.layerCount = 1;
//	VkPipelineStageFlags sourceStage;
//	VkPipelineStageFlags destinationStage;
//	if( oldLayout == VK_ShaderModule_LAYOUT_UNDEFINED && newLayout == VK_ShaderModule_LAYOUT_TRANSFER_DST_OPTIMAL ){
//		barrier.srcAccessMask = 0;
//		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
//		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
//		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
//	} else if( oldLayout == VK_ShaderModule_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_ShaderModule_LAYOUT_SHADER_READ_ONLY_OPTIMAL ){
//		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
//		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
//		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
//		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
//	} else {
//		throw std::invalid_argument("unsupported layout transition!");
//	}
//	commandBuffer.pipelineBarrier(
//		sourceStage, destinationStage,
//		0,
//		0, nullptr,
//		0, nullptr,
//		1, &barrier
//	);
//	commandBuffer.end();
//	commandBuffer.execute();
//	commandBuffer.destroy();
//}
void VulkanShaderModule::loadFile( const String& filename, VulkanDevice* device ){
	assert( device );
	//std::cout << "VulkanShaderModule::loadFile" << filename << "\n"; std::cout.flush();
	if( _device != nullptr )
		throw std::runtime_error( "VulkanShaderModule already created" );
	_filename = filename;
	_device = device;
	auto code = readFile( _filename.asStd() );
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>( code.data() );
	assert( _vkShaderModule == nullptr );
	Vulkan::check( vkCreateShaderModule( _device->vkDevice(), &createInfo, nullptr, &_vkShaderModule ), "vkCreateShaderModule failed" );
}
void VulkanShaderModule::destroy(){
	if( _vkShaderModule ){
		vkDestroyShaderModule( device()->vkDevice(), _vkShaderModule, nullptr );
		_vkShaderModule = nullptr;
	}
	_device = nullptr;
}

std::vector<char> VulkanShaderModule::readFile( const std::string& filename ){
	std::ifstream file( filename, std::ios::ate | std::ios::binary );
	if (!file.is_open()) {
		logError( "VulkanShaderModule::readFile failed", filename );
		throw std::runtime_error( "failed to read shader-file" );
	}
	size_t fileSize = (size_t) file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();
	return buffer;
}
