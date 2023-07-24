#pragma once

#include "vulkan/device/VulkanDeviceObject.h"
#include "vulkan/resource/VulkanBuffer.h"

class VulkanRaytracingShaderBindingTable : public VulkanDeviceObject
{
    DEBUGCOUNTER( VulkanRaytracingShaderBindingTable );
private:
    List<VulkanShaderModule*> _shaders;
private:
    enum { MaxStageCount = 256 };
    VkPipelineShaderStageCreateInfo _stages[ MaxStageCount ];
    uint32_t _stage_count = 0;
private:
    enum { MaxGroupCount = 16 };
    VkRayTracingShaderGroupCreateInfoKHR _groups[ MaxGroupCount ];
    uint32_t _group_count = 0;
private:
    uint32_t _raygen_count = 0;
    uint32_t _miss_count = 0;
	uint32_t _anyhit_count = 0;
	uint32_t _hit_count = 0;
	uint32_t _callable_count = 0;
public:
	explicit VulkanRaytracingShaderBindingTable();
	virtual ~VulkanRaytracingShaderBindingTable() override;
public:
	void bindDevice( VulkanDevice* device );
public:
    uint32_t stage_count() const{ return _stage_count; }
    VkPipelineShaderStageCreateInfo* stages_data(){ return _stages; }
	void bindShader( uint32_t index, VkShaderStageFlagBits stage, const String& shaderFilename, const char* main = "main" );
    void bindShader( uint32_t index, VkShaderStageFlagBits stage, VulkanShaderModule* shaderModule, const char* main = "main" );
public:
    uint32_t group_count() const{ return _group_count; }
    VkRayTracingShaderGroupCreateInfoKHR* groups_data(){ return _groups; }
    void bindGroupGeneral( uint32_t index, VkRayTracingShaderGroupTypeKHR type, uint32_t shader );
	void bindGroupClosestHit( uint32_t index, VkRayTracingShaderGroupTypeKHR type, uint32_t anyHitShader, uint32_t closestHitShader );
public:
    uint32_t raygen_count() const{ return _raygen_count; }
    uint32_t miss_count() const{ return _miss_count; }
    uint32_t hit_count() const{ return _hit_count; }
    uint32_t callable_count() const{ return _callable_count; }
	void setCount( uint32_t raygen_count, uint32_t miss_count, uint32_t hit_count, uint32_t callable_count );
public:
	void create();
public:
// private:
//     void resize( uint32_t size );
// public:
//     uint32_t size() const{ return _size; }
//     VkPipelineShaderStageCreateInfo* data(){ return &_stages[ 0 ]; }
// public:
//     void cmdTraceRays( VulkanCommandBuffer& commandBuffer, uint32_t width, uint32_t height, uint32_t depth );
public:
	virtual void destroy() override;
};


