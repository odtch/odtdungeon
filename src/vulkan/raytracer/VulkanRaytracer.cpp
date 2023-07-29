#include "stdodt.h"
#include "VulkanRaytracer.h"
#include "VulkanRaytracerGlobals.h"
#include "VulkanPipelineRaytracerExtension.h"
#include "VulkanRaytracingPipelineExtension.h"
#include "VulkanRaytracingPipelineProperties.h"
#include "VulkanRaytracerRenderLayer.h"
#include "VulkanRaytracerInstance.h"
#include "VulkanRaytracerSkinInstance.h"
#include "vulkan/device/VulkanInstance.h"
#include "vulkan/device/VulkanDevice.h"
#include "vulkan/device/VulkanRequirement.h"
#include "vulkan/command/VulkanQueue.h"
#include "vulkan/command/VulkanCommandBuffer.h"
#include "vulkan/resource/VulkanMesh.h"
#include "vulkan/as/VulkanAccelerationStructureExtension.h"
#include "utils/File.h"
#include "renderer/RenderLayer.h"
#include "renderer/RenderLight.h"
#include "skin/OdtSkin.h"

//uint VulkanRaytracer::AntiAliasingCount = 3;

VulkanRaytracer::VulkanRaytracer( Window* window )
	:VulkanPresenter( VK_IMAGE_USAGE_TRANSFER_DST_BIT, window )
	//,_meshPool( window->device() )
{
//	_pcs.prepare = PerformanceCounter::Create( "Raytracer Prepare" );
	_pcs.load = PerformanceCounter::Create( "Raytracer Load" );;
	_pcs.render = PerformanceCounter::Create( "Raytracer Render" );;
	_pcs.copytofb = PerformanceCounter::Create( "Raytracer Copy to FB" );
	_pcs.present = PerformanceCounter::Create( "Raytracer Present" );
	_globals = new VulkanRaytracerGlobals();
}
VulkanRaytracer::~VulkanRaytracer(){
//	ASSERT( _extension == null );
	_skininstances.removeAll();
	_layers.deleteAll();
	odelete( _globals );
}
Texture* VulkanRaytracer::loadTexture( const String& name ){
	String filename = String( "media/" ) + name;
	BinaryFileReader reader( filename );
	Image* image = new Image();
	image->load( reader );
	Texture* texture = new Texture();
	//texture->load( reader );
	texture->setImage( image );
	_queue.post( VulkanTextureCreated, texture, null, this );
	return texture;
}
Mesh<VertexPNT>* VulkanRaytracer::createDynamicMeshPNT( const String& name ){
	MeshPNT* mesh = new MeshPNT();
	_queue.post( VulkanMeshPNTCreated, mesh, null, this );
	return mesh;
}
Mesh<VertexPNT>* VulkanRaytracer::loadMeshPNT( const String& name ){
	MeshPNT* mesh = new MeshPNT();
	{
		String filename = String( "media/" ) + name;
		BinaryFileReader reader( filename );
		mesh->load( reader );
	}
	_queue.post( VulkanMeshPNTCreated, mesh, null, this );
	return mesh;
}
RenderLayer* VulkanRaytracer::createRootLayer(){
	return createNextLayer( null );
}
RenderLayer* VulkanRaytracer::createNextLayer( RenderLayer* prev ){
	RenderLayer* layer = new VulkanRaytracerRenderLayer();
	_queue.post( VulkanLayerCreated, layer, prev, this );
	return layer;

}
RenderInstancePNT* VulkanRaytracer::createInstance( RenderLayer* layer, const PosOri& posori, Mesh<VertexPNT>* mesh, Material* material ){
	RenderInstancePNT* instance = new VulkanRaytracerInstancePNT( asserted( dynamic_cast<VulkanRaytracerRenderLayer*>( layer) ), posori, mesh, material );
	_queue.post( VulkanRenderInstancePNTCreated, instance, null, this );
	return instance;
}
RenderSkinInstance* VulkanRaytracer::createSkinInstance( RenderLayer* layer, const PosOri& posori, Skin* skin ){
	VulkanRaytracerSkinInstance* instance = new VulkanRaytracerSkinInstance( asserted( dynamic_cast<VulkanRaytracerRenderLayer*>( layer) ), posori, skin );
	_queue.post( VulkanRaytracerSkinInstanceCreated, instance, null, this );
	return instance;
}
void VulkanRaytracer::addLight( RenderLayer* layer, RenderLight* light ){
	_queue.post( VulkanLightAdded, layer, light, this );
}
void VulkanRaytracer::getRequirements( VulkanRequirements& requirements ){
	requirements.requireApiVersion( VK_API_VERSION_1_2 );
	requirements.requireDeviceExtension( VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME );
	requirements.requireDeviceExtension<VulkanAccelerationStructureExtension>();
	requirements.requireDeviceExtension<VulkanRaytracingPipelineExtension>();
	{
		auto requirement = new VulkanTypedDeviceFeatureRequirement<VkPhysicalDeviceDescriptorIndexingFeatures>( VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES );
		requirement->features.runtimeDescriptorArray = VK_TRUE;
		requirements.add( requirement );
	}
	requirements.requireComputeQueue();
}
void VulkanRaytracer::createDevice(){
	VulkanPresenter::createDevice();
	_meshPool.create( device() );
	assert( _properties == null );
	_properties = new VulkanRaytracingPipelineProperties();
	_properties->get( device()->physicalDevice() );
	////	_background_thread = new VulkanBackgroundThread( queue, Vulkan::Get()->event_queue() );
	////	_background_thread->start();
	_meshLoaderImmediate.create( "imed", computeCommandPool() );
	//	_meshLoaderBackground.create( "back", computeCommandPool() );
	//	//_blases.create( computeCommandPool() );
	_textures.create( computeCommandPool() );
	_tlases.create( computeCommandPool() );
	_materials.create( computeCommandPool(), &_textures );
	_instances.create( computeCommandPool() );
	_lights.create( computeCommandPool() );
	_globals->create( computeCommandPool() );
	_sbt.bindDevice( device() );
	String mediapath = "media";
	_sbt.bindShader( 0, VK_SHADER_STAGE_RAYGEN_BIT_KHR, mediapath + "/vrt.rgen.glsl.spv" );
	_sbt.bindShader( 1, VK_SHADER_STAGE_MISS_BIT_KHR, mediapath + "/vrt.rmiss.glsl.spv" );
	_sbt.bindShader( 2, VK_SHADER_STAGE_MISS_BIT_KHR, mediapath + "/vrt.shadow.rmiss.glsl.spv" );
	_sbt.bindShader( 3, VK_SHADER_STAGE_MISS_BIT_KHR, mediapath + "/vrt.transl.rmiss.glsl.spv" );
	//_sbt.bindShader( 3, VK_SHADER_STAGE_ANY_HIT_BIT_KHR, mediapath + "/vrt.rahit.glsl.spv" );
	_sbt.bindShader( 4, VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR, mediapath + "/vrt.rchit.glsl.spv" );
	_sbt.bindShader( 5, VK_SHADER_STAGE_ANY_HIT_BIT_KHR, mediapath + "/vrt.shadow.rahit.glsl.spv" );
	_sbt.bindShader( 6, VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR, mediapath + "/vrt.shadow.rchit.glsl.spv" );
	_sbt.bindShader( 7, VK_SHADER_STAGE_ANY_HIT_BIT_KHR, mediapath + "/vrt.transl.rahit.glsl.spv" );
	_sbt.bindShader( 8, VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR, mediapath + "/vrt.transl.rchit.glsl.spv" );
	_sbt.bindGroupGeneral( 0, VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR, 0 ); // rgen
	_sbt.bindGroupGeneral( 1, VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR, 1 ); // rmiss
	_sbt.bindGroupGeneral( 2, VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR, 2 ); // shadow
	_sbt.bindGroupGeneral( 3, VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR, 3 ); // transl.miss
	//_sbt.bindGroupClosestHit( 2, VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR, 2 );
	_sbt.bindGroupClosestHit( 4, VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR, VK_SHADER_UNUSED_KHR, 4 ); // rchit
	_sbt.bindGroupClosestHit( 5, VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR, 5, 6 ); // shadow
	_sbt.bindGroupClosestHit( 6, VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR, 7, 8 ); // transl
	_sbt.setCount( 1, 3, 3, 0 );
	_sbt.create();
	////	_globals->update( _tasks.loadData );
	//	if( _extension )_extension->create( device );
}
void VulkanRaytracer::destroyDevice(){
	while( !_vulkanmeshes.isEmpty() ){
		VulkanMesh* vm = _vulkanmeshes.takeFirst();
		vm->_mesh->setVulkanMesh( null );
		delete vm;
	}
	_globals->destroy();
	odelete( _properties );
	_meshPool.destroy();
	//	while( !_preloadedMeshes.isEmpty() ){
	//		AbstractMesh* mesh = _preloadedMeshes.takeLast();
	//		ASSERT( mesh->vulkanMesh()->instanceCount() == 1 );
	//		mesh->vulkanMesh()->decInstanceCount();
	//		ASSERT( mesh->vulkanMesh()->instanceCount() == 0 );
	//		VulkanMesh* vulkanmesh = mesh->vulkanMesh();
	//		mesh->setVulkanMesh( null );
	//		odelete( vulkanmesh );
	//	}
	////	if( _background_thread ){
	////		_background_thread->stop();
	////		odelete( _background_thread );
	////	}
	_meshLoaderImmediate.destroy();
	//	_meshLoaderBackground.destroy();
	_tasks.buildTLAS.clear();
	_tasks.loadData.clear();
	_tasks.render.clear();
	_tasks.copytofb.clear();
	//	_tasks.present.clear();
	computeQueue()->waitIdle();
	//	if( _extension ){
	//		_extension->destroy();
	//		delete _extension;
	//		_extension = null;
	//	}
	pipeline.destroy();
	pipelineLayout.destroy();
	_sbt.destroy();
	descriptorSet.destroy();
	descriptorPool.destroy();
	descriptorSetLayout.destroy();
	_tlases.destroy();
	_instances.destroy();
	_materials.destroy();
	_lights.destroy();
	//	//_blases.destroy();
	_textures.destroy();
	_meshPool.destroy();
	_target.destroy();
	VulkanPresenter::destroyDevice();
}
void VulkanRaytracer::run(){
	//sleep_ms( 1000 ); // startup-sleep, give scene some time to initialize
	VulkanPresenter::run();
}
void VulkanRaytracer::render( VkImage targetimage ){
	//logDebug( "VulkanRaytracer::render" );
	_queue.handle( this );
	{
		MutexLocker locker;
		locker.lock( writeMutex() );
		for( VulkanRaytracerSkinInstance* skininstance : _skininstances ){
			skininstance->updateMeshInCPU();
		}
		startLoadData();
	}
	startRender();
	startCopyToFB( targetimage );
	_pcs.present->start();
	//VulkanPresenter::render();
	_pcs.present->stop();
}

bool VulkanRaytracer::handle( const Message& message ){
	switch( message.type ){
	case VulkanTextureCreated:
		_textures.getTextureIndex( (Texture*) message.p1 );
		return true;
	case VulkanLayerCreated:
		{
			VulkanRaytracerRenderLayer* layer = asserted( (VulkanRaytracerRenderLayer*) message.p1 );
			VulkanRaytracerRenderLayer* prev = (VulkanRaytracerRenderLayer*) message.p2;
			assert( ( _layers.size() == 0 && prev == null ) || ( _layers.size() > 0 && prev != null ) );
			if( prev ){
				assert( prev->_nextLayer == null );
				prev->_nextLayer = layer;
			}
			layer->_index = _layers.size();
			_layers.add( layer );
		}
		return true;
	case VulkanMeshPNTCreated:
		{
			MeshPNT* mesh = (MeshPNT*) message.p1;
			auto vm = new VulkanMesh( mesh, &this->_meshPool );
			_vulkanmeshes.add( vm );
			mesh->setVulkanMesh( vm );
		}
		return true;
	case VulkanRenderInstancePNTCreated:
		{
			VulkanRaytracerInstancePNT* instance = (VulkanRaytracerInstancePNT*)message.p1;
			_globals->registerMaterial( instance->material() );
			instance->_layer->_instances.add( instance );
		}
		return true;
	case VulkanRaytracerSkinInstanceCreated:
		{
			VulkanRaytracerSkinInstance* skininstance = (VulkanRaytracerSkinInstance*)message.p1;
			_globals->registerMaterial( skininstance->skin()->material() );
			MeshPNT* mesh = new MeshPNT();
			skininstance->createMesh( mesh );
			auto vm = new VulkanMesh( mesh, &this->_meshPool );
			mesh->setVulkanMesh( vm );
			_vulkanmeshes.add( vm );
			VulkanRaytracerInstancePNT* pntinstance = new VulkanRaytracerInstancePNT( skininstance->_layer, skininstance->posori(), mesh, skininstance->skin()->material() );
			skininstance->_layer->_instances.add( pntinstance );
			_skininstances.add( skininstance );
		}
		return true;
	case VulkanLightAdded:
		((VulkanRaytracerRenderLayer*)message.p1)->_lights.add( (RenderLight*) message.p2 );
		return true;
	default:
		logError( "VulkanRaytracer unhandled message", message.type );
		ASSERT( false );
	}
	return false;
}
void VulkanRaytracer::startLoadData(){
	_pcs.load->start();
	_tasks.loadData.clear();
//	for( RenderLayer* layer : _layers ){
//		layer->prerender();
//	}
	_globals->begin();
	_tasks.buildTLAS.clear();
	_meshLoaderImmediate.reinit();
//	_meshLoaderBackground.preTick();
	_tasks.buildTLAS.create( "build tlas", VulkanTask::RunOnce, 30, computeCommandPool() );
	_tasks.buildTLAS.addWaitFor( _meshLoaderImmediate.buildBLASTask() );
	_tasks.loadData.create( "rt.load data", VulkanTask::RunOnce, 10, computeCommandPool() );
//	_tasks.loadData.addWaitFor( _tasks.buildTLAS );
	_instances.reset();
	_tlases.reset();

//	_globals->begin();
//	//List<SceneArea*> scene_areas;
//	//for( Viewport* viewport : viewports() )
	assert( !_layers.isEmpty() );
	_globals->_data.target_width = _target.size().width;
	_globals->_data.target_height = _target.size().height;
	_globals->_data.target_half_width = _globals->_data.target_width / 2;
	_globals->_data.target_half_height = _globals->_data.target_height / 2;
	_globals->_data.layer_count = 0;
	for( VulkanRaytracerRenderLayer* layer : _layers ){
		uint layerindex = _globals->_data.layer_count;
//		int layerindex = layer->index();
//		assert( _globals->_data.layer_count == layer->index() );
		_globals->_data.layer_count++;
		assert( _globals->_data.layer_count <= VuklanRaytracerGlobalsData_MaxLayerCount );
		VulkanLayerData& layerdata = _globals->_data.layers[ layerindex ];
//		int todo_render_area_only_once;
		VulkanTLAS* _current_solid_tlas = _tlases.next();
		loadLayer( layer, layerdata, _current_solid_tlas );
//		layerdata.tlas_index = -1;
		layerdata.tlas_index = _current_solid_tlas->index();
//		if( layer->hasTranslucentLayer() ){
//			layerdata.translucent_layer_index = layer->translucentLayer()->index();
//		} else {
			layerdata.translucent_layer_index = -1;
//		}
		if( layer->_nextLayer == null ){
			layerdata.next_layer_index = -1;
			layerdata.next_camera_action = 0;
		} else {
			layerdata.next_layer_index = layer->_nextLayer->_index;
			assert( 0 < layerdata.next_layer_index );
			layerdata.next_camera_action = layer->_nextCameraAction;
			if( layerdata.next_camera_action == VulkanLayer_NextCameraAction_2D_to_3D ){
				layerdata.next_camera_2d_fov_size = fmax( _target.size().width, _target.size().height ); // * 1.1f problem beim mouse.ray....
				//_globals->_data.layers_next_camera_transform[ layerindex ] = Mat4::Translation( Vec3( 0, 0, 0 ) ).asGlm(); // mouse.ray? y -50
				_globals->_data.layers_next_camera_transform[ layerindex ] = layer->_nextCamera.posori().matrix().asGlm();
				//Vec3 d = layer->_nextCamera.posori().matrix().mapNormal( Vec3( 0, 1, 0 ) );	logDebug( "d", d );

//			} else if( layerdata.next_camera_action == VulkanLayer_NextCameraAction_Transform ){
//				_globals->_data.layers_next_camera_transform[ layerindex ] = layer->nextCameraTransform().asGlm();
			} else {
				assert( false );
			}
//			if( layer->hasNextCamera() ){
//				_globals->_data.layers_next_camera_transform[ layerindex ] = Mat4::Translation( layer->nextCamera()->position() ).asGlm();
//				_globals->_data.layers_next_camera_transform[ layerindex ] = layer->nextCamera()->posori().matrix().asGlm();
//			}
		}
	}
	{
		_globals->_data.instance_count = _instances.count();
		_globals->_data.mesh_count = _meshPool.size(); // _blases.size();
//		_globals->_data.view_mode = 0;
//		if( _globals->_data.antialisingCount != AntiAliasingCount ){
//			//std::cout << "VulkanRaytracer switch AntialisingCount from " << _globals->_data.antialisingCount << " to " << AntiAliasingCount << "\n";
//			assert( 0 < AntiAliasingCount && AntiAliasingCount <= 6 );
//			_globals->_data.antialisingCount = AntiAliasingCount;
//		}
		_tlases.update();
		_materials.update( *_globals, _tasks.loadData );
		_instances.update( _tasks.loadData );
		_lights.update( _tasks.loadData );
		_globals->update( _tasks.loadData );
//		if( _extension )_extension->update( _commandPool );
	}
//	//logDebug( "VulkanRaytracer ", _globals->_data.layer_count, "layers", _globals->_data.layers[0].next_layer_index );
	_meshLoaderImmediate.start();
//	_meshLoaderBackground.postTick();
	_tasks.buildTLAS.start();
	_tasks.loadData.start();
	_pcs.load->stop();
}
void VulkanRaytracer::startRender(){
	//	if( _instances.isEmpty() ){
	//		logDebug( "VulkanRaytracer:: no instances" );
	//		return;
	//	}
	_pcs.render->start();
	_target.update( window_extend(), *computeCommandPool() );
	_tasks.render.clear();
	_tasks.render.create( "rt.render", VulkanTask::RunOnce, 10, computeCommandPool() );
	////	_tasks.render.commandBuffer().begin(
	////				VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, computeCommandPool() );
	////	_tasks.render.commandBuffer().addWait( &_tasks.loadData.completedSemaphore() );
	_tasks.render.addWaitFor( _tasks.loadData );
	_tasks.render.addWaitFor( _tasks.buildTLAS );
	const uint32_t MAX_MESH_COUNT = _meshPool.size();
	if( descriptorSetLayout.isCreated() ){
		if( descriptorSetLayout.descriptorSize( 1 ) != MAX_MESH_COUNT ||
			descriptorSetLayout.descriptorSize( 3 ) != _tlases.count() ||
			descriptorSetLayout.descriptorSize( 6 ) != (uint32_t) _textures.size()
		){
			logDebug( "VulkanRaytracer mesh count changed", MAX_MESH_COUNT );
			descriptorSet.destroy();
			descriptorPool.destroy();
			descriptorSetLayout.destroy();
			pipelineLayout.destroy();
			pipeline.destroy();
		}
	}
	ASSERT( 0 < _textures.size() );
	if( !descriptorSetLayout.isCreated() ){
		descriptorSetLayout.addBinding( 0, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, VK_SHADER_STAGE_RAYGEN_BIT_KHR );
		descriptorSetLayout.addBinding( 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, MAX_MESH_COUNT, VK_SHADER_STAGE_RAYGEN_BIT_KHR|VK_SHADER_STAGE_ANY_HIT_BIT_KHR|VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR );
		descriptorSetLayout.addBinding( 2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, MAX_MESH_COUNT, VK_SHADER_STAGE_ANY_HIT_BIT_KHR|VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR );
		descriptorSetLayout.addBinding( 3, VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR, _tlases.count(), VK_SHADER_STAGE_RAYGEN_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR|VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR );
		descriptorSetLayout.addBinding( 4, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_RAYGEN_BIT_KHR|VK_SHADER_STAGE_ANY_HIT_BIT_KHR|VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR );
		descriptorSetLayout.addBinding( 5, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_RAYGEN_BIT_KHR|VK_SHADER_STAGE_ANY_HIT_BIT_KHR|VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR );
		descriptorSetLayout.addBinding( 6, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, _textures.size(), VK_SHADER_STAGE_RAYGEN_BIT_KHR|VK_SHADER_STAGE_ANY_HIT_BIT_KHR|VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR );
		descriptorSetLayout.addBinding( 7, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_RAYGEN_BIT_KHR|VK_SHADER_STAGE_ANY_HIT_BIT_KHR|VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR );
		descriptorSetLayout.addBinding( 8, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_RAYGEN_BIT_KHR|VK_SHADER_STAGE_ANY_HIT_BIT_KHR|VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR );
		//if( _extension )_extension->addBindings( descriptorSetLayout );
		descriptorSetLayout.create( device() );
		descriptorPool.create( 5, 5, &descriptorSetLayout );
	}
	if( !descriptorSet.isCreated() ){
		descriptorSet.create( &descriptorPool );
	}
	descriptorSet.reset();
	descriptorSet.bindStorageImage( 0, &_target.imageView );
	//descriptorSet.bindStorageImage( 0, targetImageView );
	descriptorSet.bindBufferArray( 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, &_meshPool.vertexBufferArray() );
	descriptorSet.bindBufferArray( 2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, &_meshPool.indexBufferArray() );
	descriptorSet.bindAccelerations( 3, _tlases.count(), _tlases.descriptor_info() );
	descriptorSet.bindBuffer( 4, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, _instances.buffer() );
	descriptorSet.bindBuffer( 5, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, _lights._buffer );
	descriptorSet.bindTextureArray( 6, &_textures.imageArray() );
	//std::cout << "Texture-Count: " << _textures._textures.size() << "\n";
	descriptorSet.bindBuffer( 7, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, _globals->buffer() );
	descriptorSet.bindBuffer( 8, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, _materials.buffer() );
	//if( _extension )_extension->bind( descriptorSet );
	descriptorSet.update();
	if( !pipelineLayout.isCreated() ){
		pipelineLayout.addSet( &descriptorSetLayout );
		//pipelineLayout.addPushConstant( VK_SHADER_STAGE_RAYGEN_BIT_KHR, sizeof( PushConstants ) );
		pipelineLayout.create( device() );
	}
	if( !pipeline.isCreated() ){
		pipeline.create( &_sbt, &pipelineLayout, *_properties );
	}
	//	if( _extension )_extension->updatePipeline( &pipelineLayout, _properties );
	//	{
	//		int todo_view_center__splitt_static_and_important_objects_static_complete_hide_by_object_normal__importants_not_triangle_normal_check;
	//		int todo_von_timps_clustering__sphere_collider_at_ray_hit;
	//	}
	if( _target._imageInitial ){
		_target._imageInitial = false;
	} else {
		VkImageMemoryBarrier barrier;
		_target.image.setImageMemoryBarrier( barrier,
			VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,  // Src and dst access masks
			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL,  // Src and dst layouts
			VK_IMAGE_ASPECT_COLOR_BIT);
		vkCmdPipelineBarrier( _tasks.render.commandBuffer().vkCommandBuffer(),             // Command buffer
			VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, // Src and dst pipeline stages
			0,                     // Dependency flags
			0, nullptr,            // Global memory barriers
			0, nullptr,            // Buffer memory barriers
			1, &barrier);          // Image memory barriers
	}
	_tasks.render.commandBuffer().bindPipeline( VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, &pipeline );
	vkCmdBindDescriptorSets( _tasks.render.commandBuffer().vkCommandBuffer(), VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR,
					 pipelineLayout.vkPipelineLayout(), 0, 1,
					 &descriptorSet.vkDescriptorSet(), 0, nullptr );
	////	vkCmdPushConstants( commandBuffer.vkCommandBuffer(), pipelineLayout.vkPipelineLayout(), VK_SHADER_STAGE_RAYGEN_BIT_KHR, 0, sizeof(PushConstants), &_pushConstants );
	{
		// Run the ray tracing pipeline and trace rays
		pipeline.cmdTraceRays( _tasks.render.commandBuffer(), _target._size.width, _target._size.height, 1 );
	}
	//	if( _extension ){
	//		assert( false ); //_extension->cmd( commandBuffer, pipelineLayout, descriptorSet );
	//	}
	//	_tasks.render.addOnCompleted( [this](){
	//		//logDebug( "render completed" );
	//		_tasks.loadData.clear();
	//	} );
	_tasks.render.start();
	//	for( RenderLayer* layer : _layers ){
	//		for( Renderable* renderable : layer->renderables() ){
	//			renderable->render( *this );
	//		}
	//	}
	_pcs.render->stop();
}









//void VulkanRaytracer::resize( const VkExtent2D &size ){
//	queue->waitIdle();
//	pipeline.destroy();
//	pipelineLayout.destroy();
//	descriptorSet.destroy();
//	_target.resize( size, _commandPool );
//}
//void VulkanRaytracer::loadToGPU( const List<Collection*>& collections ){
//	logInfo( "VulkanRaytracer::loadToGPU" );
//	StopClock clock;
//	clock.start();
//	List<AbstractMesh*> meshes;
//	for( Collection* collection : collections ){
//		//logInfo( "  ", collection->id() );
//		for( Resource* resource : collection->resources() ){
//			//logInfo( "    ", resource->id() );
//			resource->collectPreLoad( meshes );
//			AbstractMesh* mesh = dynamic_cast<AbstractMesh*>( resource );
//			if( mesh ){
//				meshes.add( mesh, false );
//			}
//		}
//	}
//	logInfo( "to load", meshes.size(), "to optimaze multiple meshes at once" );
//	while( !meshes.isEmpty() ){
//		AbstractMesh* mesh = meshes.takeLast();
//		if( !mesh->hasVulkanMesh() ){
//			mesh->setVulkanMesh( new VulkanMesh( mesh, &_meshPool ) );
//		}
//		VulkanMesh* vulkanmesh = mesh->vulkanMesh();
//		vulkanmesh->incInstanceCount();
//		_preloadedMeshes.add( mesh, false );
////		// geht nicht, muss im loop passieren, muesste entweder vorher in eine liste oder der loader flexibler machen
//		_meshLoaderImmediate.reinit();
//		_meshLoaderImmediate.tryUpdateIfNeeded( vulkanmesh );
//		_meshLoaderImmediate.start();
//		while( _meshLoaderImmediate.buildBLASTask().isRunning() ){
//			// logInfo( "  m", _meshLoaderImmediate.loadMBTask().isRunning(), _meshLoaderImmediate.loadMBTask().isCompleted() , _meshLoaderImmediate.buildBLASTask().isRunning(), _meshLoaderImmediate.buildBLASTask().isCompleted() );
//			this->queue->onIdle();
//		}
//	}
//	clock.pause();
//	logInfo( "VulkanRaytracer::loadToGPU completed", clock.seconds(), "s" );
//}
//void VulkanRaytracer::onRenderableAdded( Renderable* renderable ){
//	ASSERT( renderable );
//	if( renderable->isPNT() ){
//		MeshPNT* mesh = renderable->asPNT()->mesh();
//		if( !mesh->hasVulkanMesh() ){
//			mesh->setVulkanMesh( new VulkanMesh( mesh, &_meshPool ) );
//		}
//		mesh->vulkanMesh()->incInstanceCount();
//	}
//}
//void VulkanRaytracer::onRenderableRemoved( Renderable* renderable ){
//	ASSERT( renderable );
//	if( renderable->isPNT() ){
//		MeshPNT* mesh = renderable->asPNT()->mesh();
//		ASSERT( mesh->hasVulkanMesh() );
//		mesh->vulkanMesh()->decInstanceCount();
//		if( mesh->vulkanMesh()->instanceCount() == 0 ){
//			VulkanMesh* vulkanmesh = mesh->vulkanMesh();
//			mesh->setVulkanMesh( null );
//			odelete( vulkanmesh );
//		}
//	}
//}
//void VulkanRaytracer::tick(){
//	if( _tasks.loadData.isNull() ){
//		startLoadData();
//	} else if( _tasks.render.isNull() ){
//		startRender();
//		if( _tasks.render.isNull() ){
//			if( _tasks.loadData.isCompleted() ){
//				logDebug( "VulkanRaytracer.tick restart load" );
//				_tasks.loadData.clear();
//			}
//		}
//	} else if( _tasks.copytofb.isNull() ){
//		startCopyToFB();
//	}
////	if( !_tasks.render.isNull() ){
////		if( _tasks.render.isCompleted() ){
////			_tasks.loadData.clear();
////		}
////	}
////	if( !_copyimage.task.isNull() ){
////		if( _copyimage.task.isCompleted() )
////	}
//}
void VulkanRaytracer::loadLayer( RenderLayer* alayer, VulkanLayerData& layerdata, VulkanTLAS* tlas ){
	VulkanRaytracerRenderLayer* layer = asserted( dynamic_cast<VulkanRaytracerRenderLayer*>( alayer ) );
	layerdata.first_instance_index = _instances.count();
//	for( Renderable* renderable : layer->renderables() ){
	for( VulkanRaytracerInstancePNT* renderable : layer->_instances ){
//		//add( _current_solid_tlas, renderable->posori(), renderable );
//		if( renderable->isPNT() )
		{
//			auto pntr = renderable->asPNT();
//			Mesh<VertexPNT>* mesh = pntr->mesh();
			Mesh<VertexPNT>* mesh = renderable->mesh();
			ASSERT( mesh->hasVulkanMesh() );
			VulkanMesh* vulkanmesh = mesh->vulkanMesh();
			ASSERT( vulkanmesh );
			//VulkanMaterial* material = dynamic_cast<VulkanMaterial*>( renderable->material() );
			//ASSERT( material );
//			//logDebug( "Raytracer check", vulkanmesh );
//			if( mesh->updateType() == AbstractMesh::UpdateImmediate ){
				_meshLoaderImmediate.tryUpdateIfNeeded( vulkanmesh );
//			} else {
//				_meshLoaderBackground.tryUpdateIfNeeded( vulkanmesh );
//			}
//			if( !vulkanmesh->hasCurrent() )
//				continue;
//			if( !vulkanmesh->current()->hasVulkanBLAS() )
//				continue;
//			//logDebug( "  a", vulkanmesh->current()->vulkanBLAS()->_rebuildingTLAS );
////				if( vulkanmesh->current()->vulkanBLAS()->_rebuildingTLAS )
////					continue;
			VulkanMeshDataPointer& current = vulkanmesh->current();
			VulkanMeshData* data = current.data();
			uint meshIndex = data->index();
			//int materialIndexi = this->_globals->materials().indexOf( material );
			//assert( 0 <= materialIndexi );
			//uint materialIndex = materialIndexi;
			uint materialIndex = renderable->material()->materialindex();
			//materials._materials.
			//uint materialIndex = pntr->material()->index();
//			//std::cout << "VulkanRaytracer::add " << meshIndex << " at " << glm::to_string( posori.position() ) << "\n";
//#ifdef ODTDEBUG
//			if( pntr->material()->hasFlag( MaterialFlag_NoShadowEmit ) ){
//				if( mesh->geometryFlags() & VK_GEOMETRY_OPAQUE_BIT_KHR ){
//					ASSERT( false );
//				}
//			}
//#endif
			const Mat4& transform = renderable->posori().matrix();
			uint32_t instanceIndex = _instances.add( transform, meshIndex, materialIndex, renderable->color(), renderable->textureOffsetY(), renderable->textureTile() );
			VulkanBLAS* blas = data->vulkanBLAS();
			ASSERT( blas );
//			//auto blas = _blases.blasesArray.at( meshIndex );
//			//logDebug( "Raytracer add", instanceIndex, materialIndex, renderable->posori().map( Vec3::Null ) );
			tlas->add( transform,
								instanceIndex,
								255, //material->raytracingFlags(), // mask
								0,  // shaderBindingTableRecordOffset
								0, // flags: VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;  // How to trace this instance
								blas
								);
//		} else {
//			assert( false );
		}
	}{
//		int todo_load_mb_task_is_wohl_der_falsche;
		tlas->addRebuild( _meshLoaderImmediate.loadMBTask(), _tasks.buildTLAS );
//	}{
		layerdata.first_light_index = _globals->_data.light_count;
		layerdata.light_count = 0;
		for( RenderLight* light : layer->_lights ){
			_lights.set( _globals->_data.light_count, *light );
			_globals->_data.light_count++;
			layerdata.light_count++;
		}
//	}{
		layerdata.first_decal_index = _globals->_data.decal_count;
		layerdata.decal_count = 0;
//		for( Decal* decal : layer->decals() ){
//			_globals->addDecal( *decal, _textures );
//			layerdata.decal_count++;
//		}
	}
}
void VulkanRaytracer::startCopyToFB( VkImage targetImage ){
	//logDebug( "startCopyToFB" );
	_pcs.copytofb->start();
	_tasks.copytofb.clear();
	_tasks.copytofb.create( "rt.copyToFB", VulkanTask::RunOnce, 10, computeCommandPool() );
	_tasks.copytofb.addWaitFor( _tasks.render );
	{
		VkImageMemoryBarrier barrier;
		_target.image.setImageMemoryBarrier( barrier,
			VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,  // Src and dst access masks
			VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,  // Src and dst layouts
			VK_IMAGE_ASPECT_COLOR_BIT);
		vkCmdPipelineBarrier( _tasks.copytofb.commandBuffer().vkCommandBuffer(),             // Command buffer
			VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, // Src and dst pipeline stages
			0,                     // Dependency flags
			0, nullptr,            // Global memory barriers
			0, nullptr,            // Buffer memory barriers
			1, &barrier);          // Image memory barriers
	}
//	if( _extension ){
//		assert( false ); //_extension->cmd2( commandBuffer );
//	}
	//	imageLinear.getData( []( void* data ){
	//		stbi_write_hdr("/home/rt/temp/space/out.hdr", render_width, render_height, 4, reinterpret_cast<float*>(data));
	//	});
	{
		//VkImage targetImage = null;
		//if( _window->addBeginAcquireNextRenderImage( _tasks.copytofb, targetImage ) )
		{
			assert( targetImage != null );
			{
				VkImageMemoryBarrier barrier;
				memset( &barrier, 0, sizeof( barrier ) );
				barrier.sType = {VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.image = targetImage;
				barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				barrier.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
				barrier.subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;
				vkCmdPipelineBarrier( _tasks.copytofb.commandBuffer().vkCommandBuffer(),             // Command buffer
					VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, // Src and dst pipeline stages
					0,                     // Dependency flags
					0, nullptr,            // Global memory barriers
					0, nullptr,            // Buffer memory barriers
					1, &barrier);          // Image memory barriers
			}{
				VkImageBlit region;
				memset( &region, 0, sizeof( region ) );
				region.srcSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
				region.srcSubresource.baseArrayLayer = 0;
				region.srcSubresource.layerCount     = 1;
				region.srcSubresource.mipLevel       = 0;
				region.srcOffsets[1]      = { (int)_target._size.width, (int)_target._size.height, 1};
				region.dstSubresource = region.srcSubresource;
				region.dstOffsets[1]      = { (int)_target._size.width, (int)_target._size.height, 1};
				vkCmdBlitImage(
							_tasks.copytofb.commandBuffer().vkCommandBuffer(),                             // Command buffer
							  _target.image.vkImage(),                           // Source image
							  VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,  // Source image layout
							  targetImage, //imageLinear.vkImage(),                     // Destination image
							VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, // VK_IMAGE_LAYOUT_GENERAL,
							//VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, // VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,  // Destination image layout
							  1, &region,
							VK_FILTER_NEAREST );                           // Regions
			}{
				VkImageMemoryBarrier barrier;
				memset( &barrier, 0, sizeof( barrier ) );
				barrier.sType = {VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
				barrier.srcAccessMask = 0;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.image = targetImage;
				barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				barrier.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
				barrier.subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;
				vkCmdPipelineBarrier( _tasks.copytofb.commandBuffer().vkCommandBuffer(),             // Command buffer
					VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, // Src and dst pipeline stages
					0,                     // Dependency flags
					0, nullptr,            // Global memory barriers
					0, nullptr,            // Buffer memory barriers
					1, &barrier);          // Image memory barriers
			}
		}
	}
	_tasks.copytofb.addOnCompleted( [this](){
		//logDebug( "copytofb completed" );
		_tasks.render.clear();
		_tasks.copytofb.clear();
	} );
	_tasks.copytofb.start();
	_pcs.copytofb->stop();
}
