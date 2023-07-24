#pragma once

#include "window/Window.h"
#include "renderer/Renderer.h"
#include "vulkan/VulkanCore.h"

class VulkanPresenter : public Renderer
{
private:
	Window* _window;
	VkExtent2D _window_extend;
	FpsCounter _fps;
	// PerformanceCounter* _present_pc = null;
private:
	enum State {
		Initializing,
		Ready,
		//WaitForRenderImage,
		Rendering,
		Presenting,
		FramebufferInvalid,
		Destroying
	} _state = Initializing;
private:
	Vulkan* _vulkan = null;
    VulkanInstance* _instance = null;
    VulkanDevice* _device = null;
private:
	VulkanSurface* _surface = null;
private:
	VulkanQueue* _presentQueue = null;
	VulkanQueue* _computeQueue = null;
	VulkanCommandPool* _computeCommandPool = null;
private:
	VkImageUsageFlags _swapchainImageUsage;
	VulkanSwapchain* _swapchain = null;
	VkFormat _depthFormat;
	VulkanImage* _depthImage = null;
	VulkanImageView* _depthImageView = null;
	VulkanRenderPass* _renderPass = null;
	std::vector<VulkanFramebuffer*> _framebuffers;
private:
//    VulkanTask _prevRenderTask;
	uint32_t _prevRenderImageIndex = 9999;
public:
	explicit VulkanPresenter( VkImageUsageFlags swapchainImageUsage, Window* window );
    virtual ~VulkanPresenter() override;
protected:
	virtual void run() override;
public:
	// VulkanInstance* instance(){ return _instance; }
	VulkanDevice* device(){ ASSERT( _device ); return _device; }
	// VulkanSurface& surface(){ return _surface; }
	VulkanSwapchain* swapchain(){ ASSERT( _swapchain ); return _swapchain; }
	// VkFormat depthFormat() const{ return _depthFormat; }
	// VulkanRenderPass& renderPass(){ return _renderPass; }
	// const std::vector<VulkanFramebuffer*>& framebuffers(){ return _framebuffers; }
protected:
	virtual void getRequirements( VulkanRequirements& requirements ) = 0;
	void createDevice();
	void createFramebuffer();
	bool acquireNextRenderImage( VkImage& targetImage );
	virtual void render();
	void present();
	void destroyFramebuffer();
	void destroyDevice();
};
