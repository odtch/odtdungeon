#include "stdodt.h"
#include "VulkanCore.h"

int logcount = 0;

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	[[maybe_unused]] VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	[[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	[[maybe_unused]] void* pUserData
){
	if( messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT && messageType == 1 ){
        return VK_FALSE;
    }
    if( pCallbackData ){
        if( pCallbackData->pMessageIdName ){
            if( strcmp( "VUID-VkStridedDeviceAddressRegionKHR-size-04631", pCallbackData->pMessageIdName ) == 0 ){
                static int e04631 = 0;
                if( 3 < e04631 ){
                    return VK_FALSE;
                }
                e04631++;
                std::cout << "ignore e04631: ";
            }
        }
    }
	std::cout << "VulkanDebugCallback: " << messageSeverity << ": " << messageType << ":\n  " << pCallbackData->pMessage << std::endl;
	std::cout.flush();
	// VUID-VkSwapchainCreateInfoKHR-imageExtent-01274
	// This happens because the 'moment' you query the swapchain image extent is possibly not the same moment the OS resizes the window (and thus changing the extent).
    logcount++;
    if( logcount > 50 ){
        std::cout << "VulkanDebugCallback: too many logs, exit" << std::endl;
        std::cout.flush();
        exit( 1 );
    }
	return VK_FALSE;
}

#ifdef DEBUG
bool Vulkan::debuggingEnabled = true;
#else
bool Vulkan::debuggingEnabled = false;
#endif

/*
 *
Vulkan::Vulkan(){
}
Vulkan::~Vulkan(){
}
*/

const char* Vulkan::Id = "Vulkan";

Vulkan::Vulkan()
	:Singleton( Id )
{
#ifdef ODTDEBUG
			Vulkan::debuggingEnabled = true;
//			//https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/master/docs/debug_printf.md
			putenv( (char*) "DEBUG_PRINTF_TO_STDOUT=1" );
			putenv( (char*) "VK_LAYER_ENABLES=VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT" );
//			//requirements.requireDeviceExtension( VK_KHR_SHADER_NON_SEMANTIC_INFO_EXTENSION_NAME );
#endif
}
Vulkan::~Vulkan(){
}
bool Vulkan::isSuccess( VkResult result ){
	return result == VK_SUCCESS;
}
void Vulkan::check( VkResult result, const char* error_message ){
	assert( error_message );
	if( !isSuccess( result ) ){
		std::cout << "Vulkan::check failed " << result << ": " << error_message << ", " << getVkResultAsText( result ) << "\n";
		std::cout.flush();
		//assert( false );
		throw std::runtime_error( error_message );
	}
}
const char* Vulkan::getVkResultAsText( VkResult result ){
	switch( result ){
	case VK_SUCCESS: return "VK_SUCCESS";
	case VK_NOT_READY: return "VK_NOT_READY";
	case VK_TIMEOUT: return "VK_TIMEOUT";
	case VK_EVENT_SET: return "VK_EVENT_SET";
	case VK_EVENT_RESET: return "VK_EVENT_RESET";
	case VK_INCOMPLETE: return "VK_INCOMPLETE";
	case VK_ERROR_OUT_OF_HOST_MEMORY: return "VK_ERROR_OUT_OF_HOST_MEMORY";
	case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
	case VK_ERROR_INITIALIZATION_FAILED: return "VK_ERROR_INITIALIZATION_FAILED";
	case VK_ERROR_DEVICE_LOST: return "VK_ERROR_DEVICE_LOST";
	case VK_ERROR_MEMORY_MAP_FAILED: return "VK_ERROR_MEMORY_MAP_FAILED";
	case VK_ERROR_LAYER_NOT_PRESENT: return "VK_ERROR_LAYER_NOT_PRESENT";
	case VK_ERROR_EXTENSION_NOT_PRESENT: return "VK_ERROR_EXTENSION_NOT_PRESENT";
	case VK_ERROR_FEATURE_NOT_PRESENT: return "VK_ERROR_FEATURE_NOT_PRESENT";
	case VK_ERROR_INCOMPATIBLE_DRIVER: return "VK_ERROR_INCOMPATIBLE_DRIVER";
	case VK_ERROR_TOO_MANY_OBJECTS: return "VK_ERROR_TOO_MANY_OBJECTS";
	case VK_ERROR_FORMAT_NOT_SUPPORTED: return "VK_ERROR_FORMAT_NOT_SUPPORTED";
	case VK_ERROR_FRAGMENTED_POOL: return "VK_ERROR_FRAGMENTED_POOL";
	case VK_ERROR_UNKNOWN: return "VK_ERROR_UNKNOWN";
	case VK_ERROR_OUT_OF_POOL_MEMORY: return "VK_ERROR_OUT_OF_POOL_MEMORY";
	case VK_ERROR_INVALID_EXTERNAL_HANDLE: return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
	case VK_ERROR_FRAGMENTATION: return "VK_ERROR_FRAGMENTATION";
	case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
	case VK_ERROR_SURFACE_LOST_KHR: return "VK_ERROR_SURFACE_LOST_KHR";
	case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
	case VK_SUBOPTIMAL_KHR: return "VK_SUBOPTIMAL_KHR";
	case VK_ERROR_OUT_OF_DATE_KHR: return "VK_ERROR_OUT_OF_DATE_KHR";
	case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
	case VK_ERROR_VALIDATION_FAILED_EXT: return "VK_ERROR_VALIDATION_FAILED_EXT";
	case VK_ERROR_INVALID_SHADER_NV: return "VK_ERROR_INVALID_SHADER_NV";
	case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
	case VK_ERROR_NOT_PERMITTED_EXT: return "VK_ERROR_NOT_PERMITTED_EXT";
	case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT: return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
	case VK_THREAD_IDLE_KHR: return "VK_THREAD_IDLE_KHR";
	case VK_THREAD_DONE_KHR: return "VK_THREAD_DONE_KHR";
	case VK_OPERATION_DEFERRED_KHR: return "VK_OPERATION_DEFERRED_KHR";
	case VK_OPERATION_NOT_DEFERRED_KHR: return "VK_OPERATION_NOT_DEFERRED_KHR";
	case VK_PIPELINE_COMPILE_REQUIRED_EXT: return "VK_PIPELINE_COMPILE_REQUIRED_EXT";
//    case VK_ERROR_OUT_OF_POOL_MEMORY_KHR: return "VK_ERROR_OUT_OF_POOL_MEMORY_KHR";
//    case VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR: return "VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR";
//    case VK_ERROR_FRAGMENTATION_EXT: return "VK_ERROR_FRAGMENTATION_EXT";
//    case VK_ERROR_INVALID_DEVICE_ADDRESS_EXT: return "VK_ERROR_INVALID_DEVICE_ADDRESS_EXT";
//    case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR: return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR";
//    case VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT: return "VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT";
	default: return "unknown VkResult";
	}
}
/*
void Vulkan::getInstanceExtensionProperties( std::vector<VkExtensionProperties>& extensions ){
}
void Vulkan::getInstanceLayerProperties( std::vector<VkLayerProperties>& layers ){
	assert( layers.size() == 0 );
	uint32_t layerCount = 0;
	VkResult result = vkEnumerateInstanceLayerProperties( &layerCount, nullptr );
	if( result != VK_SUCCESS ){
		throw std::runtime_error( "vkEnumerateInstanceLayerProperties count failed" );
	}
	layers.resize( layerCount );
	result = vkEnumerateInstanceLayerProperties( &layerCount, layers.data() );
	if( result != VK_SUCCESS ){
		throw std::runtime_error( "vkEnumerateInstanceLayerProperties get failed" );
	}
	if( Vulkan::debuggingEnabled ){
		std::cout << "VulkanSystem: available layers:\n";
		for (const auto& layer : layers) {
			std::cout << '\t' << layer.layerName << '\n';
		}
	}
}
*/
void Vulkan::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}
//void Vulkan::receive( const Event& event ){
//	_event_queue.add( event );
//}

uint32_t vulkanGetAccessMaskPipelineStageFlags( uint32_t accessMask ){
	// see commands_vk.hpp makeAccessMaskPipelineStageFlags
	static const VkPipelineStageFlags supportedShaderBits = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT
											   | VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT
											   | VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
											   | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
	static const uint32_t accessPipes[] = {
	  /*  0 */ VK_ACCESS_INDIRECT_COMMAND_READ_BIT,						VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT,
	  /*  2 */ VK_ACCESS_INDEX_READ_BIT,									VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
	  /*  4 */ VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,					    VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
	  /*  6 */ VK_ACCESS_UNIFORM_READ_BIT,								supportedShaderBits,
	  /*  8 */ VK_ACCESS_INPUT_ATTACHMENT_READ_BIT,						VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
	  /* 10 */ VK_ACCESS_SHADER_READ_BIT,								supportedShaderBits,
	  /* 12 */ VK_ACCESS_SHADER_WRITE_BIT,							    supportedShaderBits,
	  /* 14 */ VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,					    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
	  /* 16 */ VK_ACCESS_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT,	    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
	  /* 18 */ VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,					    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
	  /* 20 */ VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,			    VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
	  /* 22 */ VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,			    VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
	  /* 24 */ VK_ACCESS_TRANSFER_READ_BIT,							    VK_PIPELINE_STAGE_TRANSFER_BIT,
	  /* 26 */ VK_ACCESS_TRANSFER_WRITE_BIT,							    VK_PIPELINE_STAGE_TRANSFER_BIT,
	  /* 28 */ VK_ACCESS_HOST_READ_BIT,								    VK_PIPELINE_STAGE_HOST_BIT,
	  /* 30 */ VK_ACCESS_HOST_WRITE_BIT,								    VK_PIPELINE_STAGE_HOST_BIT,
	  /* 32 */ VK_ACCESS_MEMORY_READ_BIT,							    0,
	  /* 34 */ VK_ACCESS_MEMORY_WRITE_BIT,							    0,
	#if VK_NV_device_generated_commands
	  /* 36 */ VK_ACCESS_COMMAND_PREPROCESS_READ_BIT_NV,				    VK_PIPELINE_STAGE_COMMAND_PREPROCESS_BIT_NV,
	  /* 38 */ VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV,			    VK_PIPELINE_STAGE_COMMAND_PREPROCESS_BIT_NV,
	#endif
	#if VK_NV_ray_tracing
	  VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_NV,				VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_NV | supportedShaderBits | VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_NV,
	  VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_NV,		    VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_NV,
	#endif
	  0
	};
	if( !accessMask ){
		return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	}
	uint32_t pipes = 0;
	for( uint32_t i = 0; i < 255; i += 2 ){
		if( accessPipes[i] == 0 ){
			assert(pipes != 0);
			return pipes;
		}
		if( accessPipes[i] & accessMask ){
			pipes |= accessPipes[i + 1];
		}
	}
	assert( false );
	return 0;
}
