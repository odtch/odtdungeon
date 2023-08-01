#include "stdodt.h"
#include "Mesh.h"
#include "ResourceType.h"
//#include "ResourceStorage.h"

AbstractMesh::AbstractMesh(){
	static std::atomic<uint32_t> next_meshindex( 0 );
	_meshindex = next_meshindex.fetch_add( 1, std::memory_order_relaxed );
}
AbstractMesh::~AbstractMesh(){
//	ASSERT( _vulkanmesh == null );
}
//void AbstractMesh::setUpdateBackground(){
//	_updateType = UpdateBackground;
//}
void AbstractMesh::setUnmodified(){
	_modificationState = NotModified;
}
void AbstractMesh::setModified( ModificationState modificationState ){
//	if( hasVulkanMeshBuffer() ){
//		std::cout << "AbstractMesh::setModified " << modificationState << "\n";
//	}
	_modificationState = modificationState;
}
void AbstractMesh::setGeometryFlags( VkGeometryFlagsKHR geometryFlags ){
	assert( _vulkanmesh == null );
	_geometryFlags = geometryFlags;
}
void AbstractMesh::setVulkanMesh( VulkanMesh* vulkanmesh ){
	if( vulkanmesh ){
		assert( _vulkanmesh == null );
		_vulkanmesh = vulkanmesh;
	} else {
		assert( _vulkanmesh != null );
		_vulkanmesh = null;
	}
}
//void AbstractMesh::collectPreLoad( List<AbstractMesh*>& meshes ){
//	meshes.add( this );
//}
void AbstractMesh::load( BinaryFileReader& reader ){
	//Resource::load( reader );
	_geometryFlags = reader.read_uint32();
}
void AbstractMesh::save( BinaryFileWriter& writer ) const {
//	Resource::save( writer );
	writer.write_uint32( _geometryFlags );
}

