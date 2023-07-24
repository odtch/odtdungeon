#include "stdodt.h"

#include "VulkanDescriptorPool.h"
#include "VulkanDescriptorSetLayout.h"
#include "vulkan/device/VulkanDevice.h"

VulkanDescriptorPool::VulkanDescriptorPool()
	:_vkDescriptorPool( nullptr )
	,_setlayout( nullptr )
{
}
VulkanDescriptorPool::~VulkanDescriptorPool(){
	ASSERT( _vkDescriptorPool == nullptr );
	destroy();
	ASSERT( _setlayout == nullptr );
}
//void VulkanDescriptorPool::create( const VulkanWindowRenderTarget& renderTarget, VulkanDescriptorSetLayout* setlayout ){
//	uint32_t descriptorCount = static_cast<uint32_t>( renderTarget.swapchainSize() );
//	uint32_t maxSets = descriptorCount;
//	create( descriptorCount, maxSets, setlayout );
//}
void VulkanDescriptorPool::create( uint32_t descriptorCount, uint32_t maxSets, VulkanDescriptorSetLayout* setlayout ){
	//std::cout << "VulkanDescriptorPool::create\n";
	assert( setlayout );
	if( _vkDescriptorPool != nullptr )
		throw std::runtime_error( "VulkanDescriptorPool already created" );
	assert( _setlayout == nullptr );
	_setlayout = setlayout;
	bindDevice( setlayout->device() );
	assert( setlayout->bindingCount() < 12 );
	VkDescriptorPoolSize poolSizes[ 12 ];
	for( int i = 0; i < setlayout->bindingCount(); i++ ){
		poolSizes[i].type = setlayout->descriptorType( i );
		poolSizes[i].descriptorCount = descriptorCount;
	}
	VkDescriptorPoolCreateInfo poolInfo;
	memset( &poolInfo, 0, sizeof( poolInfo ) );
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = setlayout->bindingCount();
	poolInfo.pPoolSizes = poolSizes;
	poolInfo.maxSets = maxSets;
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	Vulkan::check( vkCreateDescriptorPool( vkDevice(), &poolInfo, nullptr, &_vkDescriptorPool ), "vkCreateDescriptorPool failed" );
	assert( _vkDescriptorPool );
}
void VulkanDescriptorPool::destroy(){
	if( _vkDescriptorPool ){
		vkDestroyDescriptorPool( vkDevice(), _vkDescriptorPool, nullptr );
		_vkDescriptorPool = nullptr;
	}
	_setlayout = nullptr;
	VulkanDeviceObject::destroy();
}
