#include "stdodt.h"
#include "VulkanRaytracer.h"
#include "VulkanPipelineRaytracerExtension.h"
#include "VulkanRaytracingPipelineExtension.h"
#include "vulkan/device/VulkanInstance.h"
#include "vulkan/device/VulkanRequirement.h"
#include "vulkan/as/VulkanAccelerationStructureExtension.h"

class xDummyRenderInstancePNT : public RenderInstancePNT
{
public:
    explicit xDummyRenderInstancePNT( const PosOri& posori, Mesh<VertexPNT>* mesh, Material* material )
        :RenderInstancePNT( posori, mesh, material ){}
    virtual ~xDummyRenderInstancePNT() override{}
};



VulkanRaytracer::VulkanRaytracer( Window* window )
	:VulkanPresenter( VK_IMAGE_USAGE_TRANSFER_DST_BIT, window )
{
}
VulkanRaytracer::~VulkanRaytracer(){
}
Material* VulkanRaytracer::createMaterial( const String& name ){
    Material* material = new Material( name );
    return material;
}

Mesh<VertexPNT>* VulkanRaytracer::createDynamicMeshPNT( const String& name ){
    MeshPNT* mesh = new MeshPNT( name );
    return mesh;
}
RenderInstancePNT* VulkanRaytracer::createInstance( const PosOri& posori, Mesh<VertexPNT>* mesh, Material* material ){
    RenderInstancePNT* instance = new xDummyRenderInstancePNT( posori, mesh, material );
	return instance;
}
void VulkanRaytracer::getRequirements( VulkanRequirements& requirements ){
	requirements.requireApiVersion( VK_API_VERSION_1_2 );
	requirements.requireDeviceExtension( VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME );
	requirements.requireDeviceExtension<VulkanAccelerationStructureExtension>();
	requirements.requireDeviceExtension<VulkanRaytracingPipelineExtension>();
	{
		auto requirement = new VulkanTypedDeviceFeatureRequirement<VkPhysicalDeviceDescriptorIndexingFeatures>( VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES );
		requirement->features.runtimeDescriptorArray = VK_TRUE;
		requirements.add( requirement );
	}
	requirements.requireComputeQueue();
}
void VulkanRaytracer::run(){
    VulkanPresenter::run();
}

