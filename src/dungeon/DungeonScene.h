#pragma once

#include "scene/Scene.h"

class DungeonScene : public Scene
{
private:
	RenderLayer* _uilayer = null;
	Camera* _camera = null;
	RenderInstancePNT* _box1 = null;
public:
    explicit DungeonScene( Renderer* renderer );
    virtual ~DungeonScene() override;
public:
	virtual void animate( float dt ) override;
protected:
    virtual void run() override;
};
