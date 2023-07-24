#include "stdodt.h"
#include "VulkanRaytracingPipelineProperties.h"
#include "vulkan/device/VulkanPhysicalDevice.h"

VulkanRaytracingPipelineProperties::VulkanRaytracingPipelineProperties(){
    memset( &_properties, 0, sizeof( _properties ) );
}
VulkanRaytracingPipelineProperties::~VulkanRaytracingPipelineProperties(){
}
void VulkanRaytracingPipelineProperties::get( VulkanPhysicalDevice* physicalDevice ){
    assert( physicalDevice );
    // Get the properties of ray tracing pipelines on this device. We do this by
    // using vkGetPhysicalDeviceProperties2, and extending this by chaining on a
    // VkPhysicalDeviceRayTracingPipelinePropertiesKHR object to get both
    // physical device properties and ray tracing pipeline properties.
    // This gives us information about shader binding tables.
    assert( _properties.sType == 0 );
    _properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR;
    VkPhysicalDeviceProperties2 device_properties2;
    memset( &device_properties2, 0, sizeof( device_properties2 ) );
    device_properties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
    device_properties2.pNext = &_properties;
    vkGetPhysicalDeviceProperties2( physicalDevice->vkDevice(), &device_properties2 );
    const VkDeviceSize sbtHeaderSize      = _properties.shaderGroupHandleSize;
    const VkDeviceSize sbtBaseAlignment   = _properties.shaderGroupBaseAlignment;
    const VkDeviceSize sbtHandleAlignment = _properties.shaderGroupHandleAlignment;
    // Compute the stride between shader binding table (SBT) records.
    // This must be:
    // - Greater than _properties.shaderGroupHandleSize (since a record
    //     contains a shader group handle)
    // - A multiple of _properties.shaderGroupHandleAlignment
    // - Less than or equal to _properties.maxShaderGroupStride
    // In addition, each SBT must start at a multiple of
    // _properties.shaderGroupBaseAlignment.
    // Since we store all records contiguously in a single SBT, we assert that
    // sbtBaseAlignment is a multiple of sbtHandleAlignment, round sbtHeaderSize
    // up to a multiple of sbtBaseAlignment, and then assert that the result is
    // less than or equal to maxShaderGroupStride.
    assert( sbtBaseAlignment % sbtHandleAlignment == 0 );
    _sbtStride = sbtBaseAlignment * ((sbtHeaderSize + sbtBaseAlignment - 1) / sbtBaseAlignment);
    assert( _sbtStride <= _properties.maxShaderGroupStride );
}
const VkDeviceSize VulkanRaytracingPipelineProperties::sbtHeaderSize() const{
    assert( _properties.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR );
    return _properties.shaderGroupHandleSize;
}
const VkDeviceSize VulkanRaytracingPipelineProperties::sbtStride() const {
    assert( _properties.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR );
    return _sbtStride;
}

