#pragma once

#include "vulkan/VulkanCore.h"
#include "VulkanRaytracerData.h"
#include "vulkan/resource/VulkanBuffer.h"

class VulkanRaytracerGlobals
{
private:
	VulkanCommandPool* _commandPool;
private: public:
	VuklanRaytracerGlobalsData _data;
private:
	VulkanBuffer _buffer;
private:
	List<VulkanMaterial*> _materials;
public:
    explicit VulkanRaytracerGlobals();
    ~VulkanRaytracerGlobals();
public:
	void create( VulkanCommandPool* commandPool );
	void destroy();
public:
	const List<VulkanMaterial*>& materials(){ return _materials; }
	void addMaterial( VulkanMaterial* material );
//public:
//	void begin();
//	void addDecal( const Decal& decal, VulkanTextureArray& textures );
public:
	void update( VulkanTask& task );
private:
	void updateData();
	void updateBuffer( VulkanTask& task );
public:
	VulkanBuffer& buffer(){ return _buffer; }
public:
//	friend class VulkanComputeRaytracer;
//	friend class VulkanRaytracer;
};
