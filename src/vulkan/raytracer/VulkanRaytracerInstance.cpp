#include "stdodt.h"
#include "VulkanRaytracerInstance.h"

VulkanRaytracerInstancePNT::VulkanRaytracerInstancePNT(VulkanRaytracerRenderLayer* layer, const Mat4& posori, Mesh<VertexPNT>* mesh, Material* material)
	:RenderInstancePNT( posori, mesh, material )
	,_layer( asserted( layer ) )
{
}
VulkanRaytracerInstancePNT::~VulkanRaytracerInstancePNT(){
}
