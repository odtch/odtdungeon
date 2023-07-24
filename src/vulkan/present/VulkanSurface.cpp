#include "stdodt.h"
#include "VulkanSurface.h"
#include "vulkan/device/VulkanInstance.h"
#include "vulkan/device/VulkanDevice.h"
#include "vulkan/device/VulkanPhysicalDevice.h"
#include "window/Window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

VulkanSurface::VulkanSurface()
	:_vkSurface( nullptr )
	,_window( nullptr )
	,_instance( nullptr )
{
	//std::cout << "VulkanSurface()\n";
}
VulkanSurface::~VulkanSurface(){
	if( _vkSurface ){
		std::cout << "~VulkanSurface not destroyed\n";
	}
	destroy();
	ASSERT( _instance == nullptr );
	ASSERT( _window == nullptr );
	ASSERT( _vkSurface == nullptr );
}
void VulkanSurface::create( Window* window, VulkanInstance* instance ){
	assert( window );
	assert( instance );
	if( _instance != nullptr )
		throw std::runtime_error( "VulkanSurface already created" );
	_instance = instance;
	_window = window;
	assert( _vkSurface == nullptr );
	Vulkan::check( glfwCreateWindowSurface( _instance->vkInstance(), window->glfWindow(), nullptr, &_vkSurface ), "glfwCreateWindowSurface failed" );
}
void VulkanSurface::destroy(){
	if( _vkSurface != nullptr ){
		vkDestroySurfaceKHR( instance()->vkInstance(), _vkSurface, nullptr );
		_vkSurface = nullptr;
	}
	_window = nullptr;
	_instance = nullptr;
}
void VulkanSurface::getCapabilities( VulkanDevice& device, VkSurfaceCapabilitiesKHR& capabilities ){
	memset( &capabilities, 0, sizeof( capabilities ) );
	Vulkan::check( vkGetPhysicalDeviceSurfaceCapabilitiesKHR( device.physicalDevice()->vkDevice(), _vkSurface, &capabilities ), "vkGetPhysicalDeviceSurfaceCapabilitiesKHR" );
}
