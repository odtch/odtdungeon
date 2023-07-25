#pragma once

#include "vulkan/VulkanCore.h"
#include "vulkan/resource/VulkanBufferArray.h"

class VulkanMeshPool {
	DEBUGCOUNTER( VulkanMeshPool );
	ODTNOCOPY( VulkanMeshPool );
private:
	VulkanDevice* _device = null;
private:
	List<VulkanMeshData*> _datas;
	List<VulkanMeshData*> _useds;
	List<VulkanMeshData*> _unuseds;
private:
	VulkanBufferArray _vertexBufferArray;
	VulkanBufferArray _indexBufferArray;
public:
	explicit VulkanMeshPool();
	virtual ~VulkanMeshPool();
public:
	uint32_t size() const{ return _datas.size(); }
	VulkanBufferArray& vertexBufferArray(){ return _vertexBufferArray; }
	VulkanBufferArray& indexBufferArray(){ return _indexBufferArray; }
public:
	void create( VulkanDevice* device );
	void destroy();
public:
	VulkanMeshDataPointer createData( size_t verticesSize, size_t indicesSize );
private:
	void releaseData( VulkanMeshData* mesh );
public:
	friend class VulkanMeshDataPointer;
};

