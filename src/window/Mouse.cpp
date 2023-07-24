#include "stdodt.h"

#include "Mouse.h"
#include "Window.h"

Mouse::Mouse()
    :_window( null )
{
    for( int b = 0; b < ButtonCount; b++ ){
		_buttons_down[b] = false;
    }
}
Mouse::~Mouse(){
}
void Mouse::create( Window* window ){
    assert( window );
    assert( _window == null );
    _window = window;
}
bool Mouse::isButtonDown( int button ) const {
    assert( 0 <= button && button < ButtonCount );
	return _buttons_down[ button ];
}
bool Mouse::isButtonDownAndReset(int button){
	assert( 0 <= button && button < ButtonCount );
	bool down = _buttons_down[ button ];
	if( down ){
		_buttons_down[ button ] = false;
	}
	return down;
}
//float Mouse::getScrollOffsetYandReset(){
//	float y = _scroll_offset.y();
//	_scroll_offset.setY( 0 );
//	return y;
//}

//void Mouse::setPosition(const Vec2& position)
//{
//	_position = position;
//}
//void Mouse::setRay( const Ray& ray ){
//	_ray = ray;
//}
