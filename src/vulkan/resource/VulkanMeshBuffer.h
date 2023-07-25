#pragma once

#include "vulkan/VulkanCore.h"
#include "VulkanBuffer.h"
#include "resource/Mesh.h"
#include "vulkan/command/VulkanTask.h"

namespace VulkanMeshBufferState {
	enum Type {
		NotInitialized,
		Loading,
		Loaded
	};
}

class VulkanMeshBufferData : public SharedPointerData
{
	DEBUGCOUNTER(VulkanMeshBufferData);
private:
	VulkanMeshBufferState::Type _state = VulkanMeshBufferState::NotInitialized;
	AbstractMesh* _mesh = null;
	size_t _vertexSize = 0;
	uint32_t _vertexCount = 0;
	VulkanBuffer _vertexBuffer;
	uint32_t _indexCount = 0;
	VulkanBuffer _indexBuffer;
public:
	friend class VulkanMeshBuffer;
};

class VulkanMeshBuffer : private SharedPointer<VulkanMeshBufferData>
{
	DEBUGCOUNTER( VulkanMeshBuffer );
public:
	explicit VulkanMeshBuffer();
	explicit VulkanMeshBuffer( const VulkanMeshBuffer& buffer );
	virtual ~VulkanMeshBuffer() override;
public:
	bool isNull(){ return SharedPointer<VulkanMeshBufferData>::isNull(); }
public:
	VulkanMeshBufferState::Type state() const { return data()->_state; }
public:
	VkGeometryFlagsKHR geometryFlags() const { return data()->_mesh->geometryFlags(); }
public:
	void create( size_t verticesMemorySize, size_t indicesMemorySize, VkBufferUsageFlags usage, VulkanDevice* device );
	void destroy();
public:
	template< typename V >
	void startLoad( VulkanTask& task, Mesh<V>* mesh, VkBufferUsageFlags vertexUsage, VkBufferUsageFlags indexUsage ){
		assert( false );
		assert( mesh );
		VulkanMeshBufferData* data = this->data();
		data->_mesh = mesh;
		const void* vertices = mesh->verticesData();
		data->_vertexSize = sizeof( V );
		data->_vertexCount = mesh->vertexCount();
		const size_t verticesSize = data->_vertexSize * data->_vertexCount;
		data->_indexCount = mesh->indexCount();
		const void* indices = mesh->indicesData();
		const size_t indicesSize = sizeof( uint32_t ) * data->_indexCount;
		startLoad( task, vertices, verticesSize, vertexUsage, indices, indicesSize, indexUsage );
	}
	void startLoad( VulkanTask& task, AbstractMesh* mesh, VkBufferUsageFlags vertexUsage, VkBufferUsageFlags indexUsage );
	void startLoad( VulkanTask& task, const void* vertices, const size_t& verticesSize, VkBufferUsageFlags vertexUsage, const void* indices, const size_t& indicesSize, VkBufferUsageFlags indexUsage );
public:
	const size_t& vertexSize() const{ return data()->_vertexSize; }
	uint32_t vertexCount() const{ return data()->_vertexCount; }
	const VulkanBuffer& vertexBuffer() const{ return data()->_vertexBuffer; }
	VulkanBuffer& vertexBuffer() { return data()->_vertexBuffer; }
public:
	uint32_t indexCount() const{ return data()->_indexCount; }
	const VulkanBuffer& indexBuffer() const{ return data()->_indexBuffer; }
	VulkanBuffer& indexBuffer(){ return data()->_indexBuffer; }
private:
	void onLoadCompleted();
protected:
	virtual void destroyData( VulkanMeshBufferData* data ) override;
public:
	friend class VulkanMeshBufferLoadCommand;
};
