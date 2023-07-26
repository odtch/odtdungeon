#include "stdodt.h"

#include "VulkanComputeExecutor.h"
#include "VulkanComputeProgram.h"
#include "vulkan/device/VulkanDevice.h"

VulkanComputeExecutor::VulkanComputeExecutor()
	:_program( nullptr )
	,_state( Uninitialized )
{
}
VulkanComputeExecutor::~VulkanComputeExecutor(){
	ASSERT( !isBinded() );
	ASSERT( _state == Uninitialized );
}
void VulkanComputeExecutor::create( VulkanComputeProgram* program, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ  ){
	assert( program );
	assert( _program == null );
	ASSERT( _state == Uninitialized );
	_program = program;
	bindDevice( _program->device() );
	//_completedSemaphore.create( program->device() );
	_doneFence.create( "ComputeExecutor", 0, program->device() );
	_pipeline.create( &program->shaderModule(), &program->pipelineLayout() );
	_descriptorSet.create( &program->descriptorPool() );
	bind( _descriptorSet );
	_descriptorSet.update();
	_commandBuffer.begin( 0, &_program->commandPool() );
	_commandBuffer.bindPipeline( VK_PIPELINE_BIND_POINT_COMPUTE, &_pipeline );
	_commandBuffer.bindDescriptorSet( VK_PIPELINE_BIND_POINT_COMPUTE, &program->pipelineLayout(), &_descriptorSet );
	vkCmdDispatch( _commandBuffer.vkCommandBuffer(), groupCountX, groupCountY, groupCountZ  );
	_commandBuffer.end();
	_state = Initialized;
}
void VulkanComputeExecutor::destroy(){	
	_commandBuffer.destroy();
	_descriptorSet.destroy();
	_pipeline.destroy();
	_doneFence.clear();
	//_completedSemaphore.clear();
	_program = null;
	_state = Uninitialized;
	VulkanDeviceObject::destroy();
}
VulkanCommandPool& VulkanComputeExecutor::commandPool(){
	return _program->commandPool();
}

VulkanCommandBuffer&VulkanComputeExecutor::commandBuffer()
{
	return _commandBuffer;
}
bool VulkanComputeExecutor::isRunning() const{
	((VulkanComputeExecutor*)this)->updateState();
	if( _state == Running ){
		return true;
	} else {
		return false;
	}
}
bool VulkanComputeExecutor::isCompleted() const{
	((VulkanComputeExecutor*)this)->updateState();
	if( _state == Completed ){
		return true;
	} else {
		return false;
	}
}
void VulkanComputeExecutor::start(){
	if( _state == Initialized ){
	} else if( _state == Completed ){
		_doneFence.reset();
	} else {
		assert( false );
	}
	_state = Running;
	_commandBuffer.submit(
				//&_completedSemaphore,
				&_doneFence );
}
void VulkanComputeExecutor::updateState(){
	if( _state == Running ){
		if( _doneFence.isSignaled() ){
			_state = Completed;
			onCompleted();
		}
	}
}
void VulkanComputeExecutor::onCompleted(){
}
