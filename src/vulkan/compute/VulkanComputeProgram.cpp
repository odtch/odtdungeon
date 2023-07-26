#include "stdodt.h"

#include "VulkanComputeProgram.h"
#include "vulkan/device/VulkanDevice.h"

VulkanComputeProgram::VulkanComputeProgram(){
}
VulkanComputeProgram::~VulkanComputeProgram(){
	ASSERT( !isBinded() );
}
void VulkanComputeProgram::create( const String& shaderFilename, uint32_t maxExecutorCount, VulkanDevice* device ){
	assert( device );
	bindDevice( device );
	addBindings( _descriptorSetLayout );
	_descriptorSetLayout.create( device );
	_descriptorPool.create( 1, maxExecutorCount, &_descriptorSetLayout );
	_pipelineLayout.addSet( &_descriptorSetLayout );
	_pipelineLayout.create( device );
	_shaderModule.loadFile( shaderFilename.asStd(), device );
	_commandPool.create( device->getComputeQueue() );
}
void VulkanComputeProgram::destroy(){
	_commandPool.destroy();
	_shaderModule.destroy();
	_descriptorPool.destroy();
	_pipelineLayout.destroy();
	_descriptorSetLayout.destroy();
	VulkanDeviceObject::destroy();
}
