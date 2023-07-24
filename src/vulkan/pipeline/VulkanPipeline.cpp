#include "stdodt.h"

#include "VulkanPipeline.h"
#include "VulkanPipelineLayout.h"
#include "vulkan/device/VulkanDevice.h"
#include "vulkan/resource/VulkanShaderModule.h"
#include "vulkan/present/VulkanRenderPass.h"

VulkanPipeline::VulkanPipeline()
	:_vkPipeline( nullptr )
{
}
VulkanPipeline::~VulkanPipeline(){
	ASSERT( _vkPipeline == nullptr );
	destroy();
}
void VulkanPipeline::bindPipeline( VkPipeline vkPipeline ){
	assert( _vkPipeline == nullptr);
	assert( vkPipeline );
	_vkPipeline = vkPipeline;
}
void VulkanPipeline::destroy(){
	if( _vkPipeline ){
		vkDestroyPipeline( vkDevice(), _vkPipeline, nullptr );
		_vkPipeline = nullptr;
	}
	VulkanDeviceObject::destroy();
}



VulkanGraphicsPipeline::VulkanGraphicsPipeline()
	:_shaderstage_count( 0 )
{
	memset( &_shaderstages, 0, sizeof( _shaderstages ) );
}
VulkanGraphicsPipeline::~VulkanGraphicsPipeline(){
}
void VulkanGraphicsPipeline::addShaderStage( VkShaderStageFlagBits stage, VulkanShaderModule* shader ){
	assert( shader );
	assert( !isCreated() );
	assert( _shaderstage_count +1 <= Max_ShaderStage_Count );
	VkPipelineShaderStageCreateInfo& stageinfo = _shaderstages[ _shaderstage_count ];
	stageinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	stageinfo.stage = stage;
	stageinfo.module = shader->vkShaderModule();
	stageinfo.pName = "main";
	_shaderstage_count++;
}
void VulkanGraphicsPipeline::destroy(){
	_shaderstage_count = 0;
	VulkanPipeline::destroy();
}
void VulkanGraphicsPipeline::create(
		const VkVertexInputBindingDescription& vertexInputBindingDescription,
		const VkVertexInputAttributeDescription* vertexInputAttributeDescription,
		int vertexInputAttributeDescriptionCount,
		VkPrimitiveTopology topology,
		VulkanRenderPass& renderPass,
		const VkExtent2D& renderTargetSize, VulkanPipelineLayout* layout
){
	assert( layout );
	bindDevice( layout->device() );

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &vertexInputBindingDescription;
	vertexInputInfo.vertexAttributeDescriptionCount = vertexInputAttributeDescriptionCount;
	vertexInputInfo.pVertexAttributeDescriptions = vertexInputAttributeDescription;

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = topology;
	inputAssembly.primitiveRestartEnable = VK_FALSE;


	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.stencilTestEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	VkViewport viewport;
	memset( &viewport, 0, sizeof( viewport ) );
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float) renderTargetSize.width;
	viewport.height = (float) renderTargetSize.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor;
	memset( &scissor, 0, sizeof( scissor ) );
	scissor.offset = {0, 0};
	scissor.extent = renderTargetSize;
	VkPipelineViewportStateCreateInfo viewportState;
	memset( &viewportState, 0, sizeof( viewportState  ) );
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkGraphicsPipelineCreateInfo pipelineInfo;
	memset( &pipelineInfo, 0, sizeof( pipelineInfo ) );
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = _shaderstage_count;
	pipelineInfo.pStages = _shaderstages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.layout = layout->vkPipelineLayout();
	pipelineInfo.renderPass = renderPass.vkRenderPass();
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	VkPipeline vkPipeline = nullptr;
	Vulkan::check( vkCreateGraphicsPipelines( vkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &vkPipeline ), "vkCreateGraphicsPipelines failed" );
	assert( vkPipeline );
	bindPipeline( vkPipeline );
}
