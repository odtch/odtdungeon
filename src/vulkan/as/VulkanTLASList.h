#pragma once

#include "vulkan/VulkanCore.h"
#include "resource/Mesh.h"
#include "vulkan/resource/VulkanImageArray.h"
#include "VulkanTLAS.h"

class VulkanTLASList
{
	DEBUGCOUNTER(VulkanTLASList);
private:
	VulkanCommandPool* _commandPool = null;
private:
	enum { MaxTlasCount = 10 };
private:
	List<VulkanTLAS*> _tlases;
private:
	uint32_t _next_tlas_index = 0;
private:
	VkWriteDescriptorSetAccelerationStructureKHR _descriptor_info;
	VkAccelerationStructureKHR _accelerations[ MaxTlasCount ];
public:
	explicit VulkanTLASList();
	~VulkanTLASList();
public:
	void create( VulkanCommandPool* commandPool );
	void destroy();
public:
	void reset();
	VulkanTLAS* get( int index );
	VulkanTLAS* next();
	void update();
public:
	uint32_t count() const{ return _next_tlas_index; }
//	VkWriteDescriptorSetAccelerationStructureKHR* descriptor_info(){ return &_descriptor_info[0]; }
	VkWriteDescriptorSetAccelerationStructureKHR* descriptor_info(){ return &_descriptor_info; }
};
