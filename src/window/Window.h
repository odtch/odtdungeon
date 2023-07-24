#pragma once

#include "WindowCore.h"
#include "Keyboard.h"
#include "Mouse.h"

class Window
{
private:
	GLFWwindow* _window = null;
private:
	String _title;
private:
	int _width = 0;
	int _height = 0;
private:
	bool _active = false;
private:
	Keyboard _keyboard;
	Mouse _mouse;
//private:
//	Renderer* _renderer = null;
//private:
//	PerformanceCounter* _frame_pc = null;
//private:
//	PerformanceCounter* _idle_pc = null;
//	FloatRange _idleRange;
//	bool _first_on_idle = true;
//	float _first_rest_time = 0;
//	std::chrono::high_resolution_clock::time_point _prev_idle_timepoint;
private:
	MessageListeners _listeners;
//private:
//	JobScheduler* _scheduler = null;
//private:
//	Collections _collections;
private:
//	List<Viewport*> _viewports;
//	Viewport* _viewport = null;
//private:
//	Viewport* _active_viewport = null;
//private:
//	List<Scene*> _scenes;
public:
	explicit Window();
//	//RenderSystem* renderSystem );
	virtual ~Window();
public:
//	void setRenderer( Renderer* renderer );
public:
	void addListener( MessageListener* listener );
	void removeListener( MessageListener* listener );
//public:
//	const Collections& collections() const { return _collections; }
//	Collections& collections(){ return _collections; }
public:
	void create( int width, int height, const String& title );
	//void sendOnDestroyed();
	void destroy();
public:
	void main();
public:
	GLFWwindow* glfWindow() const{ assert( _window ); return _window; }
public:
	const String& title() const{ return _title; }
public:
	int width() const{ return _width; }
	int height() const{ return _height; }
//	const VkExtent2D& size() const { return _size; }
public:
	Keyboard& keyboard(){ return _keyboard; }
public:
	Mouse& mouse(){ return _mouse; }	
//	void setMousePosition( int x, int y );
	void setMouseCursorVisible( bool visible );
////	RenderSystem* renderSystem() const{ return _renderSystem; }
public:
//	const List<Viewport*>& viewports() const{ return _viewports; }
//	Viewport* createViewport();
//	Viewport* findViewport( const Vec2& position_in_window ) const;
//	const Viewport& viewport() const { assert( _viewport ); return *_viewport; }
//	Viewport& viewport(){ assert( _viewport ); return *_viewport; }
//	void setViewport( Viewport* viewport );
//	//Viewport& viewport() { return _viewport; }
//public:
//	bool hasActiveViewport() const{ return _active_viewport != null; }
//	Viewport* activeViewport(){ assert( _active_viewport ); return _active_viewport; }
//	//void setController( Controller* controller );
////	Viewer* viewer(){ assert( _viewer ); return _viewer; }
////	void setViewer( Viewer* viewer );
//public:
//	void addScene( Scene* scene );
//	void removeScene( Scene* scene );
//	const List<Scene*>& scenes() const{ return _scenes; }
public:
	float avg_fps() const;
	void onNewFrame();
public:
	virtual void onResized( int width, int height );
	virtual void onIdle();
public:
	virtual void onActivated();
	virtual void onDeactivated();
	virtual void onKey( int key, int scancode, int action, int mods );
	virtual void onMouseMove( double mx, double my );
	virtual void onMouseButton( int button, int action, int mods );
	virtual void onMouseScroll( float xo, float yo );
//public:
//	friend class Keyboard;
};

