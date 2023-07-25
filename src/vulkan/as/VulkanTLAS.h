#pragma once

#include "VulkanAccelerationStructure.h"
#include "resource/ResourceCore.h"

class VulkanTLAS : public VulkanAccelerationStructure
{
private:
	uint32_t _index;
private:
	enum { MaxInstanceCount = 320000 };
	VkAccelerationStructureInstanceKHR _instances[ MaxInstanceCount ];
	uint32_t _instance_count = 0;
private:
	VulkanBuffer _instanceBuffer;
	uint32_t _prev_count = 0;
private:
	VulkanBuffer _stagingBuffer;
	VulkanBuffer _scratchBuffer;
private:
	bool _rebuilding = false;
	List<VulkanBLAS*> _buildingBlases;
public:
	explicit VulkanTLAS( uint32_t index );
	virtual ~VulkanTLAS() override;
public:
	uint32_t index() const{ return _index; }
public:
	void reset();
	void add(
		const Mat4& transform,
		uint32_t instanceIndex,
		uint32_t mask,
		uint32_t instanceShaderBindingTableRecordOffset,
		VkGeometryInstanceFlagsKHR flags,
		VulkanBLAS* blas
	);
	void addRebuild( VulkanTask& loadtask, VulkanTask& buildtask );
	//bool update( VulkanCommandPool& commandPool );
	bool isEmpty() const{ return _instance_count == 0; }
public:
	virtual void destroy() override;
public:
	friend class VulkanTLASUpdateCommand;
};


