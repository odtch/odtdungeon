#pragma once

#include "vulkan/VulkanCore.h"
#include "VulkanImageArray.h"

class VulkanTextureArray
{
private:
	VulkanCommandPool* _commandPool;
private:
	List<Texture*> _textures;
private:
	List<VulkanImage*> _textureImages;
	List<VulkanImageView*> _textureImageViews;
	List<VulkanSampler*> _textureSamplers;
private:
	VulkanImageArray _imageArray;
public:
	explicit VulkanTextureArray();
	~VulkanTextureArray();
public:
	void create( VulkanCommandPool* commandPool );
	void destroy();
public:
	uint32_t size() const{ return _imageArray.size(); }
	uint getTextureIndex( Texture* texture );
public:
	VulkanImageArray& imageArray(){ return _imageArray; }
// public:
// 	friend class VulkanRaytracer;
// 	friend class VulkanRaytracerInstances;
};
