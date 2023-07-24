#include "stdodt.h"

#include "WindowListener.h"
#include "Window.h"

AbstractWindowListener::AbstractWindowListener(){
}
AbstractWindowListener::~AbstractWindowListener(){
}
void AbstractWindowListener::onWindowCreated( Window* window ){
	OUNUSED( window );
}
void AbstractWindowListener::onWindowDestroy(){
}
void AbstractWindowListener::onWindowIdle( float dt ){
	OUNUSED( dt );
}
void AbstractWindowListener::onWindowResized( int width, int height ){
	OUNUSED( width );
	OUNUSED( height );
}
