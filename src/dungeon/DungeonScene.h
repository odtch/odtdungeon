#pragma once

#include "scene/Scene.h"

class DungeonScene : public Scene
{
public:
    explicit DungeonScene( Renderer* renderer );
    virtual ~DungeonScene() override;
protected:
    virtual void run() override;
};
