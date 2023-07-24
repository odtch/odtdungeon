#pragma once

#include "vulkan/VulkanCore.h"
#include "VulkanRaytracerData.h"
#include "vulkan/resource/VulkanBuffer.h"

class VulkanRaytracerGlobals
{
private:
	VulkanCommandPool* _commandPool;
private:
    VuklanRaytracerGlobalsData data;
private:
	VulkanBuffer _buffer;
public:
    explicit VulkanRaytracerGlobals();
    ~VulkanRaytracerGlobals();
public:
	void create( VulkanCommandPool* commandPool );
	void destroy();
public:
	void addMaterial( VulkanMaterial* material );
//public:
//	void begin();
//	void addDecal( const Decal& decal, VulkanTextureArray& textures );
//public:
//	void update( VulkanTask& task );
public:
//	friend class VulkanComputeRaytracer;
//	friend class VulkanRaytracer;
};
