#pragma once

#include "vulkan/VulkanCore.h"
#include "window/WindowCore.h"

class VulkanSurface
{
private:
	VkSurfaceKHR _vkSurface;
	Window* _window;
	VulkanInstance* _instance;
public:
	VulkanSurface();
	~VulkanSurface();
public:
	void create( Window* window, VulkanInstance* instance );
	void destroy();
public:
	VkSurfaceKHR vkSurface() const { assert( _vkSurface ); return _vkSurface; }
	Window* window() const { assert( _window ); return _window; }
	VulkanInstance* instance() const { assert( _instance ); return _instance; }
public:
	void getCapabilities( VulkanDevice& device, VkSurfaceCapabilitiesKHR& capabilities );
};

