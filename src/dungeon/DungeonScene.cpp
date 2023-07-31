#include "stdodt.h"

#include "DungeonScene.h"
#include "DungeonCollection.h"
#include "math/Camera.h"
#include "scene/SceneArea.h"
#include "scene/SceneObject.h"
#include "scene/SceneRenderInstanceProperty.h"
#include "renderer/RenderLight.h"
#include "renderer/RenderLayer.h"
#include "resource/MeshBuilder.h"

#include "physics/PhysicsShapes.h"
#include "physics/PhysicsBody.h"
#include "physics/PhysicsDynamicBody.h"

#include "converter/AssImp.h"
#include "converter/AssImpAnimation.h"
#include "resource/Skeleton.h"
#include "skin/SkinImporter.h"
#include "skin/Skin.h"

#include "character/CharRagdoll.h"
#include "character/CharAnimation.h"
#include "character/CharMocapCollection.h"

CharRagdoll* charragdoll1 = null;
CharRagdoll* charragdoll2 = null;
CharAnimation* charanim1 = null;

DungeonScene::DungeonScene( Renderer* renderer )
    :Scene( renderer )
{
	_camera = new Camera();
}
DungeonScene::~DungeonScene(){
	_area1->deleteAllChilds();
	deleteAllChilds();
	_resources2delete.deleteAll();
	odelete( _camera );
}
void DungeonScene::animate( float dt ){
	_camera->translate(
		_camera->right() * ( control.cameraMoveX * 30 * dt )
		+ _camera->direction() * ( control.cameraMoveY * 30 * dt )
		+ _camera->up() * ( control.cameraMoveZ * 30 * dt )
	);
	_camera->rotate( control.cameraRotateX * 100 * dt, _camera->right() );
	_camera->rotate( control.cameraRotateY * 100 * dt, _camera->direction() );
	_camera->rotate( control.cameraRotateZ * 100 * dt, _camera->up() );
	_uilayer->setNextFixedCamera( *_camera );
	if( charanim1 ){
		static float at = 0;
		at += dt;
		if( at > charanim1->duration() )at -= charanim1->duration();
		CharPose p;
		charanim1->getPose( at, p );
		if( charragdoll1 ){
			charragdoll1->loadPose( p );
		}
		if( charragdoll2 ){
			charragdoll2->loadPose( p );
		}
	}
	Scene::animate( dt );
}
void DungeonScene::run(){
    {
		_camera->setPosOri( PosOri().translated( Vec3( 0, 10, 2 ) ).rotated( 180, Vec3::Up ) );
		_uilayer = renderer().createRootLayer();
		_uilayer->setNextFixedCamera( *_camera );
		RenderLayer* layer = renderer().createNextLayer( _uilayer );
		_area1 = new SceneArea( "area1", layer );
		addChild( _area1 );
		{
			Material* material = new Material();
			_resources2delete.add( material );
			material->setColor( Vec4( 0.8f, 0.8f, 0.8f, 1.0f ) );
			renderer().addLight( _uilayer, RenderLight::CreateAmbient( vec4( 0.8f, 0.5f, 0.5f, 1.0f ) ) );
			MeshPNT* mesh = new MeshPNT();
			_resources2delete.add( mesh );
			MeshBuilder::CreateBox( *mesh, PosOri(), Vec3( 56, 59, 1 ), VertexPNT() );
			renderer().createInstance( _uilayer, PosOri().translated( Vec3( 800, 6, 0 ) ).rotated( 30, Vec3::Right ).rotated( 15, Vec3::Up ).rotated( 15, Vec3::Forward ), mesh, material );
		}{
			renderer().addLight( layer, RenderLight::CreateAmbient( vec4( 0.2f, 0.2f, 0.2f, 1.0f ) ) );
			renderer().addLight( layer, RenderLight::CreateDirectional( Vec3( -0.41f, 0.1f, -2.0f ).normalized(), vec4( 0.8f, 0.8f, 0.8f, 1.0f ) ) );
		}
	}
	{
		Material* material = new Material();
		material->setColor( Vec4( 0.8f, 0.8f, 0.8f, 1.0f ) );
		_resources2delete.add( material );
		Vec3 gr( 10, 5, 0.1f );
		createStaticBox( Vec3( 0, gr.y(), 0 ), Vec3( gr.x(), 0.2f, 0.2f ), material );
		createStaticBox( Vec3( 0,-gr.y(), 0 ), Vec3( gr.x(), 0.2f, 1.4f ), material );
		createStaticBox( Vec3( gr.x(), 0, 0 ), Vec3( 0.2f, gr.y(), 1.4f ), material );
		createStaticBox( Vec3(-gr.x(), 0, 0 ), Vec3( 0.2f, gr.y(), 1.4f ), material );
		createStaticBox( Vec3( 0, 0, -1.0f ), Vec3( gr.x(), gr.y(), 1.0f ), material );
//		float br = 0.2f;
//		PhysicsShape* bs = PhysicsShape::CreateSphere( br );
//		_resources2delete.add( bs );
//		MeshPNT* bm = new MeshPNT();
//		_resources2delete.add( bm );
//		MeshBuilder::CreateSphere( *bm, Vec3::Null, br, 3, VertexPNT() );
//		for( int b = 0; b < 50; b++ ){
//			SceneObject* bo1 = new SceneObject();
//			bo1->setPosOri( PosOri().translated( Vec3( -3.65f, 0, 5 + b * br * 1.01f  ) ) );
//			new SceneRenderInstancePNTProperty( bm, material, bo1 );
//			new PhysicsBody( bs, PhysicsMotionType_Dynamic, bo1 );
//			_area1->addChild( bo1 );
//		}
	}
	{
		PhysicsShape* platform05shape = PhysicsShape::CreateMesh( *DungeonCollection::Get()->getMeshPNT( "platform05" ) );
		_resources2delete.add( platform05shape );
		SceneObject* platform = new SceneObject();
		platform->setPosOri( PosOri().translated( Vec3( -4, 0, 5.7f ) ));
		new SceneRenderInstancePNTProperty( DungeonCollection::Get()->getMeshPNT( "platform05" ), DungeonCollection::Get()->getMaterial( "dt01" ), platform );
		new PhysicsBody( platform05shape, PhysicsMotionType_Static, platform );
		_area1->addChild( platform );
	}{
		SceneObject* banner = new SceneObject();
		banner->setPosOri( PosOri().translated( Vec3( -4, 0, 5.7f + 0.3315f ) ));
		new SceneRenderInstancePNTProperty( DungeonCollection::Get()->getMeshPNT( "banner02" ), DungeonCollection::Get()->getMaterial( "dt01" ), banner );
		_area1->addChild( banner );
	}{
//		Material* material = CharMocapCollection::Get()->getMaterial( "mcg" );
//		CharRagdollType* motusman_type = CharMocapCollection::Get()->get<CharRagdollType>( "mm" );
//		SceneObject* r = new SceneObject();
//		CharRagdoll* motusman_ragdoll = new CharRagdoll( motusman_type, r );
//		r->setPosOri( PosOri().translated( Vec3( 4, 2, 0 ) ) );
//		new CharRagdollSkin( motusman_ragdoll, material, r );
//		_area1->addChild( r );
//
//		charragdoll1 = motusman_ragdoll;
	}{
		Material* material = DungeonCollection::Get()->getMaterial( "fk01" );
		CharRagdollType* ragdolltype = DungeonCollection::Get()->get<CharRagdollType>( "rt" );
		SceneObject* r = new SceneObject();
		CharRagdoll* ragdoll = new CharRagdoll( ragdolltype, r );
		r->setPosOri( PosOri().translated( Vec3( 0, 2, 0 ) ) );
		new CharRagdollSkin( ragdoll, material, r );
		_area1->addChild( r );
		charragdoll2 = ragdoll;
	}
	charanim1 = CharMocapCollection::Get()->get<CharAnimation>( "MOB1_Stand_Relaxed_To_Walk_F" );
	Scene::run();
}
void DungeonScene::createStaticBox( const Vec3& c, const Vec3& r, Material* material ){
	PhysicsShape* gs = PhysicsShape::CreateBox( r );
	_resources2delete.add( gs );
	MeshPNT* gm = new MeshPNT();
	_resources2delete.add( gm );
	MeshBuilder::CreateBox( *gm, PosOri(), r, VertexPNT() );
	SceneObject* go = new SceneObject();
	go->setPosOri( PosOri().translated( c ));
	new SceneRenderInstancePNTProperty( gm, material, go );
	new PhysicsBody( gs, PhysicsMotionType_Static, go );
	_area1->addChild( go );
}
