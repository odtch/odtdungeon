#include "stdodt.h"

#include "DungeonScene.h"
#include "math/Camera.h"
#include "renderer/RenderLight.h"
#include "renderer/RenderLayer.h"
#include "resource/MeshBuilder.h"

DungeonScene::DungeonScene( Renderer* renderer )
    :Scene( renderer )
{
	_camera = new Camera();
}
DungeonScene::~DungeonScene(){
}
void DungeonScene::animate( float dt ){
	_box1->setPosOri( _box1->posori().rotated( 25.0f * dt, Vec3::Up ) );
	Orientation co = _camera->posori().orientation();
	//co.rotate( -30.0f * dt, co.right() );
	co.rotate( -30.0f * dt, co.up() );
	/*
	Vec3 camera_pos = Mat4::RotationAtAxis( 71.0f * dt, Vec3::Up ).map( _camera->position() );
	Vec3 camera_dir = ( Vec3::Null - camera_pos ).normalized();
	_camera->setPosOri( PosOri( camera_pos, camera_dir, Vec3::Up ) );
	_camera->posori().ro
	*/
	_camera->translate(
		_camera->right() * ( control.cameraMoveX * 30 * dt )
		+ _camera->direction() * ( control.cameraMoveY * 30 * dt )
		+ _camera->up() * ( control.cameraMoveZ * 30 * dt )
	);
	_camera->rotate( control.cameraRotateX * 100 * dt, _camera->right() );
	_camera->rotate( control.cameraRotateY * 100 * dt, _camera->direction() );
	_camera->rotate( control.cameraRotateZ * 100 * dt, _camera->up() );
//	_camera->setPosOri( PosOri( co.direction() * -7.0f, co ) );
//	_camera->recalcMatrices( vec2( 600, 400 ));
//	_camera->recalcTest();
	_uilayer->setNextFixedCamera( *_camera );
	Scene::animate( dt );
}
void DungeonScene::run(){
    {
		Material* material = renderer().createMaterial( "dungeonboxmaterial" );
		material->setColor( Vec4( 0.8f, 0.8f, 0.8f, 1.0f ) );
		Texture* texture_red = renderer().loadTexture( "test_red" );

		_camera->setPosOri( PosOri().translated( Vec3( 0, -40, 0 ) ) );
		_uilayer = renderer().createRootLayer();
		_uilayer->setNextFixedCamera( *_camera );

		RenderLayer* layer = renderer().createNextLayer( _uilayer );

		{
			renderer().addLight( _uilayer, RenderLight::CreateAmbient( vec4( 0.8f, 0.5f, 0.5f, 1.0f ) ) );
			//renderer().addLight( uilayer, RenderLight::CreateDirectional( Vec3( 0.1f, 0.1f, -1.0f ).normalized(), vec4( 0.5f, 0.8f, 0.5f, 1.0f ) ) );
			MeshPNT* mesh = renderer().createDynamicMeshPNT( "dungeonboxmesh" );
			MeshBuilder::CreateBox( *mesh, PosOri(), Vec3( 56, 59, 1 ), VertexPNT() );
			renderer().createInstance( _uilayer, PosOri().translated( Vec3( 800, 6, 0 ) ).rotated( 30, Vec3::Right ).rotated( 15, Vec3::Up ).rotated( 15, Vec3::Forward ), mesh, material );
		}
		{
			renderer().addLight( layer, RenderLight::CreateAmbient( vec4( 0.2f, 0.2f, 0.2f, 1.0f ) ) );
			renderer().addLight( layer, RenderLight::CreateDirectional( Vec3( -0.1f, 0.1f, -2.0f ).normalized(), vec4( 0.8f, 0.8f, 0.8f, 1.0f ) ) );
			MeshPNT* mesh = renderer().createDynamicMeshPNT( "dungeonboxmesh2" );
			MeshBuilder::CreateBox( *mesh, PosOri(), Vec3( 1, 1, 1 ), VertexPNT() );
			renderer().createInstance( layer, PosOri().translated( Vec3( 0, 0, 0 ) ).rotated( 30, Vec3::Right ).rotated( 15, Vec3::Up ).rotated( 15, Vec3::Forward ), mesh, material );
			_box1 = renderer().createInstance( layer, PosOri().translated( Vec3( 3, 0, 0 ) ), mesh, material );


			mesh = renderer().createDynamicMeshPNT( "dungeonboxmesh3" );
			MeshBuilder::CreateSphere( *mesh, Vec3::Null, 0.4f, 2, VertexPNT() );
			renderer().createInstance( layer, PosOri().translated( Vec3( -3, 0, 0 ) ), mesh, material );
		}
		{
			MeshPNT* mesh = renderer().loadMeshPNT( "platform5" );
			renderer().createInstance( layer, PosOri().translated( Vec3( -8, 0, 0 ) ), mesh, material );
			renderer().createInstance( layer, PosOri().translated( Vec3( -9.9, 0.1f, -0.1f ) ), mesh, material );
			renderer().createInstance( layer, PosOri().translated( Vec3( -11.8, 0, -0.0f ) ), mesh, material );
		}
    }
    Scene::run();
}
