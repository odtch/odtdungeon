#include "stdodt.h"

#include "VulkanSampler.h"
#include "vulkan/device/VulkanDevice.h"
//#include "vulkan/device/VulkanPhysicalDevice.h"
//#include "VulkanBuffer.h"
//#include "VulkanCommandPool.h"
//#include "VulkanCommandBuffer.h"

VulkanSampler::VulkanSampler()
	:_device( nullptr )
	,_vkSampler( nullptr )
{
}
VulkanSampler::~VulkanSampler(){
	if( _vkSampler ){
		std::cout << "~VulkanSampler not destroyed\n";
	}
	destroy();
	ASSERT( _vkSampler == nullptr && _device == nullptr );
}
void VulkanSampler::create( VulkanDevice* device ){
	assert( device );
	if( _vkSampler != nullptr || _device != nullptr )
		throw std::runtime_error( "VulkanSampler already created" );
	_device = device;
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = 16.0f;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	assert( _vkSampler == nullptr );
	Vulkan::check( vkCreateSampler( _device->vkDevice(), &samplerInfo, nullptr, &_vkSampler ), "vkCreateSampler failed" );
}
void VulkanSampler::destroy(){
	if( _vkSampler ){
		vkDestroySampler( device()->vkDevice(), _vkSampler, nullptr );
		_vkSampler = nullptr;
	}
	_device = nullptr;
}
