#pragma once

#include "vulkan/VulkanCore.h"
#include "VulkanRequirement.h"
#include "VulkanDeviceExtension.h"
#include "VulkanQueryPool.h"

class VulkanDevice // : public Singleton
{
public:
	static const char* Id;
private:
	VulkanInstance* _instance;
private:
	List<VulkanDeviceExtension*> _extensions;
private:
	VkDevice _vkDevice;
private:
	VulkanPhysicalDevice* _physicalDevice;
private:
	List<VulkanQueue*> _queues;
private:
	List<VulkanDeviceObject*> _objectsToDestroy;
#ifdef ODTDEBUG
private:
	List<VulkanQueryPool*> _performancecounters;
#endif
public:
	VulkanDevice( VulkanInstance* instance );
	~VulkanDevice();
public:
	VkDevice vkDevice() const{ assert( _vkDevice ); return _vkDevice; }
public:
	void create( VulkanRequirements& requirements );
	void destroy();
public:
	bool isCreated() const{ return _vkDevice != nullptr; }
public:
	VulkanPhysicalDevice* physicalDevice() const{ assert( _physicalDevice ); return _physicalDevice; };
public:
	const List<VulkanDeviceExtension*>& extensions() const{ return _extensions; }
	template< typename T>
	T* getExtension() const{
		for( VulkanDeviceExtension* extension : _extensions ){
			if( extension->name() == T::Name )
				return (T*) extension;
		}
		assert( false );
	}
public:
	VkFormat getSupportedFormat( const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features );
public:
	void getSwapchainSupport( const VulkanSurface& surface, VulkanSwapchainSupportDetails& support );
public:
    PFN_vkVoidFunction getProcAddr( const char* name );
	VulkanQueue* getQueue( VulkanQueueFamily* queueFamily );
	VulkanQueue* getComputeQueue();
	VulkanQueue* getGraphicsQueue();
	VulkanQueue* getPresentQueue( VulkanSurface& surface );
public:
	void addObjectToDestroy( VulkanDeviceObject* object );
public:
#ifdef ODTDEBUG
	VulkanQueryPool* createPerformanceCounter();
	void destroyPerformanceCounter( VulkanQueryPool* performancecounter );
#endif
public:
	void waitIdle();
public:
	void onIdle();
//public:
//	VulkanQueueFamily* getGraphicsQueueFamily() const;
//	VulkanQueueFamily* findGraphicsQueueFamily() const;
//	bool hasGraphicsQueue() const;
//public:
//	VulkanQueueFamily* getPresentQueueFamily( const VulkanSurface& surface ) const;
//	VulkanQueueFamily* findPresentQueueFamily( const VulkanSurface& surface ) const;
//	bool hasPresentQueue( const VulkanSurface& surface ) const;
//public:
//	bool isExtensionAvailable( const std::string& extension ) const;
//public:
//	const VkPhysicalDeviceFeatures& supportedFeatures() const{ return _supportedFeatures; }
//public:
//	uint32_t getMemoryTypeIndex( uint32_t typeFilter, VkMemoryPropertyFlags properties ) const;
};

