#pragma once

//#include "object/ObjectRenderable.h"
//#include "math/Skeleton.h"
#include "SkinCore.h"
#include "Skin.h"

//class SkinRenderable : public RenderablePNT
//{
//private:
//	Skin* _skin;
//private:
//	int skipc = 10;
//	Task* _cpu_mesher = null;
//	SkinMesherExecutor* _gpu_mesher = null;
//	bool _gpu_mesher_waiting = false;
//public:
//	explicit SkinRenderable( Skin* skin );
//	virtual ~SkinRenderable() override;
//public:
//	virtual void render( Renderer& renderer ) override;
//private:
//	void mapMeshOnCPU();
//	void mapMeshOnCPUasTask();
//	void mapMeshOnGPU( VulkanRaytracer& raytracer );
//};

//class SkinRenderer : public ObjectRenderableProperty
//{
//private:
//	Skin* _skin;
//	SkinRenderable* _renderable = null;
//public:
//	explicit SkinRenderer( Skin* skin, Object* object );
//	virtual ~SkinRenderer() override;
//public:
//	Skin* skin(){ return _skin; }
//public:
//};

//class SkinSkeletonRenderer : public SkinRenderer
//{
//private:
//	Skeleton* _skeleton;
//public:
//	explicit SkinSkeletonRenderer( Skeleton* skeleton, Skin* skin, Object* object );
//	virtual ~SkinSkeletonRenderer() override;
//public:
//	virtual void animate( float dt ) override;
//public:
//	void load( Skeleton* skeleton );
//};
