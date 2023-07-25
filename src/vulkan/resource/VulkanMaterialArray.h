#pragma once

#include "vulkan/VulkanCore.h"
#include "vulkan/raytracer/VulkanRaytracerData.h"
#include "vulkan/resource/VulkanBuffer.h"
#include "vulkan/resource/VulkanTextureArray.h"

class VulkanRaytracerGlobals;

class VulkanMaterialArray
{
private:
	VulkanCommandPool* _commandPool = null;
    VulkanTextureArray* _textures = null;
private:
	enum { MaxMaterialCount = 400 };
public: // todo_rm
	VulkanMaterialData _materials[ MaxMaterialCount ];
	uint32_t _materialCount = 0;
private:
	bool _modified = true;
private:
	VulkanBuffer _buffer;
public:
	explicit VulkanMaterialArray();
	~VulkanMaterialArray();
public:
	void create( VulkanCommandPool* commandPool, VulkanTextureArray* textures );
	void destroy();
public:
	void update( VulkanRaytracerGlobals& globals, VulkanTask& task );
public:
	VulkanBuffer& buffer(){ return _buffer; }
};
