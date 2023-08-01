#pragma once

#include "renderer/RenderInstance.h"
#include "vulkan/VulkanCore.h"

class VulkanRaytracerInstancePNT : public RenderInstancePNT
{
public:
	VulkanRaytracerRenderLayer* _layer;
public:
	Mesh<VertexPNT>* _prevMesh = null;
	Material* _prevMaterial = null;
public:
	explicit VulkanRaytracerInstancePNT( VulkanRaytracerRenderLayer* layer, const Mat4& posori, Mesh<VertexPNT>* mesh, Material* material );
	virtual ~VulkanRaytracerInstancePNT() override;
};
