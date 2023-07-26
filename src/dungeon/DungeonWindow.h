#pragma once

#include "window/Window.h"

#include "DungeonScene.h"
class DungeonScene;

class DungeonWindow : public Window
{
private:
	DungeonScene* _scene = null;
public:
	explicit DungeonWindow();
	virtual ~DungeonWindow() override;
public:
	void setScene( DungeonScene* scene );
public:
	virtual void onKey( int key, int scancode, int action, int mods ) override;
};
