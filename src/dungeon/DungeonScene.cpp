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
#include "character/CharCharacter.h"
#include "character/CharBrain.h"
#include "character/CharJoint.h"
#include "character/CharMocapCollection.h"

#include "brain/Brain.h"

#include "effects/Particles.h"
#include "dungeon/Spell01.h"

List<CharAnimation*> animations;

CharCharacter* character = null;
Spell01* spell01 = null;

DungeonScene::DungeonScene( Renderer* renderer )
    :Scene( renderer )
{
	_camera = new Camera();
}
DungeonScene::~DungeonScene(){
	animations.removeAll();
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

	{
		static float at = 0;
		at += dt;
		static int ai = 0;
		if( at <= animations.get( ai )->duration() ){
			CharPose p;
			animations.get( ai )->getPose( at, p );
			character->_ragdoll->loadPose( p );
		} else {
			at = 0;
			ai = ( ai + 1 ) % animations.size();
		}
		if( ai == 1 && at > animations.get( ai )->duration() * 0.175f ){
			spell01->fire();
		}
	}
	if( spell01 ){
		static CharJoint* hand = null;
		if( hand == null )hand = character->_ragdoll->getJointByName( "RightHand" );
		spell01->setAttachementPosOri( PosOri( character->posori().matrix() * hand->absolutematrix() ) );
	}
	Scene::animate( dt );
}
void DungeonScene::run(){
	RenderLayer* translucent_layer = null;
	{
		_camera->setPosOri( PosOri().translated( Vec3( 14, 4, 2 ) ).rotated( -90, Vec3::Up ) );
		_camera->setPosOri( PosOri().translated( Vec3( 14, 15, 2 ) ).rotated( -120, Vec3::Up ) );
		_uilayer = renderer().createRootLayer();
		_uilayer->setNextFixedCamera( *_camera );
		RenderLayer* layer = renderer().createNextLayer( _uilayer );
		translucent_layer = renderer().createTranslucentLayer( layer );
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
			renderer().createInstance( _uilayer, PosOri().translated( Vec3( 800, -406, 0 ) ).rotated( 30, Vec3::Right ).rotated( 15, Vec3::Up ).rotated( 15, Vec3::Forward ).matrix(), mesh, material );
		}{
			renderer().addLight( layer, RenderLight::CreateAmbient( vec4( 0.1f, 0.1f, 0.1f, 1.0f ) ) );
			renderer().addLight( layer, RenderLight::CreateDirectional( Vec3( -0.41f, 0.1f, -2.0f ).normalized(), vec4( 0.1f, 0.1f, 0.1f, 1.0f ) ) );
		}
	}
	{
		Material* material = new Material();
		material->setColor( Vec4( 0.8f, 0.8f, 0.8f, 1.0f ) );
		_resources2delete.add( material );
		Vec3 gr( 10, 50, 0.1f );
//		createStaticBox( Vec3( 0, gr.y(), 0 ), Vec3( gr.x(), 0.2f, 1.4f ), material );
//		//createStaticBox( Vec3( 0,-gr.y(), 0 ), Vec3( gr.x(), 0.2f, 0.2f ), material );
//		createStaticBox( Vec3( gr.x(), 0, 0 ), Vec3( 0.2f, gr.y(), 1.4f ), material );
//		createStaticBox( Vec3(-gr.x(), 0, 0 ), Vec3( 0.2f, gr.y(), 1.4f ), material );
//		createStaticBox( Vec3( 0, 0, -1.0f ), Vec3( gr.x(), gr.y(), 1.0f ), material );
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
		Random rnd;
		for( int x = -10; x < 10; x += 5 ){
			SceneObject* object = new SceneObject();
			object->setPosOri( PosOri().translated( Vec3( -x, -4, 0 ) ));
			new SceneRenderInstancePNTProperty( DungeonCollection::Get()->getMeshPNT( "wall01" ), DungeonCollection::Get()->getMaterial( "dt01" ), object );
			_area1->addChild( object );
		}
		for( int y = -10; y < 20; y += 5 ){
			SceneObject* object = new SceneObject();
			object->setPosOri( PosOri( Vec3( 0, y, 0 ), Vec3::Right, Vec3::Up ));
			new SceneRenderInstancePNTProperty( DungeonCollection::Get()->getMeshPNT( String( "wall0" ) + String::FromInt( 1 + ( 100 + y ) % 3 ) ), DungeonCollection::Get()->getMaterial( "dt01" ), object );
			_area1->addChild( object );
		}
		for( int x = -10; x < 10; x+= 5 ){
			for( int y = -10; y < 20; y += 5 ){
				SceneObject* object = new SceneObject();
				object->setPosOri( PosOri( Vec3( x, y, -0.0f ), Vec3::Right, Vec3::Up ));
				new SceneRenderInstancePNTProperty( DungeonCollection::Get()->getMeshPNT( String( "tile0" ) + String::FromInt( 1 + ( 100 + y + y / 5 ) % 3 ) ), DungeonCollection::Get()->getMaterial( "dt01" ), object );
				_area1->addChild( object );
			}
		}
		for( int y = -4; y < 20; y += 4 ){
			SceneObject* object = new SceneObject();
			object->setPosOri( PosOri( Vec3( 1, 2.2f + y * 0.8f, 0 ), Vec3::Right, Vec3::Up ));
			new SceneRenderInstancePNTProperty( DungeonCollection::Get()->getMeshPNT( String( "banner0" ) + String::FromInt( 1 + ( 100 + y ) % 3 ) ), DungeonCollection::Get()->getMaterial( "dt01" ), object );
			_area1->addChild( object );
		}
	}
//	{
//		PhysicsShape* platform05shape = PhysicsShape::CreateMesh( *DungeonCollection::Get()->getMeshPNT( "platform05" ) );
//		_resources2delete.add( platform05shape );
//		SceneObject* platform = new SceneObject();
//		platform->setPosOri( PosOri().translated( Vec3( -4, 14, 5.7f ) ));
//		new SceneRenderInstancePNTProperty( DungeonCollection::Get()->getMeshPNT( "platform05" ), DungeonCollection::Get()->getMaterial( "dt01" ), platform );
//		new PhysicsBody( platform05shape, PhysicsMotionType_Static, platform );
//		_area1->addChild( platform );
//	}{
//		SceneObject* banner = new SceneObject();
//		banner->setPosOri( PosOri().translated( Vec3( -4, 14, 5.7f + 0.3315f ) ));
//		new SceneRenderInstancePNTProperty( DungeonCollection::Get()->getMeshPNT( "banner02" ), DungeonCollection::Get()->getMaterial( "dt01" ), banner );
//		_area1->addChild( banner );
//	}
	{
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
//		Material* material = DungeonCollection::Get()->getMaterial( "fk01" );
//		CharRagdollType* ragdolltype = DungeonCollection::Get()->get<CharRagdollType>( "rt" );
//		char2 = new SceneObject();
//		CharRagdoll* ragdoll = new CharRagdoll( ragdolltype, char2 );
//		char2->setPosOri( PosOri().translated( Vec3( 0, 2, 0 ) ) );
//		new CharRagdollSkin( ragdoll, material, char2 );
//		//new SceneRenderInstancePNTProperty( DungeonCollection::Get()->getMeshPNT( "banner02" ), DungeonCollection::Get()->getMaterial( "dt01" ), char2 );
//		_area1->addChild( char2 );
//		charragdoll2 = ragdoll;
	}
	character = new CharCharacter( DungeonCollection::Get()->get<CharRagdollType>( "rt6" ), DungeonCollection::Get()->getMaterial( "fk01" ) );
	character->setPosOri( PosOri( Vec3( -9, -4, 0 ), Vec3::Right, Vec3::Up ) );
	character->setPosOri( PosOri().translated( Vec3( 2, 0, 0.15f ) ) );
//	new SceneRenderInstancePNTProperty( DungeonCollection::Get()->getMeshPNT( "banner02" ), DungeonCollection::Get()->getMaterial( "dt01" ), character );
	_area1->addChild( character );
	_area1->addChild( new ParticleSystem( 1000, translucent_layer ) );
	spell01 = new Spell01();
//	new SceneRenderInstancePNTProperty( DungeonCollection::Get()->getMeshPNT( "banner02" ), DungeonCollection::Get()->getMaterial( "dt01" ), spell01 );
	_area1->addChild( spell01 );
	{
//		animations.add( CharMocapCollection::Get()->get<CharAnimation>( "NJA_Cbt_Knock_Dn_Bkwd_Mid_Recover" ) );
//		animations.add( CharMocapCollection::Get()->get<CharAnimation>( "NJA_Cbt_Jump_Up_Flip" ) );
//		animations.add( CharMocapCollection::Get()->get<CharAnimation>( "NJA_Cbt_Jump_Wave" ) );
		animations.add( CharMocapCollection::Get()->get<CharAnimation>( "NJA_Cbt_Stand_V2_Idle" ) );
//		animations.add( CharMocapCollection::Get()->get<CharAnimation>( "NJA_Cbt_Std_Jump_Backward" ) );
//		animations.add( CharMocapCollection::Get()->get<CharAnimation>( "NJA_Cbt_Std_Jump_Left" ) );
//		animations.add( CharMocapCollection::Get()->get<CharAnimation>( "NJA_Punch_High_v1" ) );
		animations.add( CharMocapCollection::Get()->get<CharAnimation>( "NJA_Punch_High_v2" ) );
//		animations.add( CharMocapCollection::Get()->get<CharAnimation>( "NJA_Punch_High_v3" ) );
//		animations.add( CharMocapCollection::Get()->get<CharAnimation>( "NJA_Punch_Mid_v1" ) );
//		animations.add( CharMocapCollection::Get()->get<CharAnimation>( "NJA_Punch_Mid_v2" ) );
//		animations.add( CharMocapCollection::Get()->get<CharAnimation>( "NJA_Punch_Mid_v3" ) );
	}
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
