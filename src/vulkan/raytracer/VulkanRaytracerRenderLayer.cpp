#include "stdodt.h"

#include "VulkanRaytracerRenderLayer.h"
#include "VulkanRaytracerData.h"
#include "VulkanRaytracerInstance.h"
#include "renderer/RenderInstance.h"
#include "renderer/RenderLight.h"

VulkanRaytracerRenderLayer::VulkanRaytracerRenderLayer(){
}
VulkanRaytracerRenderLayer::~VulkanRaytracerRenderLayer(){
	_instances.deleteAll();
	_lights.deleteAll();
	//	ASSERT( _renderer == null );
	//	ASSERT( _renderables.isEmpty() );
	//	_lights.deleteAll();
	//	if( _translucent_layer ){
	//		//odelete( _translucent_layer );
	//		_translucent_layer = null;
	//	}
}
void VulkanRaytracerRenderLayer::setNextFixedCamera( const Camera& nextCamera ){
	_nextCamera = nextCamera;
	_nextCameraAction = VulkanLayer_NextCameraAction_2D_to_3D;
}



//void RenderLayer::bindRenderer( Renderer* renderer ){
//	ASSERT( renderer );
//	ASSERT( _renderer == null );
//	ASSERT( _translucent_layer == null );
//	_renderer = renderer;
//	for( Renderable* renderable : _renderables ){
//		_renderer->onRenderableAdded( renderable );
//	}
//}
//void RenderLayer::unbindRenderer( Renderer* renderer ){
//	ASSERT( _renderer == renderer );
//	ASSERT( _renderer );
//	for( Renderable* renderable : _renderables ){
//		_renderer->onRenderableRemoved( renderable );
//	}
//	_renderer = null;
//	if( _translucent_layer ){
//		renderer->removeLayer( _translucent_layer );
//		odelete( _translucent_layer );
//		_translucent_layer = null;
//	}
//}
//void RenderLayer::setController( Controller* controller ){
//	_controller = controller;
//}
//void RenderLayer::addRenderable( Renderable* renderable ){
//	ASSERT( renderable );
//	if( renderable->isPNT() ){
//		ASSERT( 2 < renderable->asPNT()->mesh()->vertexCount() );
//		ASSERT( 2 < renderable->asPNT()->mesh()->indexCount() );
//	}
//	_renderables.add( renderable, true );
//	if( _renderer ){
//		_renderer->onRenderableAdded( renderable );
//	}
//}
//void RenderLayer::removeRenderable( Renderable* renderable ){
//	_renderables.remove( renderable );
//	if( _renderer ){
//		_renderer->onRenderableRemoved( renderable );
//	}
//}
//void RenderLayer::addDecal( Decal* decal ){
//	_decals.add( decal );
//}
//void RenderLayer::removeDecal( Decal* decal ){
//	_decals.remove( decal );
//}
//RenderLight* RenderLayer::createAmbientLight( const vec4& color ){
//	RenderLight* light = RenderLight::CreateAmbient( color );
//	addLight( light );
//	return light;
//}
//RenderLight* RenderLayer::createDirectionalLight( const Vec3& direction, const vec4& color ){
//	RenderLight* light = RenderLight::CreateDirectional( direction, color );
//	addLight( light );
//	return light;
//}
//RenderLight* RenderLayer::createPointLight( const Vec3& position, float range, const vec4& color ){
//	RenderLight* light = RenderLight::CreatePoint( position, range, color );
//	addLight( light );
//	return light;
//}
//RenderLight* RenderLayer::createSpotLight( const Vec3& position, const Vec3& direction, float angle, float range, const vec4& color ){
//	RenderLight* light = RenderLight::CreateSpot( position, direction, angle, range, color );
//	addLight( light );
//	return light;
//}
//void RenderLayer::addLight( RenderLight* light ){
//	assert( light );
//	_lights.add( light );
//}
//void RenderLayer::removeLight( RenderLight* light ){
//	_lights.remove( light );
//}
//RenderLayer* RenderLayer::createTranslucentLayer(){
//	ASSERT( _translucent_layer == null );
//	assert( _renderer );
//	_translucent_layer = new RenderLayer();
//	_renderer->addLayer( _translucent_layer );
//	return _translucent_layer;
//}
//void RenderLayer::setNextLayer( RenderLayer* nextlayer ){
//	if( _nextLayer == null ){
//		assert( nextlayer );
//	} else {
//		assert( nextlayer == null );
//	}
//	_nextLayer = nextlayer;
//}
//void RenderLayer::setNextCameraAction2Dto3D(){
//	assert( _nextCameraAction == 0 );
//	_nextCameraAction = 1;
//}
//void RenderLayer::setNextCameraTransform( const Mat4& transform ){
//	assert( _nextCameraAction == 0 || _nextCameraAction == 2 );
//	_nextCameraAction = 2;
//	_nextCameraTransform = transform;
//}
//void RenderLayer::setNextCamera( const Camera* nextCamera ){
//	assert( nextCamera );
//	assert( _nextCamera == null );
//	_nextCamera = nextCamera;
//}
//void RenderLayer::setNextCameraNode( SceneCameraNode* nextCameraNode ){
//	assert( nextCameraNode );
//	assert( _nextCameraNode == null );
//	_nextCameraNode = nextCameraNode;

//}
//void RenderLayer::prerender(){
//	if( _nextCameraNode ){
//		_nextCamera = _nextCameraNode->camera();
//		SceneArea* area = _nextCameraNode->area();
//		ASSERT( area );
//		RenderLayer* nextlayer = area->layer();
//		ASSERT( nextlayer );
//		if( nextlayer != _nextLayer ){
//			logDebug( "RenderLayer::prerender switch to area", area->name() );
//			_nextLayer = nextlayer;
//		}
//	}
//}
