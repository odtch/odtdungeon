#pragma once

#include "window/Window.h"
#include "renderer/Renderer.h"
#include "vulkan/VulkanCore.h"
#include "vulkan/present/VulkanPresenter.h"

class VulkanRaytracer : public VulkanPresenter
{
public:
    explicit VulkanRaytracer( Window* window );
    virtual ~VulkanRaytracer() override;
public:
    virtual Material* createMaterial( const String& name ) override;
public:
    virtual MeshPNT* createDynamicMeshPNT( const String& name ) override;
public:
    virtual RenderInstancePNT* createInstance( const PosOri& posori, MeshPNT* mesh, Material* material ) override;
protected:
	virtual void getRequirements( VulkanRequirements& requirements ) override;
protected:
    virtual void run() override;
};
