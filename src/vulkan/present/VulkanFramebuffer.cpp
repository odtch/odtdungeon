#include "stdodt.h"

#include "VulkanFramebuffer.h"
#include "vulkan/device/VulkanDevice.h"
//#include "VulkanPhysicalDevice.h"
//#include "VulkanBuffer.h"
//#include "VulkanCommandPool.h"
#include "VulkanRenderPass.h"
//#include "VulkanCommandBuffer.h"

VulkanFramebuffer::VulkanFramebuffer()
	:_vkFramebuffer( nullptr )
{
}
VulkanFramebuffer::~VulkanFramebuffer(){
	destroy();
	ASSERT( _vkFramebuffer == nullptr );
}
void VulkanFramebuffer::create( VulkanRenderPass* renderPass, const VkExtent2D& extent, const std::vector<VkImageView>& attachments ){
	assert( renderPass );
	bindDevice( renderPass->device() );
	VkFramebufferCreateInfo framebufferInfo{};
	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.renderPass = renderPass->vkRenderPass();
	framebufferInfo.attachmentCount = static_cast<uint32_t>( attachments.size());
	framebufferInfo.pAttachments = attachments.data();
	framebufferInfo.width = extent.width;
	framebufferInfo.height = extent.height;
	framebufferInfo.layers = 1;
	assert( _vkFramebuffer == nullptr );
	Vulkan::check( vkCreateFramebuffer( vkDevice(), &framebufferInfo, nullptr, &_vkFramebuffer ), "vkCreateFramebuffer failed" );
}
void VulkanFramebuffer::destroy(){
	if( _vkFramebuffer ){
		vkDestroyFramebuffer( device()->vkDevice(), _vkFramebuffer, nullptr );
		_vkFramebuffer = nullptr;
	}
	VulkanDeviceObject::destroy();
}
