#include "stdodt.h"
#include "VulkanRaytracerInstance.h"

VulkanRaytracerInstancePNT::VulkanRaytracerInstancePNT(VulkanRaytracerRenderLayer* layer, const PosOri& posori, Mesh<VertexPNT>* mesh, Material* material)
	:RenderInstancePNT( posori, mesh, material )
	,_layer( asserted( layer ) )
{
}
VulkanRaytracerInstancePNT::~VulkanRaytracerInstancePNT(){
}
