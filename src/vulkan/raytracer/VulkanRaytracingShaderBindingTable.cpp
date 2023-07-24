#include "stdodt.h"
#include "VulkanRaytracingShaderBindingTable.h"
#include "vulkan/resource/VulkanShaderModule.h"
#include "vulkan/device/VulkanDevice.h"

VulkanRaytracingShaderBindingTable::VulkanRaytracingShaderBindingTable(){
    memset( &_stages, 0, sizeof( _stages ) );
    for( int s = 0; s < MaxStageCount; s++ ){
        _stages[s].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    }
    memset( &_groups, 0, sizeof( _groups ) );
    for( int g = 0; g < MaxGroupCount; g++ ){
        _groups[g].sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
        _groups[g].generalShader = VK_SHADER_UNUSED_KHR;
        _groups[g].closestHitShader = VK_SHADER_UNUSED_KHR;
        _groups[g].anyHitShader = VK_SHADER_UNUSED_KHR;
        _groups[g].intersectionShader = VK_SHADER_UNUSED_KHR;
        _groups[g].pShaderGroupCaptureReplayHandle = null;
    }
}
VulkanRaytracingShaderBindingTable::~VulkanRaytracingShaderBindingTable(){
    ASSERT( _shaders.isEmpty() );
}
void VulkanRaytracingShaderBindingTable::bindDevice( VulkanDevice* device ){
    VulkanDeviceObject::bindDevice( device );
}
void VulkanRaytracingShaderBindingTable::bindShader( uint32_t index, VkShaderStageFlagBits stage, const String& shaderFilename, const char* main ){
    assert( index < MaxStageCount );
    VulkanShaderModule* shaderModule = new VulkanShaderModule();
    _shaders.add( shaderModule );
	shaderModule->loadFile( shaderFilename, device() );
    bindShader( index, stage, shaderModule, main );
    // First, we create objects that point to each of our shaders.
    // These are called "shader stages" in this context.
    // These are shader module + entry point + stage combinations, because each
    // shader module can contain multiple entry points (e.g. main1, main2...)
    //_stages.bind( 0, VK_SHADER_STAGE_RAYGEN_BIT_KHR, rgenShaderModule );
}
void VulkanRaytracingShaderBindingTable::bindShader( uint32_t index, VkShaderStageFlagBits stage, VulkanShaderModule* shaderModule, const char* main ){
    assert( 0 <= index && index < MaxStageCount );
    assert( 0 < stage );
    assert( shaderModule );
    assert( _stages[ index ].module == null );
    if( _stage_count < index + 1 ){
        uint32_t size = index + 1;
        assert( _stage_count < size );
        assert( 0 < size && size < MaxStageCount );
        _stage_count = size;
    }
    _stages[ index ].stage  = stage;
    _stages[ index ].module = shaderModule->vkShaderModule();
    _stages[ index ].pName  = main;
}
void VulkanRaytracingShaderBindingTable::bindGroupGeneral( uint32_t index, VkRayTracingShaderGroupTypeKHR type, uint32_t shader ){
    assert( index == _group_count );
    assert( _group_count + 1 < MaxGroupCount );
    _groups[ _group_count ].type = type;
	_groups[ _group_count ].generalShader = shader;  // Index of ray gen, miss, or callable in `stages`
    _group_count++;
}
void VulkanRaytracingShaderBindingTable::bindGroupClosestHit(uint32_t index, VkRayTracingShaderGroupTypeKHR type, uint32_t anyHitShader, uint32_t closestHitShader){
    assert( index == _group_count );
    assert( _group_count + 1 < MaxGroupCount );
    _groups[ _group_count ].type = type;
	_groups[ _group_count ].anyHitShader = anyHitShader;
	_groups[ _group_count ].closestHitShader = closestHitShader;
    _group_count++;
}
void VulkanRaytracingShaderBindingTable::setCount( uint32_t raygen_count, uint32_t miss_count, uint32_t hit_count, uint32_t callable_count ){
    _raygen_count = raygen_count;
    _miss_count = miss_count;
    _hit_count = hit_count;
	_callable_count = callable_count;
}

void VulkanRaytracingShaderBindingTable::create(){
}
void VulkanRaytracingShaderBindingTable::destroy(){
    for( auto shader : _shaders ){
        shader->destroy();
    }
    _shaders.deleteAll();
    VulkanDeviceObject::destroy();
}
