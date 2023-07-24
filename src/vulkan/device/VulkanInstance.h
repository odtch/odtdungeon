#pragma once

#include "vulkan/VulkanCore.h"

class VulkanInstanceGlobals
{
private:
	std::vector<VkLayerProperties> _availableLayers;
private:
	std::vector<VkExtensionProperties> _availableExtensions;
public:
	VulkanInstanceGlobals();
	~VulkanInstanceGlobals();
private:
	void loadAvailableLayers();
public:
	const std::vector<VkLayerProperties>& availableLayers() const{ return _availableLayers; }
	bool isLayerAvailable( const char* layerName ) const;
	void traceAvailableLayers();
private:
	void loadAvailableExtensions();
public:
	const std::vector<VkExtensionProperties>& availableExtensions() const{ return _availableExtensions; }
	bool isExtensionAvailable( const char* extensionName ) const;
	void traceAvailableExtensions();
};

class VulkanInstance
{
private:
	VkInstance _instance;
private:
	VkDebugUtilsMessengerEXT _debugMessenger;
private:
	std::vector<VulkanPhysicalDevice*> _physicalDevices;
public:
	VulkanInstance();
	~VulkanInstance();
public:
	void create( const char* applicationName, const VulkanRequirements& requirements );
	void destroy();
public:
	const VkInstance& vkInstance() const{ assert( _instance ); return _instance; }
public:
	const std::vector<VulkanPhysicalDevice*>& physicalDevices() const{ return _physicalDevices; }
public:
	PFN_vkVoidFunction getInstanceProcAddr( const char* name );
};

