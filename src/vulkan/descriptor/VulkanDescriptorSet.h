#pragma once

#include "vulkan/VulkanCore.h"
#include "vulkan/device/VulkanDeviceObject.h"

class VulkanDescriptorSet : public VulkanDeviceObject
{
private:
	VulkanDescriptorPool* _descriptorPool;
private:
	VkDescriptorSet _vkDescriptorSet;
private:
	enum { Max_Bind_Count = 11 };
	VkDescriptorBufferInfo _buffer_infos[ Max_Bind_Count ];
	VkDescriptorImageInfo _image_infos[ Max_Bind_Count ];
	VkWriteDescriptorSet _writes[ Max_Bind_Count ];
private:
	VkWriteDescriptorSetAccelerationStructureKHR _writeDescriptorSetAccelerationStructure;
public:
	explicit VulkanDescriptorSet();
	~VulkanDescriptorSet();
public:
	bool isCreated() const{ return _vkDescriptorSet != null; }
	void create( VulkanDescriptorPool* descriptorPool );
	void destroy();
public:
	void reset();
public:
	void bindStorageBuffer( int index, VulkanBuffer& buffer );
	void bindUniformBuffer( int index, VulkanBuffer& buffer );
	void bindBuffer( int index, VkDescriptorType descriptorType, VulkanBuffer& buffer );
	void bindBufferArray( int index, VkDescriptorType descriptorType, VulkanBufferArray* bufferArray );
	void bindTexture( int index, VulkanImageView* textureImageView, VulkanSampler* textureSampler );
	void bindTextureArray( int index, VulkanImageArray* textures );
	void bindTextureArray( int index, VulkanTextureArray* textures );
	void bindStorageImage( int index, VulkanImageView* imageView );
	void bindStorageImage( int index, VkImageView imageView );
	void bindAccelerationStructure( int index, VulkanAccelerationStructure* structure );
	void bindAccelerations( int index, uint32_t count, VkWriteDescriptorSetAccelerationStructureKHR* accelerations );public:
	void update();
public:
	const VkDescriptorSet& vkDescriptorSet() const{ assert( _vkDescriptorSet ); return _vkDescriptorSet; }
	VulkanDescriptorPool* pool() const{ assert( _descriptorPool ); return _descriptorPool; }
	VulkanDescriptorSetLayout* setlayout() const;
};


