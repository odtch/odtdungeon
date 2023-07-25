#include "stdodt.h"
#include "VulkanMesh.h"
#include "VulkanMeshPool.h"
#include "vulkan/as/VulkanBLAS.h"

VulkanMeshData::VulkanMeshData( uint index, size_t verticesSize, size_t indicesSize, VulkanMeshBuffer* meshbuffer, VulkanBLAS* blas, VulkanMeshPool* pool )
	:_pool( pool )
	,_index( index )
	,_verticesSize( verticesSize )
	,_indicesSize( indicesSize )
	,_meshbuffer( meshbuffer )
	,_blas( blas )
{
	ASSERT( _pool );
	ASSERT( _meshbuffer );
	ASSERT( _blas );
//	logDebug( "VMD", _index, _verticesSize, _indicesSize );
}
VulkanMeshData::~VulkanMeshData(){
//	logDebug( "VMD~", _index, _verticesSize, _indicesSize );
	_pool = null;
	ASSERT( _blas == null );
	ASSERT( _meshbuffer == null );
}


VulkanMeshDataPointer::VulkanMeshDataPointer( VulkanMeshData* data )
	:SharedPointer<VulkanMeshData>( data )
{
}
VulkanMeshDataPointer::VulkanMeshDataPointer(){
}
VulkanMeshDataPointer::VulkanMeshDataPointer(const VulkanMeshDataPointer& buffer)
	:SharedPointer<VulkanMeshData>( buffer )
{
}
VulkanMeshDataPointer::~VulkanMeshDataPointer(){
	clear();
}
void VulkanMeshDataPointer::destroyData( VulkanMeshData* data ){
	ASSERT( data );
	ASSERT( data->_pool );
	data->_pool->releaseData( data );
}

VulkanMesh::VulkanMesh( AbstractMesh* mesh, VulkanMeshPool* pool )
	:_pool( pool )
	,_mesh( mesh )
{
	ASSERT( _mesh );
	ASSERT( _pool );
}
VulkanMesh::~VulkanMesh(){
	ASSERT( 0 == _instance_count );
	_pool = null;
	_mesh = null;
}
void VulkanMesh::incInstanceCount(){
	_instance_count++;
}
void VulkanMesh::decInstanceCount(){
	ASSERT( 0 < _instance_count );
	_instance_count--;
}

