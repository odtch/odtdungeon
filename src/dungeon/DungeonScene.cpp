#include "stdodt.h"

#include "DungeonScene.h"
#include "math/Camera.h"
#include "scene/SceneArea.h"
#include "scene/SceneObject.h"
#include "scene/SceneRenderInstanceProperty.h"
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

	_sphere1vel += Vec3::Down * 0.05f * dt;
	_sphere1->setPosOri( _sphere1->posori().translated( _sphere1vel ) );
	if( _sphere1->posori().position().z() < 0 ){
		if( _sphere1vel.z() < 0 ){
			_sphere1vel.setZ( - _sphere1vel.z() );
		}
	}
	Scene::animate( dt );
}
void DungeonScene::run(){
    {
		//_sphere1vel = Vec3::Up * 5;

		Material* material = renderer().createMaterial( "dungeonboxmaterial" );
		material->setColor( Vec4( 0.8f, 0.8f, 0.8f, 1.0f ) );
		Texture* texture_red = renderer().loadTexture( "test_red" );

		_camera->setPosOri( PosOri().translated( Vec3( 0, -40, 0 ) ) );
		_uilayer = renderer().createRootLayer();
		_uilayer->setNextFixedCamera( *_camera );

		RenderLayer* layer = renderer().createNextLayer( _uilayer );
		_area1 = new SceneArea( "area1", layer );
		addChild( _area1 );

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
			MeshBuilder::CreateSphere( *mesh, Vec3::Null, 1.4f, 3, VertexPNT() );
			//renderer().createInstance( layer, PosOri().translated( Vec3( -3, 0, 0 ) ), mesh, material );
			_sphere1 = new SceneObject();
			_sphere1->setPosOri( PosOri().translated( Vec3( -8, 0, 5 ) ));
			new SceneRenderInstancePNTProperty( mesh, material, _sphere1 );
			_area1->addChild( _sphere1 );
			_sphere2 = new SceneObject();
			new SceneRenderInstancePNTProperty( mesh, material, _sphere2 );
			_sphere2->setPosOri( PosOri().translated( Vec3( -11, 0, 5 ) ));
			_area1->addChild( _sphere2 );
		}
		{
//			MeshPNT* mesh = renderer().loadMeshPNT( "platform5" );
//			renderer().createInstance( layer, PosOri().translated( Vec3( -8, 0, 0 ) ), mesh, material );
//			renderer().createInstance( layer, PosOri().translated( Vec3( -9.9, 0.1f, -0.1f ) ), mesh, material );
//			renderer().createInstance( layer, PosOri().translated( Vec3( -11.8, 0, -0.0f ) ), mesh, material );
		}
    }
    Scene::run();
}
