#include "stdodt.h"
#include "VulkanInstance.h"
#include "VulkanRequirement.h"
#include "VulkanPhysicalDevice.h"

VulkanInstanceGlobals::VulkanInstanceGlobals(){
	loadAvailableLayers();
	loadAvailableExtensions();
}
VulkanInstanceGlobals::~VulkanInstanceGlobals(){
}
void VulkanInstanceGlobals::loadAvailableLayers(){
	assert( _availableLayers.size() == 0 );
	uint32_t layerCount = 0;
	VkResult result = vkEnumerateInstanceLayerProperties( &layerCount, nullptr );
	if( result != VK_SUCCESS ){
		throw std::runtime_error( "vkEnumerateInstanceLayerProperties count failed" );
	}
	_availableLayers.resize( layerCount );
	result = vkEnumerateInstanceLayerProperties( &layerCount, _availableLayers.data() );
	if( result != VK_SUCCESS ){
		throw std::runtime_error( "vkEnumerateInstanceLayerProperties get failed" );
	}
}
bool VulkanInstanceGlobals::isLayerAvailable( const char *layerName ) const {
	assert( layerName );
	for( const auto& layerProperties : availableLayers() ){
		if( strcmp( layerName, layerProperties.layerName ) == 0 ){
			return true;
		}
	}
	return false;
}
void VulkanInstanceGlobals::traceAvailableLayers(){
	std::cout << "VulkanInstance: available layers:\n";
	for( const VkLayerProperties& layer : availableLayers() ){
		std::cout << '\t' << layer.layerName << "\t" << layer.specVersion << "\t" << layer.implementationVersion << "\t" << layer.description << '\n';
	}
}
void VulkanInstanceGlobals::loadAvailableExtensions(){
	assert( _availableExtensions.size() == 0 );
	uint32_t extensionCount = 0;
	VkResult result = vkEnumerateInstanceExtensionProperties( nullptr, &extensionCount, nullptr );
	if( result != VK_SUCCESS ){
		throw std::runtime_error( "vkEnumerateInstanceExtensionProperties count failed" );
	}
	_availableExtensions.resize( extensionCount );
	result = vkEnumerateInstanceExtensionProperties( nullptr, &extensionCount, _availableExtensions.data() );
	if( result != VK_SUCCESS ){
		throw std::runtime_error( "vkEnumerateInstanceExtensionProperties get failed" );
	}
}
bool VulkanInstanceGlobals::isExtensionAvailable( const char* extensionName ) const {
	assert( extensionName );
	for( const auto& extension: availableExtensions() ){
		if( strcmp( extensionName, extension.extensionName ) == 0 ){
			return true;
		}
	}
	return false;
}
void VulkanInstanceGlobals::traceAvailableExtensions(){
	std::cout << "VulkanInstance: available extensions:\n";
	for (const auto& extension : _availableExtensions) {
		std::cout << '\t' << extension.extensionName << '\t' << extension.specVersion << '\n';
	}
}


VulkanInstance::VulkanInstance()
	:_instance( nullptr )
	,_debugMessenger( nullptr )
{
}
VulkanInstance::~VulkanInstance(){
	destroy();
	ASSERT( _instance == nullptr );
	ASSERT( _physicalDevices.size() == 0 );
}
void VulkanInstance::create( const char* applicationName, const VulkanRequirements& requirements ){
	assert( applicationName );
	uint32_t apiVersion = VK_API_VERSION_1_0;
	std::vector<const char*> requiredLayers;
	std::vector<const char*> requiredExtensions;
	for( VulkanRequirement* requirement : requirements ){
		if( requirement->isApiVersion() ){
			apiVersion = ( apiVersion < requirement->asApiVersion()->apiVersion() ? requirement->asApiVersion()->apiVersion() : apiVersion );
		}
		if( requirement->isInstanceLayer() ){
			for( auto existing : requiredLayers ){
				if( 0 == strcmp( existing, requirement->asInstanceLayer()->name() ) ){
					throw std::runtime_error( "layer already required" );
				}
			}
			requiredLayers.push_back( requirement->asInstanceLayer()->name() );
		}
		if( requirement->isInstanceExtension() ){
			for( auto existing : requiredExtensions ){
				if( 0 == strcmp( existing, requirement->asInstanceExtension()->name() ) ){
					throw std::runtime_error( "extension already required" );
				}
			}
			requiredExtensions.push_back( requirement->asInstanceExtension()->name() );
		}
	}
	{
		VulkanInstanceGlobals globals;
		for( const char* layer : requiredLayers ){
			if( !globals.isLayerAvailable( layer ) ){
				globals.traceAvailableLayers();
				std::cout << "VulkanInstance requiredLayer not available " << layer << '\n';
				throw std::runtime_error( "VulkanInstance-Layer not available" );
			}
		}
		for( const char* extension : requiredExtensions ){
			if( !globals.isExtensionAvailable( extension ) ){
				globals.traceAvailableExtensions();
				std::cout << "VulkanInstance requiredExtension not available " << extension << '\n';
				throw std::runtime_error( "VulkanInstance-Extension not available" );
			}
		}
	}
	assert( _instance == nullptr );
	{
		VkApplicationInfo appInfo;
		memset( &appInfo, 0, sizeof( appInfo ) );
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = applicationName;
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "odt.ch";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = apiVersion;
		VkInstanceCreateInfo vkCreateInfo{};
		vkCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		vkCreateInfo.pApplicationInfo = &appInfo;
		vkCreateInfo.enabledExtensionCount = static_cast<uint32_t>( requiredExtensions.size() );
		vkCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();
		vkCreateInfo.enabledLayerCount = static_cast<uint32_t>( requiredLayers.size() );
		vkCreateInfo.ppEnabledLayerNames = requiredLayers.data();
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		if( Vulkan::debuggingEnabled ){
			Vulkan::populateDebugMessengerCreateInfo( debugCreateInfo );
			vkCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
		} else {
			vkCreateInfo.enabledLayerCount = 0;

			vkCreateInfo.pNext = nullptr;
		}
		Vulkan::check( vkCreateInstance( &vkCreateInfo, nullptr, &_instance ), "vkCreateInstance failed" );
		assert( _instance );
	}
	if( Vulkan::debuggingEnabled ){
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		Vulkan::populateDebugMessengerCreateInfo( createInfo );
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT) getInstanceProcAddr( "vkCreateDebugUtilsMessengerEXT" );
		assert( func );
		VkResult result = func( _instance, &createInfo, nullptr, &_debugMessenger );
		if( !Vulkan::isSuccess( result ) ){
			throw std::runtime_error( "failed to set up debug messenger" );
		}
	}
	{
		uint32_t deviceCount = 0;
		VkResult result = vkEnumeratePhysicalDevices( _instance, &deviceCount, nullptr);
		Vulkan::check( result, "vkEnumeratePhysicalDevices count failed" );
		if( deviceCount == 0 ){
			throw std::runtime_error( "no physical device");
		}
		std::vector<VkPhysicalDevice> devices( deviceCount );
		result = vkEnumeratePhysicalDevices( _instance, &deviceCount, devices.data() );
		Vulkan::check( result, "vkEnumeratePhysicalDevices get failed" );
		for( const auto& device_handle : devices ){
			_physicalDevices.push_back( new VulkanPhysicalDevice( device_handle ) );
		}
	}
}
void VulkanInstance::destroy(){
	for( auto device : _physicalDevices ){
		delete device;
	}
	_physicalDevices.clear();
	if( _instance ){
		if( _debugMessenger != nullptr ){
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) getInstanceProcAddr( "vkDestroyDebugUtilsMessengerEXT" );
			assert( func );
			func( _instance, _debugMessenger, nullptr );
			_debugMessenger = nullptr;
		}
		vkDestroyInstance( _instance, nullptr );
		_instance = nullptr;
	}
}
PFN_vkVoidFunction VulkanInstance::getInstanceProcAddr( const char* name ){
	assert( name );
	PFN_vkVoidFunction func = vkGetInstanceProcAddr( _instance, name );
	if( func == nullptr ){
		std::cout << "VulkanInstance::getInstanceProcAddr failed" << name << '\n';
		throw std::runtime_error( "failed to get instance ProcAddr" );
	}
	return func;
}


