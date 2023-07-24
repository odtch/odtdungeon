#pragma once

#include "WindowCore.h"

class WindowListener
{
protected:
	virtual void onWindowCreated( Window* window ) = 0;
	virtual void onWindowDestroy() = 0;
protected:
	virtual void onWindowIdle( float dt ) = 0;
	virtual void onWindowResized( int width, int height ) = 0;
public:
	friend class Window;
};

class AbstractWindowListener : public WindowListener
{
	DEBUGCOUNTER( AbstractWindowListener )
public:
	explicit AbstractWindowListener();
	virtual ~AbstractWindowListener();
protected:
	virtual void onWindowCreated( Window* window ) override;
	virtual void onWindowDestroy() override;
protected:
	virtual void onWindowIdle( float dt ) override;
	virtual void onWindowResized( int width, int height ) override;
};
