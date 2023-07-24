#pragma once

#include "vulkan/VulkanCore.h"

class VulkanShaderModule
{
private:
	VulkanDevice* _device;
private:
	String _filename;
private:
	VkShaderModule _vkShaderModule;
public:
	explicit VulkanShaderModule();
	~VulkanShaderModule();
public:
	bool isLoaded() const{ return _vkShaderModule != null; }
	void loadFile( const String& filename, VulkanDevice* device );
	void destroy();
public:
	VulkanDevice* device() const{ assert( _device ); return _device; }
	VkShaderModule vkShaderModule() const{ assert( _vkShaderModule ); return _vkShaderModule; }
private:
	static std::vector<char> readFile( const std::string& filename );

};
