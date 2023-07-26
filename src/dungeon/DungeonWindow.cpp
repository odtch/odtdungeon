#include "stdodt.h"

#include "DungeonWindow.h"
#include "DungeonScene.h"

DungeonWindow::DungeonWindow(){
}
DungeonWindow::~DungeonWindow(){
}
void DungeonWindow::setScene( DungeonScene* scene ){
	_scene = asserted( scene );
}
void DungeonWindow::onKey( int key, int scancode, int action, int mods ){
	Window::onKey( key, scancode, action, mods );
	_scene->control.cameraMoveX = ( keyboard().isKeyDown( Keys::A ) ? -1 : 0 ) + ( keyboard().isKeyDown( Keys::D ) ? 1 : 0 );
	_scene->control.cameraMoveY = ( keyboard().isKeyDown( Keys::S ) ? -1 : 0 ) + ( keyboard().isKeyDown( Keys::W ) ? 1 : 0 );
	_scene->control.cameraMoveZ = ( keyboard().isKeyDown( Keys::Z ) ? -1 : 0 ) + ( keyboard().isKeyDown( Keys::Q ) ? 1 : 0 );
	_scene->control.cameraRotateX = ( keyboard().isKeyDown( Keys::UP ) ? 1 : 0 ) + ( keyboard().isKeyDown( Keys::DOWN ) ? -1 : 0 );
	_scene->control.cameraRotateY = ( keyboard().isKeyDown( Keys::PAGE_UP ) ? 1 : 0 ) + ( keyboard().isKeyDown( Keys::PAGE_DOWN ) ? -1 : 0 );
	_scene->control.cameraRotateZ = ( keyboard().isKeyDown( Keys::LEFT ) ? 1 : 0 ) + ( keyboard().isKeyDown( Keys::RIGHT ) ? -1 : 0 );
}
