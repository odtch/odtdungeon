#include "stdodt.h"

#include "Window.h"
//#include "Viewport.h"
//#include "renderer/OdtRender.h"
//#include "controller/Controller.h"

#define GLFW_INCLUDE_VULKAN
#define VK_ENABLE_BETA_EXTENSIONS
#include <GLFW/glfw3.h>

Window* getWindowFromGlf( GLFWwindow* glfwindow ){
	auto window = reinterpret_cast< Window*>( glfwGetWindowUserPointer( glfwindow ) );
	assert( window );
	return window;
}
void windowResizeCallback( GLFWwindow* glfwindow, int width, int height ){
	//logDebug( "windowResizeCallback", width, height );
	getWindowFromGlf( glfwindow )->onResized( width, height );
}
void windowKeyCallback( GLFWwindow* glfwindow, int key, int scancode, int action, int mods ){
	getWindowFromGlf( glfwindow )->onKey( key, scancode, action, mods );
}
void windowCursorPositionCallback( GLFWwindow* glfwindow, double xpos, double ypos ){
	getWindowFromGlf( glfwindow )->onMouseMove( xpos, ypos );
}
void windowMouseButtonCallback( GLFWwindow* glfwindow, int button, int action, int mods ){
	getWindowFromGlf( glfwindow )->onMouseButton( button, action, mods );
}
void windowScrollCallback( GLFWwindow* glfwindow, double xoffset, double yoffset ){
	getWindowFromGlf( glfwindow )->onMouseScroll( xoffset, yoffset );
}
void windowFocusCallback( GLFWwindow* glfwindow, int focused ){
	//logDebug( "wfc", focused );
	if( focused == 0 ){
		getWindowFromGlf( glfwindow )->onDeactivated();
	} else {
		getWindowFromGlf( glfwindow )->onActivated();
	}
}


Window::Window(){
//	_idle_pc = PerformanceCounter::Create( "Idle dt" );
//	_idle_pc->setTypeTicked();
//	_frame_pc = PerformanceCounter::Create( "Full Frame" );
//	_frame_pc->start();
//	//_idle_pc->start();
//	_idleRange.set( 1.0f / 390.0f, 1.0f / 30.0f );
}
Window::~Window(){
//	ASSERT( _active_viewport == null );
	ASSERT( _window == null );
	ASSERT( _listeners.isEmpty() );
//	_renderer = null;
}
//void Window::setRenderer( Renderer* renderer ){
//	assert( renderer );
//	assert( _renderer == null );
//	_renderer = renderer;
//}
void Window::addListener( WindowListener* listener ){
	assert( listener );
	_listeners.add( listener );
}
void Window::removeListener( WindowListener* listener ){
	_listeners.remove( listener );

}
void Window::create( int width, int height, const String& title ){
	assert( 0 < width && 0 < height );
	assert( _window == null );
	_title = title;
	_width = width;
	_height = height;
	if( GLFW_TRUE != glfwInit() )
		throw std::runtime_error( "glfwInit failed" );
	glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
	_window = glfwCreateWindow( width, height, title.asCStr(), nullptr, nullptr );
	if( _window == nullptr )
		throw std::runtime_error( "glfwCreateWindow failed" );
	glfwSetWindowUserPointer( _window, this );
	glfwSetFramebufferSizeCallback( _window, windowResizeCallback );
	_keyboard.create( this );
	glfwSetKeyCallback( _window, windowKeyCallback);
	_mouse.create( this );
	//glfwSetInputMode( _window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );
	glfwSetCursorPosCallback( _window, windowCursorPositionCallback );
	glfwSetMouseButtonCallback( _window, windowMouseButtonCallback );
	glfwSetScrollCallback( _window, windowScrollCallback );
	glfwSetWindowFocusCallback( _window, windowFocusCallback );
//	_scheduler = new JobScheduler();
	for( WindowListener* listener : _listeners ){
		listener->onWindowCreated( this );
	}
}
//void Window::sendOnDestroyed(){
//	int prev_count = _listeners.size();
//	for( WindowListener* listener : _listeners ){
//		listener->onWindowDestroy();
//		if( prev_count != _listeners.size() ){
//			sendOnDestroyed();
//			return;
//		}
//	}
//}
void Window::destroy(){
	if( _active ){
		onDeactivated();
	}
//	if( _scheduler ){
//		_scheduler->destroy();
//	}
//	_active_viewport = null;
//	if( _viewport ){
//		_viewport->unbindWindow();
//		_viewport->destroy();
//		delete _viewport;
//		_viewport = null;
//	}
//	_scenes.deleteAll();
	while( !_listeners.isEmpty() ){
		WindowListener* listener = _listeners.last();
		listener->onWindowDestroy();
		assert( !_listeners.contains( listener ) );
	}
//	while( !_viewports.isEmpty() ){
//		Viewport* viewport = _viewports.takeLast();
//		delete viewport;
//	}
	if( _window ){
		glfwDestroyWindow( _window );
		_window = null;
		glfwTerminate();
	}
}
void Window::main(){
	assert( _window );
	int error_count = 0;
	while( !glfwWindowShouldClose( _window ) ){
		try{
			//glfwPostEmptyEvent();
			//glfwWaitEvents();
			//for( int x = 0; x < 200; x++ )
			glfwPollEvents();
			onIdle();
		} catch( const std::exception& ex ){
			logError( "Window::main failed", ex.what() );
			error_count++;
			if( 6 < error_count ){
				throw std::runtime_error( "Window::main failed too many times" );
			}
		} catch( ... ){
			logError( "Window::main failed unexpected" );
		}
	}
}
void Window::setMouseCursorVisible( bool visible ){
	assert( _window );
	if( visible ){
		glfwSetInputMode( _window, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
	} else {
		glfwSetInputMode( _window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );
	}
}
float Window::avg_fps() const{
    assert( false );
    //return _frame_pc->avg_fps();
}
void Window::onNewFrame(){
//	_frame_pc->stop();
//	PerformanceCounter::NewFrame();
//	_frame_pc->start();
}
//Viewport* Window::createViewport(){
//	Viewport* viewport = new Viewport( this );
//	_viewports.add( viewport );
//	return viewport;
//}
//Viewport* Window::findViewport( const Vec2& position_in_window ) const {
//	for( auto viewport : _viewports ){
//		if( viewport->position().lt.x <= position_in_window.x()
//			&& position_in_window.x() <= viewport->position().rb.x
//			&& viewport->position().lt.y <= position_in_window.y()
//			&& position_in_window.y() <= viewport->position().rb.y
//		){
//			return viewport;
//		}
//	}
//	return null;
//}
//void Window::setMousePosition( int x, int y ){
//	glfwSetCursorPos( _window, x, y );
//}
//void Window::setViewport( Viewport* viewport ){
//	assert( viewport );
//	assert( _viewport == null );
//	_viewport = viewport;
//	_viewport->bindWindow( this );
//	_viewport->setRect( Rect2i( 0, 0, _size.width, _size.height ) );
//}

//void Window::addScene( Scene* scene ){
//	_scenes.add( scene );
//}
//void Window::removeScene( Scene* scene ){
//	_scenes.remove( scene );
//}
////void Window::recalcViewports(){
////	_viewports.setRect( Rect2i( 0, 0, _size.width, _size.height ) );
////	_viewports_list.removeAll();
////	_viewports.getValidViewports( _viewports_list );
////}
////void Window::setController( Controller* controller ){
////	assert( controller );
////	assert( _controller == null );
////	assert( _viewer );
////	_controller = controller;
////	_controller->create( _viewer );
////}
////void Window::setViewer( Viewer* viewer ){
////	assert( viewer );
////	assert( _viewer == null );
////	_viewer = viewer;
////	_viewer->create( this );
////}
void Window::onResized( int width, int height ){
	_width = width;
	_height = height;
//	if( _viewport ){
//		_viewport->setRect( Rect2i( 0, 0, _width, _height ) );
//	}
	for( WindowListener* listener : _listeners ){
		listener->onWindowResized( width, height );
	}
}
void Window::onIdle(){
	//logDebug( "Window.onIdle" );
	//std::cout << "W::oI\n"; std::cout.flush();
//	if( _first_on_idle ){
//		_prev_idle_timepoint = std::chrono::high_resolution_clock::now();
//		_first_on_idle = false;
//		_first_rest_time = 0.0f;
//		return;
//	}
//	auto currentTime = std::chrono::high_resolution_clock::now();
//	float dt = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - _prev_idle_timepoint ).count();
//	if( dt < 1.0f / 500.0f )
//		return;
//	if( 0 < _first_rest_time ){
//		_first_rest_time -= dt;
//		dt = 1.0f / 60;
//	}
//	const float max_dt = 1.0f / 30;
//	if( dt >= max_dt ){
//		if( 0 < _first_rest_time ){
//		} else {
//			logInfo( "dt > big", dt );
//		}
//		dt = max_dt;
//	}
////	if( dt < _idleRange.min() )
////		return; // das funktioniert noch nicht so wie gewollt
////	if( _idleRange.max() < dt )
////		dt = _idleRange.max();
//	_idle_pc->add_s( dt );
//	_prev_idle_timepoint = currentTime;
//	assert( 0 < dt );
//	//std::cout << "W::oI c\n"; std::cout.flush();
//	if( _active ){
//		assert( _renderer );
//		bool handled = false;
//		//logDebug( "--" );
//		for( RenderLayer* layer : _renderer->layers() ){
//			if( handled ){
//			} else if( !layer->hasController() ){
//			} else {
//				//const
//						Camera& camera = layer->controller()->camera();
//				vec2 fov = camera.fov();
//				fov.y = ( fov.x * _height ) / _width;
////				if( 1 < fov.y )
//				//logDebug( "fov", fov.x, fov.y );
//				{
////					Camera& camera = _viewer->camera();
//					//camera.setFov( fov );
//					//camera.recalcMatrices( vec2( _width, _height ) );
////					float screen_pos_perc_x = ( ( -1.0f + 2.0f * _mouse.position_in_window().x() / _width ) );
////					float screen_pos_perc_y = ( ( -1.0f + 2.0f * _mouse.position_in_window().y() / _height) );
//					float next_camera_2d_fov_size = fmax( _width, _height );
//					Vec3 cd( - ( _width * 0.5f ) + _mouse.position_in_window().x(),
//							 next_camera_2d_fov_size,
//							 ( _height * 0.5f ) - _mouse.position_in_window().y() );
//					//logDebug( "wc", cd );
//					cd.normalize();

//					Vec3 camera_position = camera.position();
//					Vec3 camera_direction = cd;
//					camera_direction = camera.posori().matrix().mapNormal( cd );

//					_mouse._ray.set( camera_position, camera_direction );
////					Vec3 screen_pos_perc( screen_pos_perc_x, screen_pos_perc_y, next_camera_2d_fov_size );
////					Vec3 unproj_pos = camera.projection_matrix().inverted().map( screen_pos_perc );
////					Vec3 unview_pos = camera.view_matrix().inverted().map( unproj_pos );
////					Vec3 dir = unview_pos - camera.posori().position();
//					//dir = Vec3( -0.404462f, 1.000000f, -11277916438661009768448.000000f );
//					// camera_direction = vec3( -( globals.target_half_width - pixelx ),
//					// globals.layers[ layerindex ].next_camera_2d_fov_size,
//					// ( globals.target_half_height - pixely ) );


////					logDebug( "w.oi cr", camera.position(), unview_pos, dir );
////					float dirl2 = dir.length2();
////					if( dirl2 < 0.01f ){
////						logError( "Window.onIdle calc ray dir < 0", dirl2 );
////					} else if( dirl2 > 1000000 ){
////						logError( "Window.onIdle calc ray dir > m", dirl2 );
////					} else{
////						//std::cout << "Window::onIdle set mouse ray a " << dir.toString() << "\n";
////						dir.normalize();
////						logDebug( "    d", dir );
////						_mouse._ray.set( camera.posori().position(), dir );
////						//logDebug( "w.cr", camera.position(), camera.direction(), _mouse._ray.origin(), _mouse._ray.direction() );
////					}
//				}
//				if( !layer->controller()->control( dt ) ){
//					handled = true;
//				}
//			}
//		}
//	}
//	int todo_rm;
	//std::cout << "W::oI li\n"; std::cout.flush();
    float dt = 0;
	for( auto listener : _listeners ){
		listener->onWindowIdle( dt );
	}
//		_scheduler->tick( dt );
//		if( _viewport ){
//			_viewport->control( dt, _mouse, _keyboard );
//		}
//		for( Scene* scene : _scenes ){
//			if( scene->pause_animate ){
//			} else {
//				scene->animate( dt );
//			}
//		}
//		*/
}
void Window::onActivated(){
	//logDebug( "Window::onActivated" );
	ASSERT( _active == false );
	_active = true;
}
void Window::onDeactivated(){
	//logDebug( "Window::onDeactivated" );
	ASSERT( _active == true );
	_active = false;
}
void Window::onKey( int key, int scancode, int action, int mods ){
	_keyboard.onKey( key, scancode, action, mods );
}
void Window::onMouseMove( double mx, double my ){
    //logDebug( "Window::onMouseMove", mx, my );
	//std::cout << "Window::onMouseMove " << glm::to_string( mouse_pos ) << "\n";
    //_mouse._position_in_window = Vec2( mx, my );
//	Point2i ipos( (int) mx, (int) my );
//	for( auto viewport : _viewports ){
//		if( viewport->position().contains( ipos ) ){
//			Point2i ipos_in_viewport( ipos.x - viewport->position().lt.x, ipos.y - viewport->position().lt.y );
//			Vec2 pos_in_viewport( ipos_in_viewport.x, ipos_in_viewport.y );
////			pos_in_viewport.data.x /= viewport->position().size.x;
////			pos_in_viewport.data.y /= viewport->position().size.y;
////			pos_in_viewport.data.x = -0.5f + pos_in_viewport.data.x;
////			pos_in_viewport.data.y = -0.5f + pos_in_viewport.data.y;
//			float fx = pos_in_viewport.data.x / viewport->position().size.x;
//			fx = 1.0f - fx;
//			float ifx = 1.0f - fx;
//			float fy = pos_in_viewport.data.y / viewport->position().size.y;
//			fy = 1.0f - fy;
//			float ify = 1.0f - fy;
//			//logDebug( "f", fx, ifx, fy, ify );
//			const Camera& camera = viewport->viewer()->camera();
//			Vec3 pos_on_near_plane =
//			( camera.near_plane_lt() * fx
//			+ camera.near_plane_rt() * ifx ) * fy
//					+ (
//			camera.near_plane_lb() * fx
//			+camera.near_plane_rb() * ifx ) * ify;
//			_mouse._ray.set( camera.position(), ( pos_on_near_plane - camera.position() ).normalized() );



//			//logDebug( "pvp", pos_on_near_plane );

//			return;
//		}
//	}
}
void Window::onMouseButton( int button, int action, int mods ){
	OUNUSED(mods);
	assert( 0 <= button && button < Mouse::ButtonCount );
	if( action == GLFW_PRESS ){
		_mouse._buttons_down[ button ] = true;
	} else if( action == GLFW_RELEASE ){
		_mouse._buttons_down[ button ] = false;
	} else {
		assert( false );
	}
//	std::cout << "Window::onMouseButton " << button << " ,  " << action << " "
//<< ( action == GLFW_PRESS ? "press" : "" )
//<< ( action == GLFW_RELEASE ? "release" : "" )
	//<< ", " << mods << "\n";
}
void Window::onMouseScroll( float xo, float yo ){
	logDebug( "Window::onMouseScroll", xo, yo );
	//_mouse._scroll_offset = Vec2( xo, yo );
}
