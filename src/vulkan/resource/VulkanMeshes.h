#pragma once

#include "vulkan/VulkanCore.h"
#include "resource/Mesh.h"

class VulkanMeshes {
	DEBUGCOUNTER( VulkanMeshes )
	ODTNOCOPY( VulkanMeshes )
private:
	VulkanMeshPool* _meshpool;
private:
	Array<VulkanMesh*> _index2vulkan;
private:
	List<VulkanMesh*> _vulkanmeshes;
public:
	explicit VulkanMeshes( VulkanMeshPool* meshpool );
	virtual ~VulkanMeshes();
public:
	void registerPNT( MeshPNT* mesh );
};

