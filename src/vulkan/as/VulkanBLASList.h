//#pragma once

//#include "vulkan/VulkanCore.h"
//#include "VulkanBLAS.h"
//#include "resource/Mesh.h"

//class VulkanBLASList
//{
//private:
//	VulkanCommandPool* _commandPool;
//private:
//	List<MeshPNT*> _meshes;
//private:
//	//List<VulkanMeshBuffer*> vulkan_meshes;
//	List<VulkanBLAS*> blases;
//	std::vector<VulkanBLAS*> blasesArray;
//public:
//	explicit VulkanBLASList();
//	~VulkanBLASList();
//public:
//	void create( VulkanCommandPool* commandPool );
//	void destroy();
//public:
//    int size() const{ return blases.size(); }
//	VulkanBLAS* getBlasForMeshIndex( uint meshindex );
//public:
//    friend class VulkanComputeRaytracer;
//    friend class VulkanRaytracer;
//};
