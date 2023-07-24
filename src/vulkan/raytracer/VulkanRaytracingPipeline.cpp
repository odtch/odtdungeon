#include "stdodt.h"

#include "VulkanRaytracingPipeline.h"
#include "VulkanRaytracingPipelineProperties.h"
#include "vulkan/pipeline/VulkanPipelineLayout.h"
#include "vulkan/device/VulkanDevice.h"
#include "vulkan/resource/VulkanShaderModule.h"
#include "vulkan/command/VulkanCommandBuffer.h"
#include "VulkanRaytracingShaderBindingTable.h"

VulkanRaytracingPipeline::VulkanRaytracingPipeline(){
}
VulkanRaytracingPipeline::~VulkanRaytracingPipeline(){
}
void VulkanRaytracingPipeline::create( VulkanRaytracingShaderBindingTable* sbt, VulkanPipelineLayout* layout, VulkanRaytracingPipelineProperties& properties ){
    assert( sbt );
	assert( layout );
    assert( !isCreated() );
    assert( _sbt == null );
    _sbt = sbt;
    bindDevice( layout->device() );
    // Create the shader binding table and ray tracing pipeline.
    // We'll create the ray tracing pipeline by specifying the shaders + layout,
    // and then get the handles of the shaders for the shader binding table from
    // the pipeline.
    VkRayTracingPipelineCreateInfoKHR pipelineCreateInfo;
    memset( &pipelineCreateInfo, 0, sizeof( pipelineCreateInfo ) );
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR;
    pipelineCreateInfo.flags                             = 0;  // No flags to set
    pipelineCreateInfo.stageCount                        = _sbt->stage_count();
    pipelineCreateInfo.pStages                           = _sbt->stages_data();
    pipelineCreateInfo.groupCount                        = _sbt->group_count();
    pipelineCreateInfo.pGroups                           = _sbt->groups_data();
	pipelineCreateInfo.maxPipelineRayRecursionDepth      =	3; // 1; // Depth of call tree
    pipelineCreateInfo.layout                            = layout->vkPipelineLayout();
//	for( int g = 0; g < pipelineCreateInfo.groupCount; g++ ){
//		VkRayTracingShaderGroupCreateInfoKHR* group = (VkRayTracingShaderGroupCreateInfoKHR*) &(pipelineCreateInfo.pGroups[ g]) ;
//		logDebug( group->anyHitShader, group->closestHitShader );
//	}
    PFN_vkCreateRayTracingPipelinesKHR createRayTracingPipelinesKHR =
        reinterpret_cast<PFN_vkCreateRayTracingPipelinesKHR>(
        device()->getProcAddr( "vkCreateRayTracingPipelinesKHR" )
        );
    assert( createRayTracingPipelinesKHR );
    VkPipeline rtPipeline = null;
    Vulkan::check( createRayTracingPipelinesKHR(
        vkDevice(),
        VK_NULL_HANDLE,          // Deferred operation or VK_NULL_HANDLE
        VK_NULL_HANDLE,          // Pipeline cache or VK_NULL_HANDLE
        1, &pipelineCreateInfo,  // Array of create infos
        nullptr,                 // Allocator
        &rtPipeline
    ), "vkCreateRayTracingPipelinesKHR" );

    // Now create and write the shader binding table, by getting the shader
    // group handles from the ray tracing pipeline and writing them into a
    // Vulkan buffer object.

    // Get the shader group handles:
    std::vector<uint8_t> cpuShaderHandleStorage( properties.sbtHeaderSize() * _sbt->group_count() );
    PFN_vkGetRayTracingShaderGroupHandlesKHR getRayTracingShaderGroupHandlesKHR=
        reinterpret_cast<PFN_vkGetRayTracingShaderGroupHandlesKHR>(
        device()->getProcAddr( "vkGetRayTracingShaderGroupHandlesKHR" )
        );
    assert( getRayTracingShaderGroupHandlesKHR );
    Vulkan::check( getRayTracingShaderGroupHandlesKHR(
        device()->vkDevice(),
        rtPipeline,                            // Pipeline
        0,                                     // First group
        _sbt->group_count(), //static_cast<uint32_t>(groups.size()),  // Number of groups
        cpuShaderHandleStorage.size(),         // Size of buffer
        cpuShaderHandleStorage.data()       // Data buffer
      ), "vkGetRayTracingShaderGroupHandlesKHR" );
    // Allocate the shader binding table. We get its device address, and
    // use it as a shader binding table. As before, we set its memory property
    // flags so that it can be read and written from the CPU.
    const uint32_t sbtSize = static_cast<uint32_t>( properties.sbtStride() * _sbt->group_count() );
    _sbtBuffer.create(
        sbtSize,
        VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        device()
    );
    // Copy the shader group handles to the SBT:
    uint8_t mappedSBT[ sbtSize ];
    memset( &mappedSBT, 0, sizeof( mappedSBT ) );
    for( size_t groupIndex = 0; groupIndex < _sbt->group_count(); groupIndex++ ){
        memcpy( &mappedSBT[ groupIndex * properties.sbtStride() ], &cpuShaderHandleStorage[ groupIndex * properties.sbtHeaderSize() ], properties.sbtHeaderSize() );
    }
    _sbtBuffer.loadStaging( mappedSBT, sbtSize );

   	assert( rtPipeline );
	bindPipeline( rtPipeline );

    const VkDeviceAddress sbtStartAddress = _sbtBuffer.getBufferDeviceAddress();
    {
        const VkDeviceSize sbtStride = properties.sbtStride();
        assert( _sbt->raygen_count() == 1 );
        sbtRayGenRegion.deviceAddress   = sbtStartAddress;
        sbtRayGenRegion.stride          = sbtStride;
        sbtRayGenRegion.size            = _sbt->raygen_count() * sbtStride;
        sbtMissRegion.deviceAddress     = sbtRayGenRegion.deviceAddress + sbtRayGenRegion.size;
        sbtMissRegion.stride            = sbtStride;
        sbtMissRegion.size              = _sbt->miss_count() * sbtStride;
        sbtHitRegion.deviceAddress      = sbtMissRegion.deviceAddress + sbtMissRegion.size;
        sbtHitRegion.stride             = sbtStride;
        sbtHitRegion.size               = _sbt->hit_count() * sbtStride;
        sbtCallableRegion.deviceAddress = sbtHitRegion.deviceAddress + sbtHitRegion.size;
        sbtCallableRegion.stride        = sbtStride;
        sbtCallableRegion.size          = _sbt->callable_count() * sbtStride;
    }
    _cmdTraceRaysKHR =
        reinterpret_cast<PFN_vkCmdTraceRaysKHR>(
        device()->getProcAddr( "vkCmdTraceRaysKHR" )
    );
    assert( _cmdTraceRaysKHR );
}
void VulkanRaytracingPipeline::cmdTraceRays( VulkanCommandBuffer& commandBuffer, uint32_t width, uint32_t height, uint32_t depth ){
    assert( _cmdTraceRaysKHR );
    _cmdTraceRaysKHR( commandBuffer.vkCommandBuffer(),
                      &sbtRayGenRegion,    // Region of memory with ray generation groups
                      &sbtMissRegion,      // Region of memory with miss groups
                      &sbtHitRegion,       // Region of memory with hit groups
                      &sbtCallableRegion,  // Region of memory with callable groups
                      width, height, depth
    );
}
void VulkanRaytracingPipeline::destroy(){
	_sbtBuffer.clear();
    _sbt = null;
    VulkanPipeline::destroy();
}
