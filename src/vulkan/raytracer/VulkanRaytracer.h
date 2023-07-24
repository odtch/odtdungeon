#pragma once

#include "window/Window.h"
#include "renderer/Renderer.h"
#include "vulkan/VulkanCore.h"
#include "vulkan/present/VulkanPresenter.h"

class VulkanRaytracer : public VulkanPresenter, private MessageHandler
{
private:
	MessageQueue _queue;
private:
	VulkanRaytracerGlobals* _globals = null;
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
	virtual void render() override;
public:
	virtual bool handle( const Message& message ) override;
};
