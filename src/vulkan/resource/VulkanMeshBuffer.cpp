#include "stdodt.h"
#include "VulkanMeshBuffer.h"
#include "vulkan/command/VulkanCommandPool.h"


class VulkanMeshBufferLoadCommand : public VulkanCommand
{
private:
	VulkanMeshBuffer _buffer;
public:
	explicit VulkanMeshBufferLoadCommand( VulkanMeshBuffer& buffer );
	virtual ~VulkanMeshBufferLoadCommand() override;
public:
	virtual void bind( VulkanCommandBuffer& commandBuffer ) override;
	virtual void onCompleted() override;
};
VulkanMeshBufferLoadCommand::VulkanMeshBufferLoadCommand( VulkanMeshBuffer& buffer )
	:_buffer( buffer )
{
	assert( !_buffer.isNull() );
}
VulkanMeshBufferLoadCommand::~VulkanMeshBufferLoadCommand(){
	_buffer.clear();
}
void VulkanMeshBufferLoadCommand::bind( VulkanCommandBuffer& commandBuffer ){
}
void VulkanMeshBufferLoadCommand::onCompleted(){
	_buffer.onLoadCompleted();
	_buffer.clear();
}

//int ic = 0;
VulkanMeshBuffer::VulkanMeshBuffer()
	:SharedPointer<VulkanMeshBufferData>( new VulkanMeshBufferData() )
{
//	ic++;
//	logDebug( "VMB", this, ic );
}
VulkanMeshBuffer::VulkanMeshBuffer(const VulkanMeshBuffer& buffer)
	:SharedPointer<VulkanMeshBufferData>( buffer )
{
//	ic++;
//	logDebug( "VMBc", this, ic );
}
VulkanMeshBuffer::~VulkanMeshBuffer(){
//	ic++;
//	logDebug( "VMB~", this, ic );
	clear();
}
void VulkanMeshBuffer::create( size_t verticesMemorySize, size_t indicesMemorySize, VkBufferUsageFlags usage, VulkanDevice* device ){
	ASSERT( 0 < verticesMemorySize );
	ASSERT( 0 < indicesMemorySize );
	ASSERT( device );
	VulkanMeshBufferData* data = this->data();
	data->_vertexBuffer.create( verticesMemorySize, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, device );
	data->_indexBuffer.create( indicesMemorySize, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, device );
}
void VulkanMeshBuffer::destroy(){
	data()->_vertexBuffer.clear();
	data()->_indexBuffer.clear();
}
void VulkanMeshBuffer::startLoad( VulkanTask& task, AbstractMesh* mesh, VkBufferUsageFlags vertexUsage, VkBufferUsageFlags indexUsage ){
	assert( mesh );
	VulkanMeshBufferData* data = this->data();
	data->_mesh = mesh;
	const void* vertices = mesh->abstractVertexData();
	data->_vertexSize = mesh->abstractVertexSize();
	data->_vertexCount = mesh->abstractVertexCount();
	const size_t verticesSize = data->_vertexSize * data->_vertexCount;
	data->_indexCount = mesh->abstractIndexCount();
	const void* indices = mesh->abstractIndexData();
	const size_t indicesSize = sizeof( uint32_t ) * data->_indexCount;
	startLoad( task, vertices, verticesSize, vertexUsage, indices, indicesSize, indexUsage );
}
void VulkanMeshBuffer::startLoad( VulkanTask& task, const void* vertices, const size_t &verticesSize, VkBufferUsageFlags vertexUsage, const void* indices, const size_t &indicesSize, VkBufferUsageFlags indexUsage ){
	VulkanMeshBufferData* data = this->data();
	data->_state = VulkanMeshBufferState::Loading;
	// dynamisch recreate geht nicht weil raytracer
	if( data->_vertexBuffer.isCreated() ){
		if( data->_vertexBuffer.size() < verticesSize ){
			logError( "VulkanMeshBuffer::load vertex is to small, recreating" );
			assert( false );
			data->_vertexBuffer.clear();
		}
	}
	ASSERT( data->_vertexBuffer.isCreated() );
	if( !data->_vertexBuffer.isCreated() )
		data->_vertexBuffer.create( verticesSize, vertexUsage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, task.device() );
	task.addLoadBuffer( data->_vertexBuffer, vertices, verticesSize );
	ASSERT( data->_indexBuffer.isCreated() );
	if( data->_indexBuffer.isCreated() ){
		if( data->_indexBuffer.size() < indicesSize ){
			logError( "VulkanMeshBuffer::load index is to small, recreating" );
			assert( false );
			data->_indexBuffer.clear();
		}
	}
	if( !data->_indexBuffer.isCreated() )
		data->_indexBuffer.create( indicesSize, indexUsage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, task.device() );
	task.addLoadBuffer( data->_indexBuffer, indices, indicesSize );
	task.add( new VulkanMeshBufferLoadCommand( *this ) );
}
void VulkanMeshBuffer::onLoadCompleted(){
	auto data = this->data();
	//logDebug( "VMB.onLoadCompleted", data, data->_state );
	assert( data->_state == VulkanMeshBufferState::Loading );
//	if( data->_mesh ){
//		data->_mesh->setUnmodified();
//	}
	data->_state = VulkanMeshBufferState::Loaded;
}
void VulkanMeshBuffer::destroyData( VulkanMeshBufferData* data ){
	data->_indexBuffer.clear();
	data->_vertexBuffer.clear();
	delete data;
}
