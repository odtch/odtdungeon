#include "stdodt.h"

#include "VulkanDescriptorSetLayout.h"
#include "vulkan/device/VulkanDevice.h"

VulkanDescriptorSetLayout::VulkanDescriptorSetLayout()
	:_bindings_count( 0 )
	,_vkDescriptorSetLayout( nullptr )
{
}
VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout(){
	ASSERT( _vkDescriptorSetLayout == nullptr );
	destroy();
}
void VulkanDescriptorSetLayout::addBinding( uint32_t binding, VkDescriptorType type, uint32_t count, VkShaderStageFlags stageFlags, const VkSampler* pImmutableSampler ){
	assert( _vkDescriptorSetLayout == nullptr );
	assert( _bindings_count < Max_Binding_Count );
	assert( binding == _bindings_count );
	VkDescriptorSetLayoutBinding& b = _bindings[ _bindings_count ];
	b.binding = binding;
	b.descriptorType = type;
	b.descriptorCount = count;
	b.stageFlags = stageFlags;
	b.pImmutableSamplers = pImmutableSampler;
	_bindings_count++;
}
const VkDescriptorType& VulkanDescriptorSetLayout::descriptorType(int i) const{
	assert( 0 <= i && i < _bindings_count );
	return _bindings[ i ].descriptorType;
}
uint32_t VulkanDescriptorSetLayout::descriptorSize( int i ) const {
	assert( 0 <= i && i < _bindings_count );
	return _bindings[ i ].descriptorCount;
}
void VulkanDescriptorSetLayout::create( VulkanDevice* device ){
	//std::cout << "VulkanDescriptorSetLayout::create\n";
	bindDevice( device );
	if( _vkDescriptorSetLayout != nullptr )
		throw std::runtime_error( "VulkanDescriptorSetLayout already created" );

	VkDescriptorSetLayoutCreateInfo layoutInfo;
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.pNext = nullptr;
	layoutInfo.flags = 0;
	layoutInfo.bindingCount = _bindings_count;
	layoutInfo.pBindings = _bindings;
	//for( uint b = 0; b < layoutInfo.bindingCount; b++ ){
	//	std::cout << "  " << b << " count=" << layoutInfo.pBindings[b].descriptorCount << "\n";
	//}
	Vulkan::check( vkCreateDescriptorSetLayout( vkDevice(), &layoutInfo, nullptr, &_vkDescriptorSetLayout ), "vkCreateDescriptorSetLayout failed" );
	assert( _vkDescriptorSetLayout );
}
void VulkanDescriptorSetLayout::destroy(){
	if( _vkDescriptorSetLayout ){
		vkDestroyDescriptorSetLayout( vkDevice(), _vkDescriptorSetLayout, nullptr );
		_vkDescriptorSetLayout = nullptr;
	}
	_bindings_count = 0;
	VulkanDeviceObject::destroy();
}
