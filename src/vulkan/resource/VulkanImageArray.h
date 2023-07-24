#pragma once

#include "vulkan/VulkanCore.h"

class VulkanImageArray
{
private:
	uint32_t _maxSize;
	uint32_t _size;
	Array<VkDescriptorImageInfo> _imageinfos;
public:
	explicit VulkanImageArray( uint32_t maxSize );
	~VulkanImageArray();
public:
	uint32_t size() const{ return _size; }
	void reset();
	void add( VulkanImageView* textureImageView, VulkanSampler* textureSampler );
public:
	const VkDescriptorImageInfo* descriptorBufferInfos() const{ assert( 0 < size() ); return &_imageinfos.first(); }
};
