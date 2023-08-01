#pragma once

#include "window/Window.h"
#include "renderer/Renderer.h"
#include "vulkan/VulkanCore.h"
#include "vulkan/present/VulkanPresenter.h"
#include "VulkanRaytracingPipeline.h"
#include "VulkanRaytracerTarget.h"
#include "VulkanRaytracingShaderBindingTable.h"
#include "vulkan/command/VulkanCommandPool.h"
#include "vulkan/command/VulkanTask.h"
#include "vulkan/descriptor/VulkanDescriptorSet.h"
#include "vulkan/descriptor/VulkanDescriptorSetLayout.h"
#include "vulkan/descriptor/VulkanDescriptorPool.h"
#include "vulkan/pipeline/VulkanPipelineLayout.h"
//#include "vulkan/as/VulkanBLASList.h"
#include "vulkan/as/VulkanTLASList.h"
#include "vulkan/resource/VulkanMeshPool.h"
#include "vulkan/resource/VulkanTextureArray.h"
#include "vulkan/resource/VulkanRenderableArray.h"
#include "vulkan/resource/VulkanLightArray.h"
#include "vulkan/resource/VulkanMaterialArray.h"
#include "vulkan/resource/VulkanMeshLoader.h"

class VulkanRaytracer : public VulkanPresenter, private MessageHandler
{
private:
	MessageQueue _queue;
private:
	VulkanRaytracingPipelineProperties* _properties = null;
private:
	VulkanRaytracerTarget _target;
//private:
////	List<AbstractMesh*> _preloadedMeshes;
////private:
private:
	VulkanRaytracerGlobals* _globals = null;
private:
	//VulkanBLASList _blases;
	VulkanMeshPool* _meshpool = null;
	VulkanMeshes* _meshes = null;
	VulkanTLASList _tlases;
	List<VulkanRaytracerSkinInstance*> _skininstances;
	VulkanRenderableArray _instances;
	VulkanTextureArray _textures;
	VulkanMaterialArray _materials;
	VulkanLightArray _lights;
private:
	List<VulkanRaytracerRenderLayer*> _layers;
//private:
//	VulkanPipelineRaytracerExtension* _extension = null;
//// private:
//// 	uint32_t _current_viewport_index = 0;
//// 	VulkanTLAS* _current_solid_tlas = null;
//// 	VulkanTLAS* _current_translucent_tlas = null;
//private:
//	//List<MeshPNT*> _meshes2;
//	VulkanQueue* queue;
//private:
//	//VulkanBackgroundThread* _background_thread = null;
//private:
//	VulkanCommandPool _commandPool;
private:
	VulkanDescriptorSetLayout descriptorSetLayout;
	VulkanDescriptorPool descriptorPool;
private:
	VulkanDescriptorSet descriptorSet;
	VulkanPipelineLayout pipelineLayout;
	VulkanRaytracingShaderBindingTable _sbt;
	VulkanRaytracingPipeline pipeline;
//private:
////	VulkanCommandBuffer commandBuffer;
private:
	struct {
//		PerformanceCounter* prepare = null;
		PerformanceCounter* load = null;
		PerformanceCounter* render = null;
		PerformanceCounter* copytofb = null;
		PerformanceCounter* present = null;
	} _pcs;
	VulkanMeshLoader _meshLoaderImmediate;
////	VulkanMeshBackgroundLoader _meshLoaderBackground;
	struct {
		VulkanTask buildTLAS;
		VulkanTask loadData;
		VulkanTask render;
		VulkanTask copytofb;
//		VulkanTask present;
	} _tasks;
//public:
//	static uint AntiAliasingCount;
private:
public:
    explicit VulkanRaytracer( Window* window );
    virtual ~VulkanRaytracer() override;
public:
	virtual RenderLayer* createRootLayer() override;
	virtual RenderLayer* createNextLayer( RenderLayer* prev ) override;
public:
	virtual RenderInstancePNT* createInstance( RenderLayer* layer, const Mat4& posori, MeshPNT* mesh, Material* material ) override;
	virtual RenderSkinInstance* createSkinInstance( RenderLayer* layer, const Mat4& posori, Skin* skin ) override;
public:
	virtual void addLight( RenderLayer* layer, RenderLight* light ) override;
protected:
	virtual void getRequirements( VulkanRequirements& requirements ) override;
	virtual void createDevice() override;
	virtual void destroyDevice() override;
protected:
    virtual void run() override;
	virtual void render( VkImage targetimage ) override;
public:
	virtual bool handle( const Message& message ) override;
private:
	void startLoadData();
	void startRender();
	void loadLayer( RenderLayer* layer, VulkanLayerData& layerdata, VulkanTLAS* tlas );
	void startCopyToFB( VkImage targetimage );

//public:
//	void setExtension( VulkanPipelineRaytracerExtension* extension );
//public:
//	virtual VkImageUsageFlags swapchainImageUsage() override;
//public:
//	virtual void create( VulkanDevice* device ) override;
//	virtual void destroy() override;
//	void destroyTarget();
//public:
//	virtual void resize( const VkExtent2D& size ) override;
//public:
////	VulkanTLASes& tlases(){ return _tlases; }
//// 	VulkanComputeRaytracerInstances& instances(){ return _instances; }
//// 	VulkanBLASList& meshes(){ return _meshes; }
//// 	VulkanComputeRaytracerTextures& textures(){ return _textures; }
//// 	VulkanComputeRaytracerMaterials& materials(){ return _materials; }
//	VulkanCommandPool& commandPool(){ return _commandPool; }
//public:
//	VulkanTask& buildTLASTask(){ return _tasks.buildTLAS; }
//	VulkanTask& loadDataTask(){ return _tasks.loadData; }
//	VulkanTask& renderTask(){ return _tasks.render; }
//	VulkanTask& copytofbTask(){ return _tasks.copytofb; }
//	VulkanTask& presentTask(){ return _tasks.present; }
//public:
//	virtual void loadToGPU( const List<Collection*>& collections ) override;
//public:
//	virtual void onRenderableAdded( Renderable* renderable ) override;
//	virtual void onRenderableRemoved( Renderable* renderable ) override;
//public:
//	virtual void tick() override;
//public:
//	virtual void render(  VulkanRenderPass& renderpass, VulkanFramebuffer& framebuffer, VkImage targetImage, VkImageView targetImageView, VulkanSemaphore* renderImageReadyToRenderSemaphore, VulkanFence* renderingDoneFence ) override;
//	void startRender();
//
//// public:
//// 	virtual void begin( VulkanRenderPass& renderpass, VulkanFramebuffer& framebuffer, VkImage targetImage, VkImageView targetImageView, VulkanSemaphore* renderImageReadyToRenderSemaphore, VulkanFence* renderingDoneFence ) override;
//// 	virtual uint32_t render( const Viewport& viewport, const Camera& camera, const Scene& scene ) override;
//// 	virtual void end( VulkanRenderPass& renderpass, VulkanFramebuffer& framebuffer, VkImage targetImage, VkImageView targetImageView, VulkanSemaphore* renderImageReadyToRenderSemaphore, VulkanFence* renderingDoneFence ) override;
//public:
////	uint32_t createViewport( const Rect2i& rect, const Camera& camera, VulkanTLAS* tlas_solid );
////	VulkanDevice* device() const;
////	VulkanCommandPool& commandPool() const;
////	VulkanRenderPass& renderPass();
////	const VkExtent2D& size() const;
////	Viewer* viewer() const;
////	const Camera& camera() const;
//private:
//	//void add( const Mat4& transform, Renderable* renderable );
//	void add( VulkanTLAS* tlas, const Mat4& transform, Renderable* renderable );
//// public:
//// 	virtual void addLight( const PosOri& posori, int type, float range, const vec4& color ) override;
//// public:
//// 	virtual void setViewport_Sky( uint type, const vec4& info0 = vec4( 0, 0, 0, 0 ), const vec4& info1 = vec4( 0, 0, 0, 0 ), const vec4& info2 = vec4( 0, 0, 0, 0 ) ) override;
//// 	void setParticles( VulkanTLAS* particle_tlas );
//// private:
//// 	void end( VulkanCommandBuffer& commandBuffer );
//// private:
//// 	VulkanRenderableRasterizer* findRasterizer( Renderable* renderable ) const;
//// 	VulkanRenderableRasterizer* createRasterizer( Renderable* renderable );
//public:
//	virtual bool isVulkanRaytracer() const override { return true; }
//	virtual VulkanRaytracer* asVulkanRaytracer() override { return this; }
//// public:
//// 	friend class VulkanComputeRaytracerInstances;

};
