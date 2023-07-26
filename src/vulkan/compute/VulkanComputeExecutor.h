#pragma once

#include "vulkan/VulkanCore.h"
#include "vulkan/device/VulkanDeviceObject.h"
#include "VulkanComputePipeline.h"
#include "vulkan/descriptor/VulkanDescriptorSet.h"
#include "vulkan/command/VulkanCommandBuffer.h"
#include "vulkan/command/VulkanFence.h"
#include "vulkan/resource/VulkanBuffer.h"

class VulkanComputeExecutor : public VulkanDeviceObject
{
public:
	enum State {
		Uninitialized,
		Initialized,
		Running,
		Completed
	};
private:
	VulkanComputeProgram* _program;
	VulkanComputePipeline _pipeline;
	VulkanDescriptorSet _descriptorSet;
	VulkanCommandBuffer _commandBuffer;
	//VulkanSemaphore _completedSemaphore;
	VulkanFence _doneFence;
private:
	State _state;
public:
	explicit VulkanComputeExecutor();
	virtual ~VulkanComputeExecutor() override;
public:
	void create( VulkanComputeProgram* program, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ );
	virtual void destroy() override;
protected:
	VulkanCommandPool& commandPool();
	VulkanCommandBuffer& commandBuffer();
protected:
	virtual void bind( VulkanDescriptorSet& descriptorSet ) = 0;
protected:
	//VulkanSemaphore& completedSemaphore(){ return _completedSemaphore; }
	VulkanFence& doneFence(){ return _doneFence; }
public:
	bool isRunning() const;
	bool isCompleted() const;
	void start();
private:
	void updateState();
protected:
	virtual void onCompleted();
};
