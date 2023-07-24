#pragma once

#include "WindowCore.h"

class Mouse
{
private:
	Window* _window;
private:
	enum { ButtonCount = 4 };
	bool _buttons_down[ ButtonCount ];
//private:
//	Vec2 _scroll_offset;
private:
//	Vec2 _position_in_window;
private:
//	Ray _ray;
public:
	explicit Mouse();
	virtual ~Mouse();
private:
	void create( Window* window );
public:
	Window* window(){ assert( _window ); return _window; }
public:
	bool isButtonDown( int button ) const;
	bool isButtonDownAndReset( int button );
//public:
//	float getScrollOffsetYandReset();
public:
//	const Vec2& position_in_window() const{ return _position_in_window; }
//	void setPosition( const Vec2& position );
public:
//	const Ray& ray() const{ return _ray; }
//	void setRay( const Ray& ray );
public:
	friend class Window;
};

