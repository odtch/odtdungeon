#include "VulkanPresenter.h"
#include "VulkanRenderPass.h"
#include "VulkanSurface.h"
#include "VulkanSwapchain.h"
#include "VulkanFramebuffer.h"
#include "stdodt.h"
#include "vulkan/device/VulkanDevice.h"
#include "vulkan/device/VulkanInstance.h"
#include "vulkan/device/VulkanRequirement.h"
#include "vulkan/command/VulkanQueue.h"
#include "vulkan/command/VulkanSemaphore.h"
#include "vulkan/command/VulkanTask.h"
#include "vulkan/resource/VulkanImage.h"
#include "vulkan/resource/VulkanImageView.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

VulkanPresenter::VulkanPresenter( VkImageUsageFlags swapchainImageUsage, Window* window )
	:_window( asserted(window) )
	,_swapchainImageUsage( swapchainImageUsage )
{
	_present_pc = PerformanceCounter::Create("VulkanPresenter");
	_present_pc->start();
}
VulkanPresenter::~VulkanPresenter(){
}
void VulkanPresenter::run(){
	createDevice();
	createFramebuffer();
	while (!should_stop()) {
		logDebug("VulkanPresenter::run");
		sleep_ms(1300);
	}
	destroyFramebuffer();
	destroyDevice();
}
void VulkanPresenter::createDevice()
{
	VulkanRequirements requirements;
	{
		uint32_t glfwExtensionCount = 0;
		const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		for (auto extension : extensions) {
			logDebug(extension);
			requirements.requireInstanceExtension(extension);
		}
	}
	getRequirements( requirements );
	assert(_instance == null);
	_instance = new VulkanInstance();
	_instance->create("OdtVulkanPresenter", requirements);
	{
		requirements.requireDeviceExtension( VK_KHR_SHADER_NON_SEMANTIC_INFO_EXTENSION_NAME );
		assert(_surface == null);
		_surface = new VulkanSurface();
		_surface->create(_window, _instance);
		requirements.requireDeviceExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		requirements.requirePresentQueue(_surface);
	}
	assert(_device == null);
	_device = new VulkanDevice(_instance);
	_device->create(requirements);
	_presentQueue = _device->getPresentQueue(*_surface);
	assert(_presentQueue);
}
void VulkanPresenter::createFramebuffer()
{
	VkSurfaceCapabilitiesKHR capabilities;
	assert(_surface);
	_surface->getCapabilities(*_device, capabilities);
	if (_window->width() < capabilities.minImageExtent.width
		|| capabilities.maxImageExtent.width < _window->width()
		|| _window->height() < capabilities.minImageExtent.height
		|| capabilities.maxImageExtent.height < _window->height()) {
		logError("VulkanWindowAdapater::createFramebuffer window size not in capabilities");
		//return;
	}
	//	logDebug( "VulkanPresenter::createFramebuffer", _window->width(), _window->height(),
	//			  capabilities.minImageExtent.width, capabilities.minImageExtent.height,
	//			  capabilities.maxImageExtent.width, capabilities.maxImageExtent.height
	//			  );
	_window_extend.width = _window->width();
	_window_extend.height = _window->height();
	//_renderer->resize( _window_extend );
	assert(_swapchain == null);
	_swapchain = new VulkanSwapchain(_surface, _device);
	_swapchain->create(_window_extend, _swapchainImageUsage);
	_depthFormat = _device->getSupportedFormat({VK_FORMAT_D32_SFLOAT,
												VK_FORMAT_D32_SFLOAT_S8_UINT,
												VK_FORMAT_D24_UNORM_S8_UINT},
											   VK_IMAGE_TILING_OPTIMAL,
											   VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	assert(_depthImage == null);
	_depthImage = new VulkanImage();
	_depthImage->create(_swapchain->width(),
						_swapchain->height(),
						_depthFormat,
						VK_IMAGE_TILING_OPTIMAL,
						VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
						VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
						device());
	assert(_depthImageView == null);
	_depthImageView = new VulkanImageView();
	_depthImageView->create(_depthImage, _depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
	assert(_renderPass == null);
	_renderPass = new VulkanRenderPass();
	_renderPass->create(_swapchain->imageFormat(), _depthFormat, device());
	_framebuffers.resize(_swapchain->size());
	for (size_t i = 0; i < _framebuffers.size(); i++) {
		std::vector<VkImageView> attachments = { _swapchain->vkImageView(i), _depthImageView->vkImageView() };
		VulkanFramebuffer *framebuffer = new VulkanFramebuffer();
		_framebuffers[i] = framebuffer;
		framebuffer->create( _renderPass, _swapchain->extent(), attachments);
	}
}
void VulkanPresenter::tick(){
	device()->onIdle();
	switch( _state ){
	case Ready:
		break;
	case Rendering:
//		if( _prevRenderTask.isNull() ){
//			logError( "VulkanWindowAdapter.idle rendering no task" );
//			break;
//		}
//		if( !_prevRenderTask.isCompleted() ){
//		} else {
			_state = Presenting;
//			_prevRenderTask.clear();
//			//	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
//		}
		break;
	case FramebufferInvalid:
		logDebug( "VulkanWindowAdapter::onIdle recreating framebuffer" );
		device()->waitIdle();
		//	regetFramebufferSize();
		_state = Destroying;
		destroyFramebuffer();
		_state = Initializing;
		createFramebuffer();
		_state = Ready;
		break;
	default:
		throw std::runtime_error( "VulkanWindowAdapter::onIdle invalid state" );
		break;
	}

}

bool VulkanPresenter::addBeginAcquireNextRenderImage(VulkanTask&task, VkImage&targetImage)
{
//	if( !_prevRenderTask.isNull() ){
//		logDebug( "VulkanWindowAdapter vkAcquireNextImageKHR prevRenderTask ! null" );
//	}
	//std::cout << "VulkanWindowAdapter::beginAcquireNextRenderImage\n";
	uint32_t renderImageIndex = 0;
	VulkanSemaphore imageAquiredSemaphore;
	imageAquiredSemaphore.create( device() );
	//_renderImageReadyToRenderFence.reset();
	VkResult result = vkAcquireNextImageKHR(
		device()->vkDevice(),
		swapchain()->vkSwapchain(),
		0, // timeout UINT64_MAX,
		imageAquiredSemaphore.vkSemaphore(), // 		VK_NULL_HANDLE, //_imageAvailableSemaphore.vkSemaphore(), //imageAvailableSemaphores[currentFrame]->vkSemaphore(),
		nullptr, //_renderImageReadyToRenderFence.vkFence(),
		&renderImageIndex
	);
	if( result == VK_SUCCESS ){
		//std::cout << "vkAcquireNextImageKHR imageIndex=" << _renderImageIndex << "\n";
	} else if( result == VK_NOT_READY ){
		//_performanceCounter.beginRenderingNextImageNotAcquireable++;
		logDebug( "VulkanWindowAdapter vkAcquireNextImageKHR not ready" );
		return false;
	} else if( result == VK_ERROR_OUT_OF_DATE_KHR ){
		logDebug( "VulkanWindowAdapter vkAcquireNextImageKHR out of date" );
		_state = FramebufferInvalid;
		return false;
	} else if( result == VK_SUBOPTIMAL_KHR ){
		logDebug( "VulkanWindowAdapter vkAcquireNextImageKHR suboptimal" );
		_state = FramebufferInvalid;
		return false;
	} else {
		Vulkan::check( result, "vkAcquireNextImageKHR failed" );
		return false;
	}
	task.addWaitFor( imageAquiredSemaphore, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT );
	//VkImage renderImage = _swapchain.vkImage( renderImageIndex );
	//VkImageView renderImageView = _swapchain.vkImageView( _renderImageIndex );
	//VulkanFramebuffer* framebuffer = framebuffers().at( _renderImageIndex );
	_prevRenderImageIndex = renderImageIndex;
	//_prevRenderTask = task;
	targetImage = swapchain()->vkImage( renderImageIndex );
	_state = Rendering;
	//logDebug( "VulkanWindowAdapter beginAcquireNextRenderImage added" );
	return true;

}
VkResult vulkanQueuePresent( VulkanQueue* queue, uint32_t imageIndex, VulkanSwapchain& swapchain ){
	assert( queue );
	VkPresentInfoKHR presentInfo;
	memset( &presentInfo, 0, sizeof( presentInfo ) );
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	//presentInfo.waitSemaphoreCount = 1;
	//presentInfo.pWaitSemaphores = &_renderFinishedSemaphore.vkSemaphore();// signalSemaphores;
	VkSwapchainKHR swapChains[] = { swapchain.vkSwapchain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	VkResult result = vkQueuePresentKHR( queue->vkQueue(), &presentInfo );
	if( result == VK_SUCCESS ){
		return VK_SUCCESS;
	} else if( result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ){
		logDebug( "VulkanPresentQueue::queueImage invalid framebuffer" );
		return result;;
	} else {
		Vulkan::check( result, "vkQueuePresent failed" );
	}
	return result;
}
void VulkanPresenter::present(){
	switch( vulkanQueuePresent( _presentQueue, _prevRenderImageIndex, *swapchain() ) ){
	case VK_SUCCESS:
		//_performanceCounter.onImagePresented();
		_present_pc->stop();
		_window->onNewFrame();
		_present_pc->start();
		_state = Ready;
		break;
	case VK_ERROR_OUT_OF_DATE_KHR:
		logDebug( "VulkanQueuePresent out of date" );
		_state = FramebufferInvalid;
		break;
	case VK_SUBOPTIMAL_KHR:
		logDebug( "VulkanQueuePresent suboptimal" );
		_state = FramebufferInvalid;
		break;
	default:
		throw std::runtime_error( "_presentQueue.queueImage failed" );
	}
}
void VulkanPresenter::destroyFramebuffer(){
	for (VulkanFramebuffer *framebuffer : _framebuffers) {
		framebuffer->destroy();
		delete framebuffer;
	}
	_framebuffers.clear();
	if( _renderPass ){
		_renderPass->destroy();
		odelete(_renderPass);
	}
	if( _depthImageView ){
		_depthImageView->destroy();
		odelete( _depthImageView );
	}
	if( _depthImage ){
		_depthImage->destroy();
		odelete( _depthImage );
	}
	if( _swapchain ){
		_swapchain->destroy();
		odelete( _swapchain );
	}
}
void VulkanPresenter::destroyDevice(){
	//_prevRenderTask.clear();
	//_renderer->destroy();
	_presentQueue = null;
	//	_presentingDoneFence.destroy();
	//		_renderingDoneFence.clear();
	//	_renderFinishedSemaphore.destroy();
	//	_imageAvailableSemaphore.destroy();
	//	_device->destroy();
	//_renderImageReadyToRenderSemaphore.destroy();
	//_renderImageReadyToRenderFence.destroy();
	if( _surface ){
		_surface->destroy();
		odelete( _surface );
	}
	if( _device ){
		_device->destroy();
		odelete(_device);
	}
	if( _instance ){
		_instance->destroy();
		odelete(_instance);
	}
}

//void VulkanWindowAdapter::onWindowCreated( Window* window ){
//	assert( _window == window );
//	assert( _state == Initializing );
//	assert( _renderer );
//	{
//	}
////	_renderingDoneFence.create( 0, device() );
////	_presentingDoneFence.create( 0, device() );
//	//_renderImageReadyToRenderFence.create( 0, device() );
////	_renderImageReadyToRenderSemaphore.create( device() );
////	_imageAvailableSemaphore.create( device() );
////	_renderFinishedSemaphore.create( device() );
//	assert( _state == Initializing );
//	_state = Ready;
//}
//void VulkanWindowAdapter::onWindowDestroy(){
//	//std::cout << "VulkanWindowAdapter::destroy()\n";
//	_state = Destroying;
//	_prevRenderTask.clear();
//	assert( _window );
//	_window->removeListener( this );
//	_device.waitIdle();
//	if( _presentQueue ){
//		_presentQueue->waitIdle();
//	}
////	graphicsQueue()->waitIdle();
////	presentQueue()->waitIdle();
//	destroyFramebuffer();
//	destroyDevice();
//	_window = null;
//	_renderer = null;
//}
//void VulkanWindowAdapter::onWindowResized( int width, int height ){
//	_state = FramebufferInvalid;
//}
