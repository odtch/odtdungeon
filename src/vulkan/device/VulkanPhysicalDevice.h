#pragma once

#include "vulkan/VulkanCore.h"

class VulkanQueueFamily
{
private:
	VulkanPhysicalDevice* _device;
	uint32_t _index;
	VkQueueFamilyProperties _properties;
public:
	explicit VulkanQueueFamily( uint32_t index, const VkQueueFamilyProperties& properties, VulkanPhysicalDevice* device );
	~VulkanQueueFamily();
public:
	uint32_t index() const { return _index; }
public:
	bool isGraphics() const;
	bool isCompute() const;
	bool isPresent( const VulkanSurface& surface ) const;
public:
	const VkQueueFamilyProperties& properties() const { return _properties; }
};

class VulkanPhysicalDevice
{
private:
	VkPhysicalDevice _vkDevice;
private:
	VkPhysicalDeviceProperties _properties;
private:
	std::vector<VulkanQueueFamily*> _queueFamilies;
private:
	std::vector<VkExtensionProperties> _availableExtensions;
//private:
//	VkPhysicalDeviceFeatures _supportedFeatures;
//	VkPhysicalDeviceFeatures2 _supportedFeatures2;
//	VkPhysicalDeviceBufferDeviceAddressFeatures _addressFeatures;
public:
	VulkanPhysicalDevice( VkPhysicalDevice vkDevice );
	~VulkanPhysicalDevice();
public:
	VkPhysicalDevice vkDevice() const{ return _vkDevice; }
public:
	const char* name() const{ return _properties.deviceName; }
public:
	const VkPhysicalDeviceProperties& properties() const{ return _properties; }
public:
	const std::vector<VulkanQueueFamily*>& queueFamilies() const{ return _queueFamilies; }
	VulkanQueueFamily* findComputeQueueFamily() const;
	VulkanQueueFamily* getComputeQueueFamily() const;
	VulkanQueueFamily* getGraphicsQueueFamily() const;
	VulkanQueueFamily* findGraphicsQueueFamily() const;
	bool hasGraphicsQueue() const;
public:
	VulkanQueueFamily* getPresentQueueFamily( const VulkanSurface& surface ) const;
	VulkanQueueFamily* findPresentQueueFamily( const VulkanSurface& surface ) const;
	bool hasPresentQueue( const VulkanSurface& surface ) const;
public:
	bool isExtensionAvailable( const std::string& extension ) const;
public:
//	const VkPhysicalDeviceFeatures& supportedFeatures() const{ return _supportedFeatures; }
public:
	uint32_t getMemoryTypeIndex( uint32_t typeFilter, VkMemoryPropertyFlags properties ) const;
public:
	VkFormat getSupportedFormat( const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features );
public:
	void getSwapchainSupport( const VulkanSurface& surface, VulkanSwapchainSupportDetails& support );
public:
	void trace() const;
};

