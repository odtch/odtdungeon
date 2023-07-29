#pragma once

#include "renderer/RenderInstance.h"
#include "vulkan/VulkanCore.h"

class VulkanRaytracerInstancePNT : public RenderInstancePNT
{
public:
	VulkanRaytracerRenderLayer* _layer;
public:
	explicit VulkanRaytracerInstancePNT( VulkanRaytracerRenderLayer* layer, const PosOri& posori, Mesh<VertexPNT>* mesh, Material* material );
	virtual ~VulkanRaytracerInstancePNT() override;
};
