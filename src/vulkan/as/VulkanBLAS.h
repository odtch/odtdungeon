#pragma once

#include "VulkanAccelerationStructure.h"

class VulkanBLAS : public VulkanAccelerationStructure
{
private:
	bool _rebuildNeeded = true;
	VkAccelerationStructureGeometryKHR _geometry;
	VkAccelerationStructureBuildSizesInfoKHR _sizeInfo;
	VkAccelerationStructureBuildGeometryInfoKHR _buildGeometryInfo;
	VkAccelerationStructureBuildRangeInfoKHR _offsetInfo;
private:
	bool _rebuilding = false;
public:
	bool _rebuildingTLAS = false;
public:
	explicit VulkanBLAS();
	virtual ~VulkanBLAS() override;
public:
	bool isRebuildNeeded() const{ return _rebuildNeeded; }
	void setRebuildNeeded();
public:
	void addRebuild( VulkanTask& task, VulkanMeshBuffer& mesh );
	virtual void destroy() override;
public:
	friend class VulkanBLASUpdateCommand;
};

