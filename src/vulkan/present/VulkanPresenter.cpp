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
	,_frameratelimiter( 60 - 0 )
	,_swapchainImageUsage( swapchainImageUsage )
{
	//_present_pc = PerformanceCounter::Create("VulkanPresenter");
	//_present_pc->start();
}
VulkanPresenter::~VulkanPresenter(){
}


#include "vulkan/command/VulkanCommand.h"
#include "vulkan/command/VulkanCommandPool.h"
#include "vulkan/command/VulkanCommandBuffer.h"

void VulkanPresenter::run(){
	static VulkanCommandPool* vp = null;
	assert( _state == Initializing );
	createDevice();
	createFramebuffer();
	_state = Ready;
	VkImage targetImage = 0;
	//StopClock prev_present_clock;
	while( !should_stop() ){
		//logDebug("VulkanPresenter::run", _state );
		device()->onIdle();
		//sleep_ms( 4 );
		switch( _state ){
		case Ready:
			//logDebug( "acqu" );
			if( acquireNextRenderImage( targetImage ) ){
				assert( _state == Rendering );
			}
			//logDebug( "acqu done" );
			break;
		case Rendering:
			//		if( _prevRenderTask.isNull() ){
			//			logError( "VulkanPresenter::idle rendering no task" );
			//			break;
			//		}
			//		if( !_prevRenderTask.isCompleted() ){
			//		} else {
			//sleep_ms( 13 );
			render( targetImage );
			_state = Presenting;
			break;
		case Presenting:
			//logDebug( "pres" );
			_frameratelimiter.tick();
			present();
			//logDebug( "pres done" );
			assert( _state != Presenting );
			break;
		case FramebufferInvalid:
			logDebug( "VulkanPresenter::run recreating framebuffer" );
			device()->waitIdle();
			//	regetFramebufferSize();
			_state = Destroying;
			destroyFramebuffer();
			_state = Initializing;
			createFramebuffer();
			_state = Ready;
			break;
		default:
			throw std::runtime_error( "VulkanPresenter::run invalid state" );
			break;
		}
	}
	if( vp ){
		vp->destroy();
		odelete( vp );
	}
	destroyFramebuffer();
	destroyDevice();
}
void VulkanPresenter::createDevice(){
	assert( _vulkan == null );
	_vulkan = new Vulkan();
	VulkanRequirements requirements;
	{
		uint32_t glfwExtensionCount = 0;
		const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		for( auto extension : extensions ){
			//logDebug(extension);
			requirements.requireInstanceExtension(extension);
		}
	}
	getRequirements( requirements );
	assert(_instance == null);
	_instance = new VulkanInstance();
	_instance->create( _window->title(), requirements );
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
	assert( _presentQueue == null );
	_presentQueue = _device->getPresentQueue(*_surface);
	assert(_presentQueue);
	assert( _computeQueue == null );
	_computeQueue = _device->getComputeQueue();
	assert( _computeQueue );
	assert( _computeCommandPool == null );
	_computeCommandPool = new VulkanCommandPool();
	_computeCommandPool->create( _computeQueue );
	assert( _computeCommandPool );
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
		logError( "VulkanPresenter::createFramebuffer window size not in capabilities");
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
bool VulkanPresenter::acquireNextRenderImage( VkImage& targetImage ){
	uint32_t renderImageIndex = 0;
	VulkanSemaphore imageAquiredSemaphore;
	imageAquiredSemaphore.create( device() );
	//_renderImageReadyToRenderFence.reset();
	VkResult result = vkAcquireNextImageKHR(
		device()->vkDevice(),
		swapchain()->vkSwapchain(),
		1000, // timeout UINT64_MAX in nanoseconds,
		imageAquiredSemaphore.vkSemaphore(), // 		VK_NULL_HANDLE, //_imageAvailableSemaphore.vkSemaphore(), //imageAvailableSemaphores[currentFrame]->vkSemaphore(),
		nullptr, //_renderImageReadyToRenderFence.vkFence(),
		&renderImageIndex
	);
	if( result == VK_SUCCESS ){
		//std::cout << "vkAcquireNextImageKHR imageIndex=" << _renderImageIndex << "\n";
	} else if( result == VK_NOT_READY ){
		//_performanceCounter.beginRenderingNextImageNotAcquireable++;
		logDebug( "VulkanPresenter::acquireNextRenderImage not ready" );
		return false;
	} else if( result == VK_ERROR_OUT_OF_DATE_KHR ){
		logDebug( "VulkanPresenter::acquireNextRenderImage out of date" );
		_state = FramebufferInvalid;
		return false;
	} else if( result == VK_SUBOPTIMAL_KHR ){
		logDebug( "VulkanPresenter::acquireNextRenderImage suboptimal" );
		_state = FramebufferInvalid;
		return false;
	} else {
		Vulkan::check( result, "VulkanPresenter::acquireNextRenderImage failed" );
		return false;
	}
	//task.addWaitFor( imageAquiredSemaphore, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT );
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
void VulkanPresenter::render( VkImage targetimage ){
	VulkanCommandBuffer* cb = new VulkanCommandBuffer();
	cb->begin(  VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, _computeCommandPool );
	VkRenderPassBeginInfo renderPassBeginInfo = {};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = asserted( _renderPass )->vkRenderPass(); // Your render pass object
	//logDebug( "_prevRenderImageIndex", _prevRenderImageIndex );
	auto framebuffer = _framebuffers[ _prevRenderImageIndex ];
	renderPassBeginInfo.framebuffer = framebuffer->vkFramebuffer(); // Your framebuffer object
	renderPassBeginInfo.renderArea.offset = {0, 0};
	renderPassBeginInfo.renderArea.extent = { _window_extend.width, _window_extend.height }; //framebufferWidth, framebufferHeight}; // The size of your framebuffer
	renderPassBeginInfo.clearValueCount = 1;
	static float r = 0;
	r += 1.0f / 6;
	if( r > 1 )r = 0;
	VkClearValue clearValues[2] = {};
	clearValues[0].color = {r, 0.0f, 0.0f, 1.0f}; // Black color (R=0, G=0, B=0, A=1)
	clearValues[1].depthStencil = {1.0f, 0}; // Depth=1.0, Stencil=0
	renderPassBeginInfo.clearValueCount = 2; // Number of elements in the clearValues array
	renderPassBeginInfo.pClearValues = clearValues;
	vkCmdBeginRenderPass(cb->vkCommandBuffer(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdEndRenderPass(cb->vkCommandBuffer());
	cb->end();
	cb->execute();
	cb->destroy();
	odelete( cb );
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
	//logDebug( "vkQueuePresentKHR" );
	VkResult result = vkQueuePresentKHR( queue->vkQueue(), &presentInfo );
	//logDebug( "vkQueuePresentKHR", result );
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
		if( _fps.tick() ){
			logDebug( "present fps=", _fps.fps(), "sleep=", _frameratelimiter._sleep_time_per_frame.toString() );
			_frameratelimiter._sleep_time_per_frame.reset();
		}
		//if( _fps.frame_count() % 10 == 2
		//_present_pc->stop();
//		if( _fps.frame_count() % 10 == 9 )
//		if( _fps.frame_count() % 60 == 59 )
		_window->onNewFrame();
		//_present_pc->start();
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
	//logDebug( "VulkanPresenter::destroyFramebuffer wait idle" );
	device()->waitIdle();
	for (VulkanFramebuffer *framebuffer : _framebuffers) {
		//logDebug( "VulkanPresenter::destroyFramebuffer framebuffer" );
		framebuffer->destroy();
		delete framebuffer;
	}
	_framebuffers.clear();
	if( _renderPass ){
		//logDebug( "VulkanPresenter::destroyFramebuffer renderPass" );
		_renderPass->destroy();
		odelete(_renderPass);
	}
	if( _depthImageView ){
		//logDebug( "VulkanPresenter::destroyFramebuffer depthImageView" );
		_depthImageView->destroy();
		odelete( _depthImageView );
	}
	if( _depthImage ){
		//logDebug( "VulkanPresenter::destroyFramebuffer depthImage" );
		_depthImage->destroy();
		odelete( _depthImage );
	}
	if( _swapchain ){
		//logDebug( "VulkanPresenter::destroyFramebuffer swapChain" );
		_swapchain->destroy();
		odelete( _swapchain );
	}
	//logDebug( "VulkanPresenter::destroyFramebuffer done" );
}
void VulkanPresenter::destroyDevice(){
	//logDebug( "VulkanPresenter::destroyDevice" );
	//_prevRenderTask.clear();
	//_renderer->destroy();
	if( _computeCommandPool ){
		_computeCommandPool->destroy();
		odelete( _computeCommandPool );
	}
	_computeQueue = null;
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
	if( _vulkan ){
		odelete( _vulkan );
	}
	//logDebug( "VulkanPresenter::destroyDevice done" );
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
