#include "stdodt.h"

#include "VulkanPipelineLayout.h"
#include "vulkan/device/VulkanDevice.h"
#include "vulkan/descriptor/VulkanDescriptorSetLayout.h"

VulkanPipelineLayout::VulkanPipelineLayout()
	:_set_count( 0 )
	,_push_constant_count( 0 )
	,_vkPipelineLayout( nullptr )
{
}
VulkanPipelineLayout::~VulkanPipelineLayout(){
	ASSERT( _vkPipelineLayout == nullptr );
	destroy();
}
void VulkanPipelineLayout::addSet( VulkanDescriptorSetLayout* set ){
	assert( set );
	assert( _vkPipelineLayout == nullptr );
	assert( _set_count < Max_Set_Count );
	_sets[ _set_count ] = set->vkDescriptorSetLayout();
	_set_count++;
}
void VulkanPipelineLayout::addPushConstant( VkShaderStageFlags flags, size_t size ){
	if( size % (4) != 0 ){
		assert( false );
		throw std::runtime_error( "VulkanPipelineLayout::addPushConstant size must be a multiple of 4 per the Vulkan spec!" );
	}
	assert( _push_constant_count == 0 );
	memset( &_push_constant_range, 0, sizeof( _push_constant_range ) );
	_push_constant_range.stageFlags = flags;
	_push_constant_range.offset = 0;
	_push_constant_range.size = size;
	_push_constant_count++;
}
void VulkanPipelineLayout::create( VulkanDevice* device ){
	//std::cout << "VulkanPipelineLayout::create\n";
	bindDevice( device );
	if( _vkPipelineLayout != nullptr )
		throw std::runtime_error( "VulkanPipelineLayout already created" );
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.pNext = nullptr;
	pipelineLayoutInfo.flags = 0;
	pipelineLayoutInfo.setLayoutCount = _set_count;
	pipelineLayoutInfo.pSetLayouts =_sets;
	pipelineLayoutInfo.pushConstantRangeCount = _push_constant_count;
	pipelineLayoutInfo.pPushConstantRanges = &_push_constant_range;
	Vulkan::check( vkCreatePipelineLayout( vkDevice(), &pipelineLayoutInfo, nullptr, &_vkPipelineLayout ), "vkCreatePipelineLayout failed" );
	assert( _vkPipelineLayout );
}
void VulkanPipelineLayout::destroy(){
	if( _vkPipelineLayout ){
		vkDestroyPipelineLayout( vkDevice(), _vkPipelineLayout, nullptr );
		_vkPipelineLayout = nullptr;
	}
	_set_count = 0;
	_push_constant_count = 0;
	VulkanDeviceObject::destroy();
}
