//#include "stdodt.h"
//#include "VulkanBLASList.h"
//#include "vulkan/OdtVulkan.h"
//#include "vulkan/resource/VulkanMesh.h"

//VulkanBLASList::VulkanBLASList()
//	:_commandPool( null )
//{
//}
//VulkanBLASList::~VulkanBLASList(){
//	if( _commandPool ){
//		logError( "~VulkanBLASList not destroyed" );
//	}
//	ASSERT( _commandPool == null );
//}
//void VulkanBLASList::create( VulkanCommandPool* commandPool ){
//	assert( _commandPool == null );
//	assert( commandPool );
//	_commandPool = commandPool;
//}
//void VulkanBLASList::destroy(){
//	_commandPool = null;
//	for( auto mesh : _meshes ){
//		assert( false );
//		/*
//		if( mesh->vulkanMesh()->hasVulkanMeshBuffer() ){
//			auto vb = mesh->vulkanMesh()->vulkanMeshBuffer();
//			mesh->vulkanMesh()->setVulkanMeshBuffer( null );
//			delete vb;
//		}
//		if( mesh->vulkanMesh()->hasVulkanBLAS() ){
//			mesh->vulkanMesh()->setVulkanBLAS( null );
//		}
//		*/
//	}
//	for( VulkanBLAS* blas : blases ){
//		blas->destroy();
//	}
//	blases.deleteAll();
//	_meshes.removeAll();
//}
//VulkanBLAS* VulkanBLASList::getBlasForMeshIndex(uint meshindex){
//	return blases.get( meshindex );
//}
