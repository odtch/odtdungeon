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
	while( !should_stop() ){
		//logDebug("VulkanPresenter::run", _state );
		device()->onIdle();
		sleep_ms( 6 );
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
			{
			if( vp == null ){
				vp = new VulkanCommandPool();
				vp->create( _computeQueue );
			}
				VulkanCommandBuffer* cb = new VulkanCommandBuffer();
				cb->begin(  VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, vp );


	//			VkCommandBufferBeginInfo beginInfo = {};
	//			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	//			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	//			vkBeginCommandBuffer(commandBuffer, &beginInfo);
				/*

				VkImage image = targetImage;

				// Transition the image layout to VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
				VkImageMemoryBarrier imageBarrier1 = {};
				imageBarrier1.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				imageBarrier1.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				imageBarrier1.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				imageBarrier1.srcAccessMask = 0; // Memory access flags for the old layout
				imageBarrier1.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT; // Memory access flags for the new layout
				imageBarrier1.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				imageBarrier1.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				imageBarrier1.image = image;
				imageBarrier1.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				imageBarrier1.subresourceRange.baseMipLevel = 0;
				imageBarrier1.subresourceRange.levelCount = 1;
				imageBarrier1.subresourceRange.baseArrayLayer = 0;
				imageBarrier1.subresourceRange.layerCount = 1;

				vkCmdPipelineBarrier(cb->vkCommandBuffer(),
									 VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
									 VK_PIPELINE_STAGE_TRANSFER_BIT,
									 0, 0, nullptr, 0, nullptr, 1, &imageBarrier1);


				// Specify the clear color
				* /
				static float r = 0;
				logDebug( "r", r );
				VkClearColorValue clearColor = { r, 0, 0.0f, 1.0f}; // Black color (R=0, G=0, B=0, A=1)
				r += 1.0f / 6;
				if( r > 1 )r = 0;
				/ *

				// Create the image subresource range
				VkImageSubresourceRange subresourceRange = {};
				subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				subresourceRange.baseMipLevel = 0;
				subresourceRange.levelCount = 1;
				subresourceRange.baseArrayLayer = 0;
				subresourceRange.layerCount = 1;

				// Record the clear color command
				vkCmdClearColorImage( cb->vkCommandBuffer(), image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearColor, 1, &subresourceRange);



				VkImageMemoryBarrier imageBarrier2 = {};
				imageBarrier2.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				imageBarrier2.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL; // The current layout of the image
				imageBarrier2.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // The desired layout for presentation
				imageBarrier2.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT; // Memory access flags for the old layout
				imageBarrier2.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT; // Memory access flags for the new layout
				imageBarrier2.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				imageBarrier2.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				imageBarrier2.image = image; // The image you want to present
				imageBarrier2.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				imageBarrier2.subresourceRange.baseMipLevel = 0;
				imageBarrier2.subresourceRange.levelCount = 1;
				imageBarrier2.subresourceRange.baseArrayLayer = 0;
				imageBarrier2.subresourceRange.layerCount = 1;

				vkCmdPipelineBarrier(cb->vkCommandBuffer(),
									 VK_PIPELINE_STAGE_TRANSFER_BIT,
									 VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
									 0, 0, nullptr, 0, nullptr, 1, &imageBarrier2);
									 */

				// Create a render pass with the clear color
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
				//logDebug( "r", r );

				VkClearValue clearValues[2] = {};
				clearValues[0].color = {r, 0.0f, 0.0f, 1.0f}; // Black color (R=0, G=0, B=0, A=1)
				clearValues[1].depthStencil = {1.0f, 0}; // Depth=1.0, Stencil=0


				renderPassBeginInfo.clearValueCount = 2; // Number of elements in the clearValues array
				renderPassBeginInfo.pClearValues = clearValues;

				// Begin the render pass
				vkCmdBeginRenderPass(cb->vkCommandBuffer(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

				// Render a full-screen quad
				// Your vertex buffer and vertex shader setup to render a full-screen quad

				// End the render pass
				vkCmdEndRenderPass(cb->vkCommandBuffer());


				// End recording the command buffer
				//vkEndCommandBuffer(commandBuffer);
				cb->end();

				cb->execute();

				cb->destroy();
				odelete( cb );
	//			// Submit the command buffer to the graphics queue for execution
	//			VkSubmitInfo submitInfo = {};
	//			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	//			submitInfo.commandBufferCount = 1;
	//			submitInfo.pCommandBuffers = &commandBuffer;

	//			// Create a fence for synchronization
	//			VkFence fence;
	//			VkFenceCreateInfo fenceCreateInfo = {};
	//			fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	//			vkCreateFence(device, &fenceCreateInfo, nullptr, &fence);

	//			// Submit the command buffer
	//			vkQueueSubmit(graphicsQueue, 1, &submitInfo, fence);

	//			// Wait for the command buffer to finish executing
	//			vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);

	//			// Clean up the fence
	//			vkDestroyFence(device, fence, nullptr);


			}

			_state = Presenting;
	//			_prevRenderTask.clear();
	//			//	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	//		}
			break;
		case Presenting:
			//logDebug( "pres" );
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

		//sleep_ms(1300);
	}
	if( vp ){
		vp->destroy();
		odelete( vp );
	}
	destroyFramebuffer();
	destroyDevice();
}
void VulkanPresenter::createDevice()
{
	assert( _vulkan == null );
	_vulkan = new Vulkan();
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
	assert( _presentQueue == null );
	_presentQueue = _device->getPresentQueue(*_surface);
	assert( _computeQueue == null );
	_computeQueue = _device->getComputeQueue();
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
		_fps.tick();
		//if( _fps.frame_count() % 10 == 2
		//_present_pc->stop();
		if( _fps.frame_count() % 10 == 9 )
			logDebug( "present fps=", _fps.fps() );
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
	logDebug( "VulkanPresenter::destroyFramebuffer wait idle" );
	device()->waitIdle();
	for (VulkanFramebuffer *framebuffer : _framebuffers) {
		logDebug( "VulkanPresenter::destroyFramebuffer framebuffer" );
		framebuffer->destroy();
		delete framebuffer;
	}
	_framebuffers.clear();
	if( _renderPass ){
		logDebug( "VulkanPresenter::destroyFramebuffer renderPass" );
		_renderPass->destroy();
		odelete(_renderPass);
	}
	if( _depthImageView ){
		logDebug( "VulkanPresenter::destroyFramebuffer depthImageView" );
		_depthImageView->destroy();
		odelete( _depthImageView );
	}
	if( _depthImage ){
		logDebug( "VulkanPresenter::destroyFramebuffer depthImage" );
		_depthImage->destroy();
		odelete( _depthImage );
	}
	if( _swapchain ){
		logDebug( "VulkanPresenter::destroyFramebuffer swapChain" );
		_swapchain->destroy();
		odelete( _swapchain );
	}
	logDebug( "VulkanPresenter::destroyFramebuffer done" );
}
void VulkanPresenter::destroyDevice(){
	logDebug( "VulkanPresenter::destroyDevice" );
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
	if( _vulkan ){
		odelete( _vulkan );
	}
	logDebug( "VulkanPresenter::destroyDevice done" );
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
