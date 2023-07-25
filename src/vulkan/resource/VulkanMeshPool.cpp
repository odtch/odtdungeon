#include "stdodt.h"
#include "VulkanMeshPool.h"
#include "VulkanMesh.h"
#include "vulkan/as/VulkanBLAS.h"

VulkanMeshPool::VulkanMeshPool()
	:_vertexBufferArray( 2000 )
	,_indexBufferArray( 2000 )
{
}
VulkanMeshPool::~VulkanMeshPool(){
	ASSERT( _datas.isEmpty() );
	ASSERT( _device == null );
}
void VulkanMeshPool::create( VulkanDevice* device ){
	assert( device );
	assert( _device == null );
	_device = device;
}
void VulkanMeshPool::destroy(){
	if( _device == null ){
		ASSERT( _useds.isEmpty() );
		return;
	}
	ASSERT( _device );
	assert( _useds.isEmpty() );
	for( VulkanMeshData* data : _unuseds ){
		if( data->_blas ){
			data->_blas->destroy();
			odelete( data->_blas );
		}
		if( data->_meshbuffer ){
			data->_meshbuffer->destroy();
			odelete( data->_meshbuffer );
		}
	}
	for( VulkanMeshData* data : _unuseds ){
		odelete( data );
	}
	_useds.removeAll();
	_datas.removeAll();
	_unuseds.removeAll();
	_vertexBufferArray.reset();
	_indexBufferArray.reset();
	_device = null;
}
VulkanMeshDataPointer VulkanMeshPool::createData( size_t verticesSize, size_t indicesSize ){
	ASSERT( 0 < verticesSize ); ASSERT( 0 < indicesSize );
	VulkanMeshData* best_data = null;
	size_t best_overhead = 0;
	for( VulkanMeshData* existing : _unuseds ){
		if( existing->_verticesSize < verticesSize )
			continue;
		if( existing->_indicesSize < indicesSize )
			continue;
		size_t overhead = ( existing->_verticesSize - verticesSize ) + ( existing->_indicesSize < indicesSize );
		if( best_data == null || overhead < best_overhead ){
			best_data = existing;
			best_overhead = overhead;
		}
	}
	if( best_data != null ){
		//logDebug( "VMP reusing ", best_data->_index, best_data->_verticesSize, best_data->_indicesSize );
		_unuseds.remove( best_data );
//		vertexBufferArray.set( meshIndex, &meshBuffer->vertexBuffer() );
//		indexBufferArray.set( meshIndex, &meshBuffer->indexBuffer() );
	} else {
		uint index = _datas.size();
		VulkanMeshBuffer* meshbuffer = new VulkanMeshBuffer();
		// VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		const VkBufferUsageFlags usage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
										 | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
										 | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR
										 | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		meshbuffer->create( verticesSize, indicesSize, usage, _device );
		VulkanBLAS* blas = new VulkanBLAS();
		best_data = new VulkanMeshData( index, verticesSize, indicesSize, meshbuffer, blas, this );
		ASSERT( index == _vertexBufferArray.size() );
		_vertexBufferArray.add( &meshbuffer->vertexBuffer() );
		ASSERT( index == _indexBufferArray.size() );
		_indexBufferArray.add( &meshbuffer->indexBuffer() );
//		blases.add( blas );
//		blasesArray.push_back( blas );
		ASSERT( index == _datas.size() );
		_datas.add( best_data );
		//logDebug( "VMP new ", best_data->_index, best_data->_verticesSize, best_data->_indicesSize );

	}
	_useds.add( best_data );
	return VulkanMeshDataPointer( best_data );
}
void VulkanMeshPool::releaseData( VulkanMeshData* data ){
	ASSERT( data );
	ASSERT( data->_pool == this );
	//logDebug( "VMP release ", data->_index, data->_verticesSize, data->_indicesSize );
	_useds.remove( data );
	_unuseds.add( data );

}
