#pragma once

#include "vulkan/VulkanCore.h"
#include "vulkan/device/VulkanDeviceObject.h"
#include "vulkan/resource/VulkanBuffer.h"

class VulkanAccelerationStructure : public VulkanDeviceObject
{
private:
	VulkanAccelerationStructureExtension* _extension;
private:
	VkAccelerationStructureKHR _vkAccelerationStructure;
	VulkanBuffer _buffer;
public:
	explicit VulkanAccelerationStructure();
	virtual ~VulkanAccelerationStructure() override;
protected:
	void bindDevice( VulkanDevice* device );
	VulkanAccelerationStructureExtension* extension() const{ assert( _extension ); return _extension; }
protected:
	void create( VkAccelerationStructureTypeKHR type, VkDeviceSize size );
	void bind( VkAccelerationStructureKHR vkAccelerationStructure );
public:
	virtual void destroy() override;
public:
	bool isCreated() const{ return _vkAccelerationStructure != nullptr; }
	const VkAccelerationStructureKHR& vkAccelerationStructure() const{ assert( _vkAccelerationStructure != nullptr ); return _vkAccelerationStructure; }
	VulkanBuffer& buffer(){ return _buffer; }
};

