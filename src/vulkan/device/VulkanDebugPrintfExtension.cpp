#include "stdodt.h"
#include "VulkanDebugPrintfExtension.h"

//const char* VulkanDebugPrintfExtension::Name = VK_KHR_SHADER_NON_SEMANTIC_INFO_EXTENSION_NAME;

//VulkanDebugPrintfExtension::VulkanDebugPrintfExtension(){
//	#ifdef _WIN32
//	  _putenv_s("DEBUG_PRINTF_TO_STDOUT", "1");
//	#else   // If not _WIN32
//	  putenv( (char*) "DEBUG_PRINTF_TO_STDOUT=1" );
//	#endif  // _WIN32
//}
//VulkanDebugPrintfExtension::~VulkanDebugPrintfExtension(){

//	VkValidationFeaturesEXT      validationInfo{VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT};
//	VkValidationFeatureEnableEXT validationFeatureToEnable = VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT;
//	validationInfo.enabledValidationFeatureCount           = 1;
//	validationInfo.pEnabledValidationFeatures              = &validationFeatureToEnable;
//	deviceInfo.instanceCreateInfoExt                       = &validationInfo;



//}
//const char* VulkanDebugPrintfExtension::name() const {
//	return Name;
//}
////void* VulkanDebugPrintfExtension::resetFeature(){
////	memset( &_features, 0, sizeof( _features ) );
////	_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
////	return &_features;
////}
////void VulkanDebugPrintfExtension::checkSupportedFeatures( const VkPhysicalDeviceFeatures &features ){
////	VulkanDeviceExtension::checkSupportedFeatures( features );
////	if( _features.accelerationStructure != VK_TRUE )
////		throw std::runtime_error( "no accelerationStructure" );
////}
