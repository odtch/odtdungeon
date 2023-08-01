#pragma once

#include "renderer/RenderLayer.h"
#include "vulkan/VulkanCore.h"
#include "math/Camera.h"

class VulkanRaytracerRenderLayer : public RenderLayer
{
private:
	int _index = -1;
//private:
//	Renderer* _renderer = null;
//private:
//	Controller* _controller = null;
//private:
//	List<Renderable*> _renderables;
private:
	List<VulkanRaytracerInstancePNT*> _instances;
	List<RenderLight*> _lights;
//private:
//	List<Decal*> _decals;
//private:
//	List<RenderLight*> _lights;
private:
	VulkanRaytracerRenderLayer* _translucent_layer = null;
private:
	VulkanRaytracerRenderLayer* _nextLayer = null;
	uint _nextCameraAction = 0;
	Camera _nextCamera;
//	const Camera* _nextCamera = null;
//	SceneCameraNode* _nextCameraNode = null;
//	Mat4 _nextCameraTransform;
public:
	explicit VulkanRaytracerRenderLayer();
	virtual ~VulkanRaytracerRenderLayer() override;
public:
	int index() const{ ASSERT( 0 <= _index ); return _index; }
public:
	virtual void setNextFixedCamera( const Camera& nextCamera ) override;
	//public:
	//	int index() const{ ASSERT( 0 <= _index ); return _index; }
	//public:
	//	Renderer* renderer(){ ASSERT( _renderer ); return _renderer; }
	//	void bindRenderer( Renderer* renderer );
	//	void unbindRenderer( Renderer* renderer );
	//public:
	//	bool hasController() const{ return _controller != null; }
	//	Controller* controller() const{ assert( _controller ); return _controller; }
	//	void setController( Controller* controller );
	//public:
	//	const List<Renderable*>& renderables() const{ return _renderables; }
	//	void addRenderable( Renderable* renderable );
	//	void removeRenderable( Renderable* renderable );
	//public:
	//	const List<Decal*>& decals() const{ return _decals; }
	//	void addDecal( Decal* decal );
	//	void removeDecal( Decal* decal );
	//public:
	//	const List<RenderLight*>& lights() const{ return _lights; }
	//	RenderLight* createAmbientLight( const vec4& color );
	//	RenderLight* createDirectionalLight( const Vec3& direction, const vec4& color );
	//	RenderLight* createPointLight( const Vec3& position, float range, const vec4& color );
	//	RenderLight* createSpotLight( const Vec3& position, const Vec3& direction, float angle, float range, const vec4& color );
	//	void addLight( RenderLight* light );
	//	void removeLight( RenderLight* light );
	public:
		bool hasTranslucentLayer() const{ return _translucent_layer != null; }
		VulkanRaytracerRenderLayer* translucentLayer() const{ ASSERT( _translucent_layer ); return _translucent_layer; }
		VulkanRaytracerRenderLayer* createTranslucentLayer();
	public:
//		bool hasNextLayer() const{ return _nextLayer != null; }
	//	RenderLayer* nextLayer() const{ assert( _nextLayer ); return _nextLayer; }
	//	void setNextLayer( RenderLayer* nextlayer );
	//public:
	//	uint nextCameraAction() const { return _nextCameraAction; }
	//	void setNextCameraAction2Dto3D();
	//public:
	//	void setNextCameraTransform( const Mat4& transform );
	//	const Mat4& nextCameraTransform() const { ASSERT( _nextCameraAction == 2 ); return _nextCameraTransform; }
	//public:
	//	bool hasNextCamera() const{ return _nextCamera != null; }
	//	const Camera* nextCamera() const{ assert( _nextCamera ); return _nextCamera; }
	//	virtual void setNextFixedCamera( const Camera& nextCamera ) = 0;
	//public:
	//	bool hasNextCameraNode() const{ return _nextCameraNode != null; }
	//	const SceneCameraNode* nextCameraNode() const{ assert( _nextCameraNode ); return _nextCameraNode; }
	//	void setNextCameraNode( SceneCameraNode* nextCameraNode );
	//public:
	//	virtual void prerender();
public:
	friend class VulkanRaytracer;
};

