#include "stdodt.h"
#include "VulkanRequirement.h"
#include "vulkan/device/VulkanPhysicalDevice.h"

VulkanRequirement::VulkanRequirement(){
}
VulkanRequirement::~VulkanRequirement(){
}

VulkanNamedRequirement::VulkanNamedRequirement( const char* name )
	:_name( name )
{
	assert( _name );
}
VulkanNamedRequirement::~VulkanNamedRequirement(){
}

VulkanInstanceLayerRequirement::VulkanInstanceLayerRequirement( const char* name )
	:VulkanNamedRequirement( name )
{
}
VulkanInstanceLayerRequirement::~VulkanInstanceLayerRequirement(){
}

VulkanInstanceExtensionRequirement::VulkanInstanceExtensionRequirement( const char* name )
	:VulkanNamedRequirement( name )
{
}
VulkanInstanceExtensionRequirement::~VulkanInstanceExtensionRequirement(){
}

VulkanApiVersionRequirement::VulkanApiVersionRequirement(uint32_t apiVersion)
	:_apiVersion( apiVersion )
{
	assert( VK_API_VERSION_1_0 <= apiVersion );
}
VulkanApiVersionRequirement::~VulkanApiVersionRequirement(){
}

VulkanDeviceFeatureRequirement::VulkanDeviceFeatureRequirement(VkStructureType type)
	:_type( type )
{
	assert( type );
}
VulkanDeviceFeatureRequirement::~VulkanDeviceFeatureRequirement(){
}

VulkanDeviceExtensionRequirement::VulkanDeviceExtensionRequirement( const char* name )
	:VulkanNamedRequirement( name )
{
}
VulkanDeviceExtensionRequirement::~VulkanDeviceExtensionRequirement(){
}

VulkanQueueRequirement::VulkanQueueRequirement(){
}
VulkanQueueRequirement::~VulkanQueueRequirement(){
}

VulkanComputeQueueRequirement::VulkanComputeQueueRequirement(){
}
VulkanComputeQueueRequirement::~VulkanComputeQueueRequirement(){
}
VulkanQueueFamily *VulkanComputeQueueRequirement::getQueueFamily( VulkanPhysicalDevice* physicalDevice ) const {
	assert( physicalDevice );
	return physicalDevice->getComputeQueueFamily();
}


VulkanPresentQueueRequirement::VulkanPresentQueueRequirement(VulkanSurface *surface)
	:_surface( surface )
{
	assert( _surface );
}
VulkanPresentQueueRequirement::~VulkanPresentQueueRequirement(){
}
VulkanQueueFamily *VulkanPresentQueueRequirement::getQueueFamily(VulkanPhysicalDevice *physicalDevice) const {
	assert( physicalDevice );
	return physicalDevice->getPresentQueueFamily( *_surface );
}
VulkanRequirements::VulkanRequirements(){
	if( Vulkan::debuggingEnabled ){
		requireInstanceLayer( "VK_LAYER_KHRONOS_validation" );
		requireInstanceExtension( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
	}
}
VulkanRequirements::~VulkanRequirements(){
	_requirements.deleteAll();
}
void VulkanRequirements::add( VulkanRequirement* requirement ){
	assert( requirement );
	_requirements.add( requirement );
}
void VulkanRequirements::requireInstanceLayer( const char *name ){
    logDebug( "VulkanRequirements::requireInstanceLayer", name );
	add( new VulkanInstanceLayerRequirement( name ) );
}
void VulkanRequirements::requireInstanceExtension( const char* name ){
    logDebug( "VulkanRequirements::requireInstanceExtension", name );
    add( new VulkanInstanceExtensionRequirement( name ) );
}
void VulkanRequirements::requireApiVersion(uint32_t apiVersion){
    logDebug( "VulkanRequirements::requireApiVersion", apiVersion );
    add( new VulkanApiVersionRequirement( apiVersion ) );
}
void VulkanRequirements::requireDeviceExtension( const char* name ){
    logDebug( "VulkanRequirements::requireDeviceExtension", name );
    add( new VulkanDeviceExtensionRequirement( name ) );
}
void VulkanRequirements::requireComputeQueue(){
    logDebug( "VulkanRequirements::requireComputeQueue" );
    add( new VulkanComputeQueueRequirement() );
}
void VulkanRequirements::requirePresentQueue( VulkanSurface* surface ){
    logDebug( "VulkanRequirements::requirePresentQueue" );
    add( new VulkanPresentQueueRequirement( surface ) );
}

