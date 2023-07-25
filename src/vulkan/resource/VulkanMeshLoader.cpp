#include "stdodt.h"
#include "VulkanMeshLoader.h"
#include "VulkanMesh.h"
#include "VulkanMeshPool.h"
#include "vulkan/as/VulkanBLAS.h"

VulkanMeshLoader::VulkanMeshLoader(){
}
VulkanMeshLoader::~VulkanMeshLoader(){
	ASSERT( _commandPool == null );
}
void VulkanMeshLoader::create( const String& name, VulkanCommandPool* commandPool ){
	ASSERT( commandPool );
	ASSERT( _commandPool == null );
	_commandPool = commandPool;
	_name = name;
}
void VulkanMeshLoader::destroy(){
	// wird mehrfach aufgerufen
	_loadMB.clear();
	_buildBLAS.clear();
	_commandPool = null;

}
void VulkanMeshLoader::reinit(){
	_loadMB.clear();
	_buildBLAS.clear();
	_loadMB.create( _name + ": load mb", VulkanTask::RunOnce, 500, _commandPool );
	_buildBLAS.create( _name + ": build blas", VulkanTask::RunOnce, 200, _commandPool );
	_buildBLAS.addWaitFor( _loadMB );
}
void VulkanMeshLoader::tryUpdateIfNeeded( VulkanMesh* mesh ){
	ASSERT( mesh );
	if( mesh->_current.isNull() ){
	} else if( mesh->_mesh->isModified() ){
	} else {
		return;
	}
	VulkanMeshDataPointer* dataptr = &mesh->_current;
	if( dataptr->isNull() ){
		(*dataptr) = mesh->_pool->createData( mesh->_mesh->abstractVertexMemorySize(), mesh->_mesh->abstractIndexMemorySize() );
	}
	VulkanMeshData* data = dataptr->data();
	if( data->_verticesSize < mesh->_mesh->abstractVertexMemorySize() || data->_indicesSize < mesh->_mesh->abstractIndexMemorySize() ){
		logWarning( "VulkanMeshLoader modified mesh is now bigger", mesh->_mesh ); //, mesh->_mesh->id() );
		dataptr->clear();
		(*dataptr) = mesh->_pool->createData( mesh->_mesh->abstractVertexMemorySize(), mesh->_mesh->abstractIndexMemorySize() );
		data = dataptr->data();
	}
	bool load_needed = true;
	if( mesh->_mesh->isModified() ){
		if( mesh->_mesh->isModifiedInGPU() ){
			load_needed = false;
		}
		mesh->_mesh->setUnmodified();
	}
	ASSERT( !data->vulkanBLAS()->_rebuildingTLAS );
	{
		data->vulkanBLAS()->setRebuildNeeded();
		if( load_needed ){
			//if( _tasks.loadMB.hasPlaceForNCommands( 3 ) ){
			const VkBufferUsageFlags usage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
											 | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR
											 | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			data->vulkanMeshBuffer()->startLoad( _loadMB, mesh->_mesh, usage, usage );
			assert( data->vulkanMeshBuffer()->state() == VulkanMeshBufferState::Loading );
//					} else {
//						logDebug( "loadMeshBufferTask to many mb to load" );
//					}
		}
	}
	if( data->vulkanBLAS()->isRebuildNeeded() ){
		//logDebug( "loadMeshBufferTask blas rebuild needed" );
		ASSERT( !data->vulkanBLAS()->_rebuildingTLAS );
		data->vulkanBLAS()->addRebuild( _buildBLAS, *data->vulkanMeshBuffer() );
	}
}
void VulkanMeshLoader::start(){
	_loadMB.start();
	_buildBLAS.start();

}

VulkanMeshBackgroundLoader::VulkanMeshBackgroundLoader(){
}
VulkanMeshBackgroundLoader::~VulkanMeshBackgroundLoader(){
}
void VulkanMeshBackgroundLoader::create( const String& name, VulkanCommandPool* commandPool ){
	_loader.create( name, commandPool );
}
void VulkanMeshBackgroundLoader::destroy(){
	_loader.destroy();
}
void VulkanMeshBackgroundLoader::preTick(){
	if( _loader.loadMBTask().isRunning() || _loader.buildBLASTask().isRunning() ){
		_running = true;
		return;
	}
	if( _running ){
		_running = false;
		_hasnewwork = false;
		count = 0;
	}
}
void VulkanMeshBackgroundLoader::tryUpdateIfNeeded( VulkanMesh* mesh ){
	if( _running )
		return;
	if( count > 100 )
		return;
	if( !_hasnewwork ){
		_loader.reinit();
		_hasnewwork = true;
	}

	if( mesh->_current.isNull() ){
		count++;
	} else if( mesh->_mesh->isModified() ){
		count++;
	} else {
	}


	_loader.tryUpdateIfNeeded( mesh );
}
void VulkanMeshBackgroundLoader::postTick(){
	if( _hasnewwork ){
		_loader.start();
		_hasnewwork = false;
		_running = true;
	}
}
