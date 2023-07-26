#pragma once

#include "SceneProperty.h"
#include "SceneObject.h"
#include "renderer/RenderCore.h"

class SceneRenderInstanceProperty : public SceneProperty
{
private:
	SceneObject* _object;
	PosOri _posori;
	RenderInstance* _instance = null;
public:
	explicit SceneRenderInstanceProperty( SceneObject* object );
	virtual ~SceneRenderInstanceProperty() override;
protected:
	virtual void onAddedToScene( Scene* scene ) override;
	virtual void onRemovedFromScene( Scene* scene ) override;
protected:
	const PosOri& posori() const{ return _posori; }
private:
	void recalcPosOri();
protected:
	virtual RenderInstance* createInstance( Renderer& renderer, RenderLayer* layer ) = 0;
protected:
	virtual void animate( float dt ) override;
};

class SceneRenderInstancePNTProperty : public SceneRenderInstanceProperty
{
private:
	MeshPNT* _mesh;
	Material* _material;
	//RenderInstancePNT* _instance = null;
public:
	explicit SceneRenderInstancePNTProperty( MeshPNT* mesh, Material* material, SceneObject* object );
	virtual ~SceneRenderInstancePNTProperty() override;
protected:
	virtual RenderInstance* createInstance( Renderer& renderer, RenderLayer* layer ) override;
};
