#include "stdodt.h"
#include "VulkanRaytracingPipelineExtension.h"
#include "vulkan/device/VulkanRequirement.h"

const char* VulkanRaytracingPipelineExtension::Name = VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME;

VulkanRaytracingPipelineExtension::VulkanRaytracingPipelineExtension(){
}
VulkanRaytracingPipelineExtension::~VulkanRaytracingPipelineExtension(){
}
const char* VulkanRaytracingPipelineExtension::name() const {
	return Name;
}
void VulkanRaytracingPipelineExtension::installRequirements( VulkanRequirements &requirements ){
	VulkanDeviceExtension::installRequirements( requirements );
	{
		auto requirement = new VulkanTypedDeviceFeatureRequirement<VkPhysicalDeviceRayTracingPipelineFeaturesKHR>(
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR
        );
		requirement->features.rayTracingPipeline = VK_TRUE;
		requirements.add( requirement );
	}
}
void* VulkanRaytracingPipelineExtension::resetFeature(){
	memset( &_features, 0, sizeof( _features ) );
	_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
	return &_features;
}
void VulkanRaytracingPipelineExtension::checkSupportedFeatures( const VkPhysicalDeviceFeatures &features ){
	VulkanDeviceExtension::checkSupportedFeatures( features );
 	if( _features.rayTracingPipeline != VK_TRUE )
 		throw std::runtime_error( "no rayTracingPipeline" );
}
