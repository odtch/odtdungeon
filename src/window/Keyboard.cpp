#include "stdodt.h"

#include "Keyboard.h"
#include "Window.h"

#include <GLFW/glfw3.h>

Keyboard::Keyboard()
	:_window( null )
{
	assert( GLFW_KEY_LAST == LastKey );
	for( int k = 0; k < LastKey; k++ ){
		_pressed_keys[ k ] = false;
	}
	ASSERT( Keys::A == GLFW_KEY_A );
}
Keyboard::~Keyboard(){
}
void Keyboard::create( Window* window ){
	assert( window );
	assert( _window == null );
	_window = window;
}
void Keyboard::onKey( int key, int scancode, int action, int mods ){
	assert( 0 <= key && key < LastKey );
	switch( action ){
	case GLFW_PRESS:
		//std::cout << "key.press k=" << key << " s=" << scancode << " m=" << mods << "\n";
		_pressed_keys[ key ] = true;
		break;
	case GLFW_REPEAT:
		break;
	case GLFW_RELEASE:
		_pressed_keys[ key ] = false;
		//std::cout << "key.relea k=" << key << " s=" << scancode << " m=" << mods << "\n";
		break;
	default:
		logError( "Keyboard::onKey unhandled action", action );
		break;
	}
}
bool Keyboard::isKeyDown( Keys::Key key ) const {
	/*
	int state = glfwGetKey( _window->glfWindow(), key );
	if( state == GLFW_RELEASE ){
		return false;
	} else if( state == GLFW_PRESS ){
		return true;
	} else {
		assert( false );
		return false;
	}
	*/
	assert( 0 <= key && ((int)key) < LastKey );
	return _pressed_keys[ key ];
}
bool Keyboard::isKeyDownAndRelease( Keys::Key  key ){
	assert( 0 <= key && ((int)key) < LastKey );
	bool p = _pressed_keys[ key ];
	_pressed_keys[ key ] = false;
	return p;
}
bool Keyboard::isShiftDown(){
	if( _pressed_keys[ GLFW_KEY_LEFT_SHIFT ] )
		return true;
	if( _pressed_keys[ GLFW_KEY_RIGHT_SHIFT ] )
		return true;
	return false;
}
