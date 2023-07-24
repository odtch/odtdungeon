#pragma once

#include "utils/Debugging.h"
#include "utils/Logger.h"
#include "utils/Singleton.h"
#include "utils/Pointer.h"
#include "utils/Clock.h"
//#include "renderer/OdtRenderer.h"
//#include "utils/OdtUtils.h"
//#include "window/WindowCore.h"
//#include "math/OdtMath.h"
//#include "resource/ResourceCore.h"
//#include "renderer/RenderCore.h"

#define VK_CHECK_RESULT( f ){ \
	VkResult res = ( f ); \
	if( res != VK_SUCCESS ){ \
		logError( "Vulkan failed", res ); \
		std::cerr << "VkResult is " << res << " in " << __FILE__ << " at line " << __LINE__ << "\n"; \
		assert( res == VK_SUCCESS ); \
	} \
}

//class Vulkan;

class VulkanRequirement;
class VulkanInstanceLayerRequirement;
class VulkanInstanceExtensionRequirement;
class VulkanApiVersionRequirement;
class VulkanDeviceFeatureRequirement;
class VulkanDeviceExtensionRequirement;
class VulkanQueueRequirement;
class VulkanRequirements;

class VulkanInstance;

class VulkanPhysicalDevice;
class VulkanQueueFamily;

class VulkanDeviceExtension;
class VulkanDevice;
class VulkanDeviceObject;

class VulkanQueryPool;

class VulkanQueue;
class VulkanCommandPool;
class VulkanCommandBuffer;
class VulkanSemaphore;
class VulkanFence;

class VulkanTask;
class VulkanCommand;

class VulkanBuffer;
class VulkanBufferArray;
class VulkanImage;
class VulkanImageView;
class VulkanSampler;
class VulkanTexture;
class VulkanTextureArray;
class VulkanImageArray;
class VulkanMeshBuffer;
class VulkanMesh;
class VulkanMeshData;
class VulkanMeshDataPointer;
class VulkanMeshPool;

class VulkanSurface;

class VulkanSwapchain;
struct VulkanSwapchainSupportDetails;
class VulkanRenderPass;
class VulkanFramebuffer;
class VulkanWindowAdapter;
class VulkanRenderer;

class VulkanDescriptorSetLayout;
class VulkanDescriptorPool;
class VulkanDescriptorSet;
class VulkanShaderModule;
class VulkanPipelineLayout;
class VulkanPipeline;

class VulkanComputeProgram;
class VulkanComputeExecutor;

//class VulkanRenderSystem;

class VulkanRasterizerWindow;
class VulkanRasterizer;

class VulkanRaytracer;
class VulkanRaytracingPipelineProperties;
class VulkanRaytracingShaderBindingTable;
class VulkanAccelerationStructureExtension;
class VulkanAccelerationStructure;
class VulkanBLAS;
//class VulkanTopLevelAccelerationInstance;
//class VulkanTopLevelAccelerationStructure;
class VulkanTLAS;
class VulkanTLASes;
class VulkanRayQueryExtension;

class VulkanBackgroundThread;

class Vulkan : public Singleton //, public EventReceiver
{
public:
	static const char* Id;
	static Vulkan* Get(){ return Singleton::Get<Vulkan>(); }
//private:
//	EventQueue _event_queue;
public:
	explicit Vulkan();
	~Vulkan();
public:
    static bool debuggingEnabled;
//public:
//	static void getInstanceExtensionProperties( std::vector<VkExtensionProperties>& extensions );
//	static void getInstanceLayerProperties( std::vector<VkLayerProperties>& layers );
public:
    static bool isSuccess( VkResult result );
    static void check( VkResult result, const char* error_message );
    static const char* getVkResultAsText( VkResult result );
public:
    static void populateDebugMessengerCreateInfo( VkDebugUtilsMessengerCreateInfoEXT& createInfo );
//public:
//	EventQueue* event_queue(){ return &_event_queue; }
//	virtual void receive( const Event& event ) override;
};

struct VulkanUntypedStructure {
	VkStructureType    sType;
	const void*        pNext;
};

uint32_t vulkanGetAccessMaskPipelineStageFlags( uint32_t accessMask );
