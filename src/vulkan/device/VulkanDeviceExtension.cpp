#include "stdodt.h"
#include "VulkanDeviceExtension.h"

VulkanDeviceExtension::VulkanDeviceExtension(){
}
VulkanDeviceExtension::~VulkanDeviceExtension(){
}
void VulkanDeviceExtension::installRequirements( [[maybe_unused]] VulkanRequirements& requirements ){
}
void VulkanDeviceExtension::checkSupportedFeatures( [[maybe_unused]] const VkPhysicalDeviceFeatures& features ){
}
void VulkanDeviceExtension::bindDevice( VulkanDevice* device ){
	assert( device );
}

