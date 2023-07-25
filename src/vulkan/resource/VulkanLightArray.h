#pragma once

#include "vulkan/VulkanCore.h"
#include "vulkan/resource/VulkanBuffer.h"
#include "vulkan/raytracer/VulkanRaytracerData.h"

class RenderLight;

class VulkanLightArray
{
private:
	VulkanCommandPool* _commandPool;
private:
	VulkanLightData _lights[ VulkanLight_MaxCount ];
	uint _light_count;
private:
	bool _modified = true;
private:
	VulkanBuffer _buffer;
public:
	explicit VulkanLightArray();
	~VulkanLightArray();
public:
	void create( VulkanCommandPool* commandPool );
	void destroy();
public:
	void set( uint index, const RenderLight& light );
	void setCount( uint count );
//	void reset();
//	void add( const PosOri& posori, int type, float range, const vec4& color );
//	uint count() const{ return _light_count; }
	void update( VulkanTask& task );
public:
    friend class VulkanComputeRaytracer;
    friend class VulkanRaytracer;
};
