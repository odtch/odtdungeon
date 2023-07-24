#include "stdodt.h"
#include "VulkanAccelerationStructureExtension.h"
#include "vulkan/device/VulkanDevice.h"

const char* VulkanAccelerationStructureExtension::Name = VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME;

VulkanAccelerationStructureExtension::VulkanAccelerationStructureExtension(){
	memset( &_features, 0, sizeof( _features ) );
	_createAccelerationStructureKHR = null;
	_getAccelerationStructureBuildSizesKHR = null;
}
VulkanAccelerationStructureExtension::~VulkanAccelerationStructureExtension(){
}
const char* VulkanAccelerationStructureExtension::name() const {
	return Name;
}
void VulkanAccelerationStructureExtension::installRequirements( VulkanRequirements& requirements ){
	VulkanDeviceExtension::installRequirements( requirements );
	{
		auto requirement = new VulkanTypedDeviceFeatureRequirement<VkPhysicalDeviceBufferDeviceAddressFeatures>( VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES );
		requirement->features.bufferDeviceAddress = VK_TRUE;
		requirements.add( requirement );
	}{
		auto requirement = new VulkanTypedDeviceFeatureRequirement<VkPhysicalDeviceAccelerationStructureFeaturesKHR>( VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR );
		requirement->features.accelerationStructure = VK_TRUE;
//		requirement->features.accelerationStructureCaptureReplay = VK_TRUE;
//		requirement->features.accelerationStructureIndirectBuild = VK_TRUE;
//		requirement->features.accelerationStructureHostCommands = VK_TRUE;
//		requirement->features.descriptorBindingAccelerationStructureUpdateAfterBind = VK_TRUE;
		requirements.add( requirement );
	}
}
void* VulkanAccelerationStructureExtension::resetFeature(){
	memset( &_features, 0, sizeof( _features ) );
	_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
	return &_features;
}
void VulkanAccelerationStructureExtension::checkSupportedFeatures( const VkPhysicalDeviceFeatures &features ){
	VulkanDeviceExtension::checkSupportedFeatures( features );
	if( _features.accelerationStructure != VK_TRUE )
		throw std::runtime_error( "no accelerationStructure" );
}
void VulkanAccelerationStructureExtension::bindDevice( VulkanDevice* device ){
	VulkanDeviceExtension::bindDevice( device );
	assert( _createAccelerationStructureKHR == null );
	_createAccelerationStructureKHR = reinterpret_cast<PFN_vkCreateAccelerationStructureKHR>(
		vkGetDeviceProcAddr( device->vkDevice(), "vkCreateAccelerationStructureKHR" )
	);
	assert( _createAccelerationStructureKHR );
	_getAccelerationStructureBuildSizesKHR = reinterpret_cast<PFN_vkGetAccelerationStructureBuildSizesKHR>(
		vkGetDeviceProcAddr( device->vkDevice(), "vkGetAccelerationStructureBuildSizesKHR" )
	);
	assert( _getAccelerationStructureBuildSizesKHR );
	_cmdBuildAccelerationStructuresKHR = reinterpret_cast<PFN_vkCmdBuildAccelerationStructuresKHR>(
		vkGetDeviceProcAddr( device->vkDevice(), "vkCmdBuildAccelerationStructuresKHR" )
	);
	assert( _cmdBuildAccelerationStructuresKHR );
}
