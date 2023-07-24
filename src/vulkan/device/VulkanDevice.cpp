#include "stdodt.h"
#include "VulkanDevice.h"
#include "VulkanInstance.h"
#include "VulkanDeviceExtension.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanDeviceObject.h"
#include "vulkan/command/VulkanQueue.h"

const char* VulkanDevice::Id = "VulkanDevice";

VulkanDevice::VulkanDevice( VulkanInstance* instance )
	://Singleton( Id )
	//,
	  _instance( instance )
	,_vkDevice( nullptr )
	,_physicalDevice( nullptr )
{
	assert( _instance );
}
VulkanDevice::~VulkanDevice(){
	//logDebug( "~VD1" );
	destroy();
	//logDebug( "~VD2" );
	ASSERT( _queues.isEmpty() );
	ASSERT( _physicalDevice == null );
	ASSERT( _vkDevice == null );
	ASSERT( _instance == null );
	//logDebug( "~VD3" );
}
void VulkanDevice::create( VulkanRequirements& requirements ){
	if( _vkDevice != nullptr || _physicalDevice != nullptr )
		throw std::runtime_error( "VulkanDevice already created" );
	assert( _extensions.isEmpty() );
	for( VulkanRequirement* requirement : requirements ){
		if( requirement->isTypedDeviceExtension() ){
			_extensions.add( requirement->createDeviceExtension() );
		}
	}
	for( VulkanDeviceExtension* extension : _extensions ){
		extension->installRequirements( requirements );
	}
	std::vector<const char*> requiredExtensions;
	List<VulkanQueueFamily*> requiredQueueFamilies;
	for( VulkanRequirement* requirement : requirements ){
		if( requirement->isDeviceExtension() ){
			requiredExtensions.push_back( requirement->asDeviceExtension()->name() );
		}
	}
	for( VulkanPhysicalDevice* physicalDevice : _instance->physicalDevices() ) {
		_physicalDevice = physicalDevice;
		try{
			for( VulkanDeviceExtension* extension : _extensions ){
				if( !physicalDevice->isExtensionAvailable( extension->name() ) )
					throw std::runtime_error( std::string( "extension not available " ) + std::string( extension->name() ) );
			}
			{
				VkPhysicalDeviceFeatures2 _supportedFeatures2;
				memset( &_supportedFeatures2, 0, sizeof( _supportedFeatures2 ) );
				_supportedFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
				VulkanUntypedStructure* prev_feature = null;
				for( VulkanDeviceExtension* extension : _extensions ){
					if( extension->hasFeature() ){
						VulkanUntypedStructure* feature = (VulkanUntypedStructure*) extension->resetFeature();
						assert( feature );
						assert( feature->sType );
						assert( feature->pNext == null );
						if( prev_feature == null ){
							assert( _supportedFeatures2.pNext == null );
							_supportedFeatures2.pNext = feature;
						} else {
							prev_feature->pNext = feature;
						}
						prev_feature = feature;
					}
				}
				vkGetPhysicalDeviceFeatures2( physicalDevice->vkDevice(), &_supportedFeatures2 );
	//			for( VulkanDeviceRequirement* requirement : createInfo.requirements() ){
	//				requirement->checkSupportedFeatures( _supportedFeatures2.features );
	//			}
				for( VulkanDeviceExtension* extension : _extensions ){
					extension->checkSupportedFeatures( _supportedFeatures2.features );
				}
			}
			requiredQueueFamilies.removeAll();
			for( VulkanRequirement* requirement : requirements ){
				if( requirement->isQueue() ){
					VulkanQueueFamily* queueFamily = requirement->asQueue()->getQueueFamily( physicalDevice );
					assert( queueFamily );
					if( !requiredQueueFamilies.contains( queueFamily ) ){
						requiredQueueFamilies.add( queueFamily );
					}
				}
			}
			std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
			queueCreateInfos.reserve( requiredQueueFamilies.size() );
			float queuePriority = 1.0f;
			for( VulkanQueueFamily* queueFamily : requiredQueueFamilies ){
				VkDeviceQueueCreateInfo queueCreateInfo{};
				queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueCreateInfo.queueFamilyIndex = queueFamily->index();
				queueCreateInfo.queueCount = 1;
				queueCreateInfo.pQueuePriorities = &queuePriority;
				queueCreateInfos.push_back( queueCreateInfo );
			}
			if( queueCreateInfos.empty() )
				throw std::runtime_error( "no device-queue to create" );			
			VkPhysicalDeviceFeatures2 requiredDeviceFeatures;
			memset( &requiredDeviceFeatures, 0, sizeof( requiredDeviceFeatures ) );
			requiredDeviceFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
			requiredDeviceFeatures.features.samplerAnisotropy = VK_TRUE;
			VulkanUntypedStructure* prev_feature = null;
			for( VulkanRequirement* requirement : requirements ){
				if( requirement->isDeviceFeature() ){
					VkStructureType type = requirement->asDeviceFeature()->type();
					VulkanUntypedStructure* features = (VulkanUntypedStructure*) requirement->asDeviceFeature()->untypedFeatures();
					assert( features );
					assert( features->sType == type );
					assert( features->pNext == null );
					if( prev_feature == null ){
						assert( requiredDeviceFeatures.pNext == null );
						requiredDeviceFeatures.pNext = features;
					} else {
						assert( prev_feature->pNext == null );
						prev_feature->pNext = features;
					}
					prev_feature = features;
				}
			}

			VkPhysicalDeviceHostQueryResetFeatures resetFeatures;
			memset( &resetFeatures, 0, sizeof( resetFeatures ) );
			resetFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES;
			resetFeatures.hostQueryReset = VK_TRUE;
			assert( prev_feature );
			prev_feature->pNext = &resetFeatures;
			//resetFeatures.pNext = prev_feature;
			prev_feature = (VulkanUntypedStructure*) &resetFeatures;

			////	_requiredDeviceFeatures.features.largePoints = VK_TRUE;

			VkDeviceCreateInfo createInfo;
			memset( &createInfo, 0, sizeof( createInfo ) );
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			createInfo.queueCreateInfoCount = static_cast<uint32_t>( queueCreateInfos.size() );
			createInfo.pQueueCreateInfos = queueCreateInfos.data();
			createInfo.enabledExtensionCount = static_cast<uint32_t>( requiredExtensions.size() );
			createInfo.ppEnabledExtensionNames = requiredExtensions.data();
			//createInfo.pEnabledFeatures = &requiredDeviceFeatures;
			createInfo.pNext = &requiredDeviceFeatures;
		//	if( Vulkan::debuggingEnabled ){
		//		std::vector<const char*> validationLayers;
		//		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		//		createInfo.ppEnabledLayerNames = validationLayers.data();
		//	} else {
		//		createInfo.enabledLayerCount = 0;
		//	}
			assert( _physicalDevice );

//			{
//				VulkanUntypedStructure* f = (VulkanUntypedStructure*) &createInfo;
//				while( f ){
//					logDebug( "VDC ", f->sType, ( f->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES ? "HostQueryReset" : "" ) );
//					f = (VulkanUntypedStructure*) f->pNext;
//				}
//			}

			Vulkan::check( vkCreateDevice( _physicalDevice->vkDevice(), &createInfo, nullptr, &_vkDevice ), "failed to create logical device" );
			assert( _vkDevice );
			for( VulkanQueueFamily* queueFamily : requiredQueueFamilies ){
				VkQueue vkQueue = nullptr;
				vkGetDeviceQueue( vkDevice(), queueFamily->index(), 0, &vkQueue );
				if( vkQueue == nullptr )
					throw std::runtime_error( "vkGetDeviceQueue failed" );
				VulkanQueue* queue = new VulkanQueue( vkQueue, queueFamily, this );
				_queues.add( queue );
			}
			requiredQueueFamilies.removeAll();
			for( VulkanDeviceExtension* extension : _extensions ){
				extension->bindDevice( this );
			}
			return;
		} catch( const std::exception& ex ){
			_physicalDevice = null;
			std::cout << "VulkanDevice " << physicalDevice->name() << " not suitable: " << ex.what() << "\n";
		}
		requiredQueueFamilies.removeAll();
	}
	for( const auto& device : _instance->physicalDevices() ) {
		device->trace();
	}
	throw std::runtime_error( "failed to find a suitable physical device" );
}
void VulkanDevice::destroy(){
	while( !_objectsToDestroy.isEmpty() ){
		VulkanDeviceObject* object = _objectsToDestroy.takeLast();
		object->destroy();
		delete object;
	}
	_extensions.deleteAll();
	_queues.deleteAll();
#ifdef ODTDEBUG
	while( !_performancecounters.isEmpty() ){
		auto pc = _performancecounters.takeFirst();
		pc->destroy();
		odelete( pc );
	}
#endif
	if( _vkDevice ){
		vkDestroyDevice( _vkDevice, nullptr );
		_vkDevice = null;
	}
	_physicalDevice = nullptr;
	_instance = null;
}
VkFormat VulkanDevice::getSupportedFormat( const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features ){
	return physicalDevice()->getSupportedFormat( candidates, tiling, features );
}
void VulkanDevice::getSwapchainSupport( const VulkanSurface& surface, VulkanSwapchainSupportDetails& support ){
	return physicalDevice()->getSwapchainSupport( surface, support );
}
PFN_vkVoidFunction VulkanDevice::getProcAddr( const char* name ){
    assert( name );
    PFN_vkVoidFunction result = vkGetDeviceProcAddr( vkDevice(), name );
    if( result == null ){
        logError( "vkGetDeviceProcAddr", name );
        assert( false );
    }
    return result;
}
VulkanQueue* VulkanDevice::getQueue( VulkanQueueFamily* queueFamily ){
	assert( queueFamily );
	for( auto queue : _queues ){
		if( queue->family() == queueFamily )
			return queue;
	}
	throw std::runtime_error( "VulkanQueue for family not found" );
}
VulkanQueue *VulkanDevice::getComputeQueue(){
	return getQueue( physicalDevice()->getComputeQueueFamily() );
}
VulkanQueue *VulkanDevice::getGraphicsQueue(){
	return getQueue( physicalDevice()->getGraphicsQueueFamily() );
}
VulkanQueue *VulkanDevice::getPresentQueue( VulkanSurface& surface ){
	return getQueue( physicalDevice()->getPresentQueueFamily( surface ) );
}
void VulkanDevice::addObjectToDestroy( VulkanDeviceObject* object ){
	_objectsToDestroy.add( object );
}
#ifdef ODTDEBUG
VulkanQueryPool* VulkanDevice::createPerformanceCounter(){
	if( !_performancecounters.isEmpty() ){
		auto pc = _performancecounters.takeFirst();
		pc->reset();
		return pc;
	}
	auto pc = new VulkanQueryPool();
	pc->create( 2, this );
	return pc;
}
void VulkanDevice::destroyPerformanceCounter( VulkanQueryPool* performancecounter ){
	assert( performancecounter );
	_performancecounters.add( performancecounter );
}
#endif
void VulkanDevice::waitIdle(){
	//logDebug( "VulkanDevice::waitIdle" );
	Vulkan::check( vkDeviceWaitIdle( vkDevice() ), "vkDeviceWaitIdle failed" );
	for( auto queue : _queues ){
		queue->waitIdle();
	}
}
void VulkanDevice::onIdle(){
	for( auto queue : _queues ){
		queue->onIdle();
	}
}

//VulkanDeviceCreateInfo::VulkanDeviceCreateInfo(){
//}
//VulkanDeviceCreateInfo::~VulkanDeviceCreateInfo(){
//	_requirements.deleteAll();
//}
//void VulkanDeviceCreateInfo::addRequirement( VulkanDeviceRequirement* requirement ){
//	assert( requirement );
//	_requirements.add( requirement );
//}
//void VulkanDeviceCreateInfo::requireExtension( const char* extension ){
//	assert( extension );
//	addRequirement( new VulkanNamedExtensionDeviceRequirement( extension ) );
//}
////void VulkanDeviceCreateInfo::requireExtension( const char* extension ){
////	assert( extension  );
////	for( auto existing : _requiredExtensions ){
////		if( 0 == strcmp( existing, extension  ) ){
////			throw std::runtime_error( "extension already required" );
////		}
////	}
////	_requiredExtensions.push_back( extension  );
////}
////void VulkanDeviceCreateInfo::requireFeatures( VulkanDeviceRequirement* featureRequirement ){
////	assert( featureRequirement );
////	_requirements.push_back( featureRequirement );
////}
////bool VulkanDeviceCreateInfo::isDeviceSuitable(VulkanPhysicalDevice *device) const {
////	for( auto extension : _requiredExtensions ){
////		if( !device->isExtensionAvailable( extension ) )
////			return false;
////	}
////////		std::cout << "fea?" << device->supportedFeatures().features.robustBufferAccess
//////		for( VulkanQueue* queue : _queues ){
//////			if( !queue->isPhysicalDeviceSupported( physicalDevice ) )
//////				return false;
//////		}
////	return true;

////}

////VulkanDeviceRequirement::VulkanDeviceRequirement(VkStructureType type)
////	:_type( type )
////{
////	assert( _type );
////}
////VulkanDeviceRequirement::~VulkanDeviceRequirement(){
////}

