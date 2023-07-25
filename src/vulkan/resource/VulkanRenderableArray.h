#pragma once

#include "vulkan/VulkanCore.h"
#include "vulkan/raytracer/VulkanRaytracerData.h"
#include "vulkan/resource/VulkanBuffer.h"

class VulkanRenderableArray
{
private:
	VulkanCommandPool* _commandPool = null;
private:
	enum { MaxInstanceCount = 123123 };
	VulkanRenderableData _instances[ MaxInstanceCount ];
private:
	uint32_t _next_instance_index = 0;
private:
	VulkanBuffer _buffer;
public:
	explicit VulkanRenderableArray();
	~VulkanRenderableArray();
public:
	void create( VulkanCommandPool* commandPool );
	void destroy();
public:
	void reset();
	uint32_t add( const Mat4& posori, uint meshIndex, uint materialIndex, const vec4& color, const float textureOffsetY, float textureTile ); //, const vec4& datas, int effect );
	void update( VulkanTask& task );
public:
	bool isEmpty() const{ return _next_instance_index == 0; }
	uint32_t count() const{ return _next_instance_index; }
public:
	VulkanBuffer& buffer(){ return _buffer; }
};
