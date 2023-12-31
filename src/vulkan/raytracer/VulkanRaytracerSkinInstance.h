#pragma once

#include "renderer/RenderInstance.h"
#include "vulkan/VulkanCore.h"
#include "vulkan/resource/VulkanImage.h"
#include "vulkan/resource/VulkanImageView.h"

class VulkanRaytracerSkinInstance : public RenderSkinInstance
{
private:
	VulkanRaytracerRenderLayer* _layer;
private:
	MeshPNT* _mesh = null;
public:
	VulkanRaytracerInstancePNT* _pntinstance = null;
public:
	explicit VulkanRaytracerSkinInstance( VulkanRaytracerRenderLayer* layer, const Mat4& posori, Skin* skin );
	virtual ~VulkanRaytracerSkinInstance() override;
public:
	void createMesh( MeshPNT* mesh );
	void updateMeshInCPU();
public:
	friend class VulkanRaytracer;
};
