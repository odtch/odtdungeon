#pragma once

#include "vulkan/VulkanCore.h"
#include "VulkanBuffer.h"
#include "VulkanMeshBuffer.h"
#include "resource/Mesh.h"
#include "vulkan/command/VulkanTask.h"

class VulkanMeshPool;

class VulkanMeshData : public SharedPointerData
{
	DEBUGCOUNTER(VulkanMeshData);
private:
	VulkanMeshPool* _pool;
	uint _index;
private:
	size_t _verticesSize;
	size_t _indicesSize;
private:
	VulkanMeshBuffer* _meshbuffer = null;
private:
	VulkanBLAS* _blas = null;
public:
	bool hasVulkanMeshBuffer() const{ return _meshbuffer != null; }
	VulkanMeshBuffer* vulkanMeshBuffer() const{ ASSERT( _meshbuffer ); return _meshbuffer; }
public:
	bool hasVulkanBLAS() const{ return _blas != null; }
	VulkanBLAS* vulkanBLAS() const{ ASSERT( _blas ); return _blas; }
public:
	explicit VulkanMeshData( uint index, size_t verticesSize, size_t indicesSize, VulkanMeshBuffer* meshbuffer, VulkanBLAS* blas, VulkanMeshPool* pool );
	~VulkanMeshData();
public:
	uint index() const{ return _index; }
public:
	friend class VulkanMesh;
	friend class VulkanMeshDataPointer;
	friend class VulkanMeshPool;
	friend class VulkanMeshLoader;
};

class VulkanMeshDataPointer : private SharedPointer<VulkanMeshData>
{
private:
	explicit VulkanMeshDataPointer( VulkanMeshData* data );
public:
	explicit VulkanMeshDataPointer();
	explicit VulkanMeshDataPointer( const VulkanMeshDataPointer& buffer );
	virtual ~VulkanMeshDataPointer() override;
public:
	bool isNull() const { return SharedPointer<VulkanMeshData>::isNull(); }
public:
	void operator=( const VulkanMeshDataPointer& copy ){ SharedPointer<VulkanMeshData>::operator=( copy ); }
public:
	VulkanMeshData* data(){ return SharedPointer<VulkanMeshData>::data(); }
protected:
	virtual void destroyData( VulkanMeshData* data ) override;
public:
	friend class VulkanMeshPool;
	friend class VulkanMeshLoader;
	friend class VulkanRaytracer;
};

class VulkanMesh
{
	DEBUGCOUNTER( VulkanMesh )
	ODTNOCOPY( VulkanMesh )
private:
	VulkanMeshPool* _pool;
private: public:
	AbstractMesh* _mesh;
private:
	uint _instance_count = 0;
private:
	VulkanMeshDataPointer _current;
	//VulkanMeshDataPointer _back;
public:
	explicit VulkanMesh( AbstractMesh* mesh, VulkanMeshPool* pool );
	virtual ~VulkanMesh();
public:
	uint instanceCount() const{ return _instance_count; }
	void incInstanceCount();
	void decInstanceCount();
public:
	bool hasCurrent() const{ return !_current.isNull(); }
	VulkanMeshDataPointer& current(){ ASSERT( !_current.isNull() ); return _current; }
public:
	friend class VulkanMeshLoader;
	friend class VulkanMeshBackgroundLoader;
};

