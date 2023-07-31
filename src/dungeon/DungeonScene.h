#pragma once

#include "scene/Scene.h"
#include "scene/SceneObject.h"

class DungeonScene : public Scene
{
public:
	struct {
		float cameraMoveX = 0;
		float cameraMoveY = 0;
		float cameraMoveZ = 0;
		float cameraRotateX = 0;
		float cameraRotateY = 0;
		float cameraRotateZ = 0;
	} control;
private:
	RenderLayer* _uilayer = null;
	Camera* _camera = null;
private:
	SceneArea* _area1 = null;
public:
    explicit DungeonScene( Renderer* renderer );
    virtual ~DungeonScene() override;
public:
	virtual void animate( float dt ) override;
protected:
    virtual void run() override;
private:
	void createStaticBox( const Vec3& c, const Vec3& r, Material* material );
};
