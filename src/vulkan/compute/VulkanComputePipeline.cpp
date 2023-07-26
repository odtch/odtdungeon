#include "stdodt.h"

#include "VulkanComputePipeline.h"
#include "vulkan/pipeline/VulkanPipelineLayout.h"
#include "vulkan/device/VulkanDevice.h"
#include "vulkan/resource/VulkanShaderModule.h"

VulkanComputePipeline::VulkanComputePipeline(){
}
VulkanComputePipeline::~VulkanComputePipeline(){
}
void VulkanComputePipeline::create( VulkanShaderModule* shaderModule, VulkanPipelineLayout* layout ){
	assert( shaderModule );
	assert( layout );
	bindDevice( layout->device() );
	VkComputePipelineCreateInfo pipelineCreateInfo;
	memset( &pipelineCreateInfo, 0, sizeof( pipelineCreateInfo ) );
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	pipelineCreateInfo.stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
	pipelineCreateInfo.stage.module = shaderModule->vkShaderModule();
	pipelineCreateInfo.stage.pName = "main";
	pipelineCreateInfo.layout = layout->vkPipelineLayout();
	VkPipeline vkPipeline = nullptr;
	Vulkan::check( vkCreateComputePipelines( vkDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, NULL, &vkPipeline ), "vkCreateComputePipelines failed" );
	assert( vkPipeline );
	bindPipeline( vkPipeline );
}
