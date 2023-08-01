#include "stdodt.h"
#include "VulkanMeshes.h"
#include "VulkanMesh.h"
#include "vulkan/as/VulkanBLAS.h"

VulkanMeshes::VulkanMeshes( VulkanMeshPool* meshpool )
	:_meshpool( asserted( meshpool ) )
	,_index2vulkan( 300 )
{
	_index2vulkan.fill( null );
}
VulkanMeshes::~VulkanMeshes(){
	while( !_vulkanmeshes.isEmpty() ){
		VulkanMesh* vm = _vulkanmeshes.takeFirst();
		vm->_mesh->setVulkanMesh( null );
		delete vm;
	}
}
void VulkanMeshes::registerPNT( MeshPNT* mesh ){
	assert( mesh );
	if( _index2vulkan.size() <= mesh->meshindex() ){
		logError( "VulkanMeshes::registerPNT meshindex to high", mesh->meshindex(), _index2vulkan.size() );
		assert( false );
	}
	VulkanMesh* vulkanmesh = _index2vulkan.get( mesh->meshindex() );
	if( vulkanmesh == null ){
		vulkanmesh = new VulkanMesh( mesh, _meshpool );
		_vulkanmeshes.add( vulkanmesh );
		mesh->setVulkanMesh( vulkanmesh );
		_index2vulkan.set( mesh->meshindex(), vulkanmesh );
	}
}
