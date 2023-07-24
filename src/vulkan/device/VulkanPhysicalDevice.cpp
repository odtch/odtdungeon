#include "stdodt.h"
#include "VulkanPhysicalDevice.h"
#include "vulkan/present/VulkanSurface.h"
#include "vulkan/present/VulkanSwapchain.h"

VulkanPhysicalDevice::VulkanPhysicalDevice(VkPhysicalDevice vkDevice)
	:_vkDevice( vkDevice )
{
	{
		assert( _vkDevice != nullptr );
		memset( &_properties, 0, sizeof( _properties ) );
		vkGetPhysicalDeviceProperties( _vkDevice, &_properties );
//		std::cout << "maxPushConstantsSize = " << _properties.limits.maxPushConstantsSize << "\n";
//		memset( &_supportedFeatures, 0, sizeof( _supportedFeatures ) );
//		memset( &_addressFeatures, 0, sizeof( _addressFeatures ) );
//		_addressFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_ADDRESS_FEATURES_EXT;
//		memset( &_supportedFeatures2, 0, sizeof( _supportedFeatures2 ) );
//		_supportedFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
//		_supportedFeatures2.pNext = &_addressFeatures;
	}{
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties( _vkDevice, &queueFamilyCount, nullptr );
		std::vector<VkQueueFamilyProperties> queueFamilies( queueFamilyCount );
		vkGetPhysicalDeviceQueueFamilyProperties( _vkDevice, &queueFamilyCount, queueFamilies.data() );
		int i = 0;
		for( const auto& queueFamilyProperty : queueFamilies ){
			VulkanQueueFamily* queueFamily = new VulkanQueueFamily( i, queueFamilyProperty, this );
			_queueFamilies.push_back( queueFamily );
//			if( queueFamilyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT ){
//				if( !_hasGraphicsQueue ){
//					_graphicsQueueIndex = i;
//					_hasGraphicsQueue = true;
//				}
//			}
//				// indices.graphicsFamily = i;
//			}
//			VkBool32 presentSupport = false;
//			vkGetPhysicalDeviceSurfaceSupportKHR( _vkDevice, i, surface.vkSurface(),&presentSupport );
//			if( presentSupport ){
//				indices.presentFamily = i;
//			}
			i++;
		}
	}{
		uint32_t extensionCount;
		Vulkan::check( vkEnumerateDeviceExtensionProperties( _vkDevice, nullptr, &extensionCount, nullptr ), "vkEnumerateDeviceExtensionProperties count failed" );
		_availableExtensions = std::vector<VkExtensionProperties>( extensionCount);
		Vulkan::check( vkEnumerateDeviceExtensionProperties( _vkDevice, nullptr, &extensionCount, _availableExtensions.data() ), "vkEnumerateDeviceExtensionProperties get failed" );
//		if( Vulkan::debuggingEnabled ){
//			std::cout << "available ddeviceext\n";
//			for (const auto& extension : _availableExtensions) {
//				std::cout << "\t" << extension.extensionName << "\n";
//			}
//		}
	}{
//		vkGetPhysicalDeviceFeatures( _vkDevice, &_supportedFeatures );
//		vkGetPhysicalDeviceFeatures2( _vkDevice, &_supportedFeatures2 );
	}
//	std::cout << "largePoints: " << _supportedFeatures.largePoints << "\n";
//	assert( _supportedFeatures.largePoints );
//	std::cout << "pointSize limits " << _properties.limits.pointSizeGranularity
//			  << ", " << _properties.limits.pointSizeRange[0]
//			  << ", " << _properties.limits.pointSizeRange[1] << "\n";
//	std::cout.flush();
}
VulkanPhysicalDevice::~VulkanPhysicalDevice(){
	_vkDevice = nullptr;
}
VulkanQueueFamily* VulkanPhysicalDevice::findComputeQueueFamily() const{
	for( auto family : _queueFamilies ){
		if( family->isCompute() )
			return family;
	}
	return nullptr;
}
VulkanQueueFamily* VulkanPhysicalDevice::getComputeQueueFamily() const {
	VulkanQueueFamily* queueFamily = findComputeQueueFamily();
	if( queueFamily == null )
		throw std::runtime_error( "no compute queue family" );
	return queueFamily;
}
VulkanQueueFamily* VulkanPhysicalDevice::getGraphicsQueueFamily() const{
	VulkanQueueFamily* queue = findGraphicsQueueFamily();
	if( queue == nullptr )
		throw std::runtime_error( "no graphics queue family" );
	return queue;
}
VulkanQueueFamily* VulkanPhysicalDevice::findGraphicsQueueFamily() const {
	for( auto family : _queueFamilies ){
		if( family->isGraphics() )
			return family;
	}
	return nullptr;
}
bool VulkanPhysicalDevice::hasGraphicsQueue() const {
	return nullptr != findGraphicsQueueFamily();
}
VulkanQueueFamily* VulkanPhysicalDevice::findPresentQueueFamily( const VulkanSurface& surface ) const {
	for( auto family : _queueFamilies ){
		if( family->isPresent( surface ) )
			return family;
	}
	return nullptr;
}
VulkanQueueFamily* VulkanPhysicalDevice::getPresentQueueFamily( const VulkanSurface& surface ) const {
	VulkanQueueFamily* queue = findPresentQueueFamily( surface );
	if( queue == nullptr )
		throw std::runtime_error( "no present queue family" );
	return queue;
}
//bool VulkanPhysicalDevice::hasPresentQueue( const VulkanSurface& surface ) const {
//	return nullptr != findPresentQueueFamily( surface );
//}
bool VulkanPhysicalDevice::isExtensionAvailable( const std::string& extension ) const {
	for( const auto& availableExtension : _availableExtensions ){
		if( 0 == extension.compare( availableExtension.extensionName ) )
			return true;
	}
	return false;
}
void VulkanPhysicalDevice::trace() const {
	std::cout << "VulkanPhysicalDevice " << name() << "\t" << _properties.deviceType << "\n";
	for( auto queueFamily : _queueFamilies ){
		std::cout << "\tQueueFamily " << queueFamily->index() << "\t"
				  << ( queueFamily->isGraphics() ? "graphics" : "" ) << '\t'
				  << ( queueFamily->isCompute() ? "compute" : "" ) << '\t'
				  // isPresent( const VulkanSurface& surface ) const;
				  << '\n';
	}
	std::cout << "\tavailableExtensions\n";
	for( const VkExtensionProperties& extension : _availableExtensions ){
		std::cout << "\t\t" << extension.extensionName << '\t' << extension.specVersion << '\n';
	}
}
void traceMemoryPropertyFlags( const VkMemoryPropertyFlags& properties ){
	if( properties & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT )std::cout << "VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT ";
	if( properties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )std::cout << "VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ";
	if( properties & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT )std::cout << "VK_MEMORY_PROPERTY_HOST_COHERENT_BIT ";
	if( properties & VK_MEMORY_PROPERTY_HOST_CACHED_BIT )std::cout << "VK_MEMORY_PROPERTY_HOST_CACHED_BIT ";
	if( properties & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT )std::cout << "VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT ";
	if( properties & VK_MEMORY_PROPERTY_PROTECTED_BIT )std::cout << "VK_MEMORY_PROPERTY_PROTECTED_BIT ";
	if( properties & VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD )std::cout << "VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD ";
	if( properties & VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD )std::cout << "VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD ";
}
uint32_t VulkanPhysicalDevice::getMemoryTypeIndex( uint32_t typeFilter, VkMemoryPropertyFlags properties ) const {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties( vkDevice(), &memProperties );
	for( uint32_t i = 0; i < memProperties.memoryTypeCount; i++ ){
		if( ( typeFilter & (1 << i) ) && ( memProperties.memoryTypes[i].propertyFlags & properties ) == properties ){
			return i;
		}
	}
	std::cout << "VulkanPhyiscalDevice::getMemoryTypeIndex " << name() << "\n";
	std::cout << " requested \t" << typeFilter << "\t";
	traceMemoryPropertyFlags( properties );
	std::cout << "\n";
	for( uint32_t i = 0; i < memProperties.memoryTypeCount; i++ ){
		std::cout << " " << i << "\t" << ( typeFilter & (1 << i) ) << "\t";
		traceMemoryPropertyFlags( memProperties.memoryTypes[i].propertyFlags );
		std::cout << "\n";
	}
	throw std::runtime_error( "failed to find suitable memory type" );
}
VkFormat VulkanPhysicalDevice::getSupportedFormat( const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features ){
	for( VkFormat format : candidates ){
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties( vkDevice(), format, &props );
		if( tiling == VK_IMAGE_TILING_LINEAR && ( props.linearTilingFeatures & features ) == features ){
			return format;
		} else if( tiling == VK_IMAGE_TILING_OPTIMAL && ( props.optimalTilingFeatures & features ) == features ){
			return format;
		}
	}
	throw std::runtime_error( "VulkanPhysicalDevice format not supported" );
}
void VulkanPhysicalDevice::getSwapchainSupport( const VulkanSurface& surface, VulkanSwapchainSupportDetails& details ){
	assert( details.formats.size() == 0 );
	Vulkan::check( vkGetPhysicalDeviceSurfaceCapabilitiesKHR( vkDevice(), surface.vkSurface(), &details.capabilities ), "vkGetPhysicalDeviceSurfaceCapabilitiesKHR failed" );
	uint32_t formatCount = 0;
	Vulkan::check( vkGetPhysicalDeviceSurfaceFormatsKHR( vkDevice(), surface.vkSurface(),&formatCount, nullptr ), "vkGetPhysicalDeviceSurfaceFormatsKHR count failed" );
	if (formatCount != 0) {
		details.formats.resize(formatCount);
		Vulkan::check( vkGetPhysicalDeviceSurfaceFormatsKHR( vkDevice(), surface.vkSurface(),&formatCount, details.formats.data() ), "vkGetPhysicalDeviceSurfaceFormatsKHR get failed" );
	}
	uint32_t presentModeCount = 0;
	Vulkan::check( vkGetPhysicalDeviceSurfacePresentModesKHR( vkDevice(), surface.vkSurface(),&presentModeCount, nullptr ), "vkGetPhysicalDeviceSurfacePresentModesKHR count failed" );
	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		Vulkan::check( vkGetPhysicalDeviceSurfacePresentModesKHR( vkDevice(), surface.vkSurface(),&presentModeCount, details.presentModes.data() ), "vkGetPhysicalDeviceSurfacePresentModesKHR get failed" );
	}
}

VulkanQueueFamily::VulkanQueueFamily(uint32_t index, const VkQueueFamilyProperties& properties, VulkanPhysicalDevice* device )
	:_device( device )
	,_index( index )
	,_properties( properties )
{
	assert( _device );
}
VulkanQueueFamily::~VulkanQueueFamily(){
	_device = nullptr;
}
bool VulkanQueueFamily::isCompute() const {
	return _properties.queueFlags & VK_QUEUE_COMPUTE_BIT;
}
bool VulkanQueueFamily::isGraphics() const {
	return _properties.queueFlags & VK_QUEUE_GRAPHICS_BIT;
}
bool VulkanQueueFamily::isPresent(const VulkanSurface& surface) const {
	VkBool32 presentSupport = false;
	Vulkan::check( vkGetPhysicalDeviceSurfaceSupportKHR( _device->vkDevice(), index(), surface.vkSurface(), &presentSupport ), "vkGetPhysicalDeviceSurfaceSupportKHR failed" );
	if( presentSupport ){
		return true;
	}
	return false;
}
