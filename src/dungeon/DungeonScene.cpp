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
#include "character/CharImporter.h"


Skin* _skin = null;
AssImpAnimation* anim3  = null;
MeshPNT* skeleton_mesh = null;
CharRagdoll* charragdoll1 = null;
CharRagdoll* charragdoll2 = null;
CharAnimation* charanim1 = null;

DungeonScene::DungeonScene( Renderer* renderer )
    :Scene( renderer )
{
	_camera = new Camera();
}
DungeonScene::~DungeonScene(){
	deleteAllChilds();
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
	if( anim3 ){
		static float at = 0;
		at += dt;
		AssImpAnimationFrame* f = anim3->findFrameAt( at );
		if( f == null ){
			at = 0;
		} else {
			_skin->loadBones( *f->skeleton() );
			if( skeleton_mesh ){
				skeleton_mesh->clear();
				f->skeleton()->createMesh( *skeleton_mesh );
			}
		}
	}
	Scene::animate( dt );
}
void DungeonScene::run(){
    {
		Material* material = new Material();
		material->setColor( Vec4( 0.8f, 0.8f, 0.8f, 1.0f ) );
		//Texture* texture_red = renderer().loadTexture( "test_red" );
		_camera->setPosOri( PosOri().translated( Vec3( 0, 10, 2 ) ).rotated( 180, Vec3::Up ) );
		_uilayer = renderer().createRootLayer();
		_uilayer->setNextFixedCamera( *_camera );
		RenderLayer* layer = renderer().createNextLayer( _uilayer );
		_area1 = new SceneArea( "area1", layer );
		addChild( _area1 );
		{
			renderer().addLight( _uilayer, RenderLight::CreateAmbient( vec4( 0.8f, 0.5f, 0.5f, 1.0f ) ) );
			//renderer().addLight( uilayer, RenderLight::CreateDirectional( Vec3( 0.1f, 0.1f, -1.0f ).normalized(), vec4( 0.5f, 0.8f, 0.5f, 1.0f ) ) );
			MeshPNT* mesh = new MeshPNT();
			MeshBuilder::CreateBox( *mesh, PosOri(), Vec3( 56, 59, 1 ), VertexPNT() );
			renderer().createInstance( _uilayer, PosOri().translated( Vec3( 800, 6, 0 ) ).rotated( 30, Vec3::Right ).rotated( 15, Vec3::Up ).rotated( 15, Vec3::Forward ), mesh, material );
		}
		{
			renderer().addLight( layer, RenderLight::CreateAmbient( vec4( 0.2f, 0.2f, 0.2f, 1.0f ) ) );
			renderer().addLight( layer, RenderLight::CreateDirectional( Vec3( -0.1f, 0.1f, -2.0f ).normalized(), vec4( 0.8f, 0.8f, 0.8f, 1.0f ) ) );

			Vec3 gr( 50, 5, 0.5f );
			PhysicsShape* gs = PhysicsShape::CreateBox( gr );
			MeshPNT* gm = new MeshPNT();
			MeshBuilder::CreateBox( *gm, PosOri(), gr, VertexPNT() );
			SceneObject* go = new SceneObject();
			go->setPosOri( PosOri().translated( Vec3::Down * gr.z() ));
			new SceneRenderInstancePNTProperty( gm, material, go );
			new PhysicsBody( gs, PhysicsMotionType_Static, go );
			_area1->addChild( go );

			float br = 1.5f;
			PhysicsShape* bs = PhysicsShape::CreateSphere( br );
			MeshPNT* bm = new MeshPNT();
			MeshBuilder::CreateSphere( *bm, Vec3::Null, br, 3, VertexPNT() );
			for( int b = 0; b < 50; b++ ){
				SceneObject* bo1 = new SceneObject();
				bo1->setPosOri( PosOri().translated( Vec3( -5, 0, 5 + b * 4 ) ) );
				new SceneRenderInstancePNTProperty( bm, material, bo1 );
				new PhysicsBody( bs, PhysicsMotionType_Dynamic, bo1 );
				_area1->addChild( bo1 );
			}
		}
		{
//			MeshPNT* mesh = renderer().loadMeshPNT( "platform5" );
//			renderer().createInstance( layer, PosOri().translated( Vec3( -8, 0, 0 ) ), mesh, material );
//			renderer().createInstance( layer, PosOri().translated( Vec3( -9.9, 0.1f, -0.1f ) ), mesh, material );
//			renderer().createInstance( layer, PosOri().translated( Vec3( -11.8, 0, -0.0f ) ), mesh, material );
		}
		{
			renderer().createInstance( layer, PosOri().translated( Vec3( -8, -5, 0 ) ), DungeonCollection::Get()->getMeshPNT( "banner02" ), DungeonCollection::Get()->getMaterial( "dt01" ));
		}
		{
//			Material* material = renderer().createMaterial( "m" );
//			material->setTexture( renderer().loadTexture( "mcg_diff" ) );
//			AssImp assimp;
//			assimp.open( "/home/rt/media/mocap/MotusMan_v55/MotusMan_v55.fbx", AssImp::YUp_to_ZUp_Synty2() );
//			Skeleton* skeleton = assimp.loadSkeleton();
//			SkinType* skintype = SkinImporter::Import( assimp, 0 );
//			AssImp a2;
//			a2.open( "/home/rt/media/mocap/FBX_Ninja_v27_Pro/Animation/Root_Motion/NJA_Rlx_Walk_Forward_Loop.fbx", AssImp::YUp_to_ZUp_Synty2() );
//			assert( 1 == a2.animationCount() );
//			anim3 = a2.loadAnimation();
//	//		charimporter.setupRagdollFromSkeleton( *skeleton );
//	//		charimporter.loadSkin( *skeleton, assimp, 0 );
//			SceneObject* so1 = new SceneObject();
//			so1->setPosOri( PosOri().translated( Vec3( 3, 0, 0 ) ) );
//			_skin = new Skin( skintype, material, so1 );
//			_area1->addChild( so1 );
		}
		{
			{
				Material* material = CharMocapCollection::Get()->getMaterial( "mcg" );
				CharRagdollType* motusman_type = CharMocapCollection::Get()->get<CharRagdollType>( "mm" );
				SceneObject* r = new SceneObject();
				CharRagdoll* motusman_ragdoll = new CharRagdoll( motusman_type, r );
				r->setPosOri( PosOri().translated( Vec3( 4, 2, 0 ) ) );
				new CharRagdollSkin( motusman_ragdoll, material, r );
				_area1->addChild( r );
				charanim1 = CharMocapCollection::Get()->get<CharAnimation>( "NJA_Rlx_Walk_Forward_Loop" );
				charragdoll1 = motusman_ragdoll;
			}
		}
		{
			Material* material = DungeonCollection::Get()->getMaterial( "fk01" );
			{
				CharRagdollType* ragdolltype = DungeonCollection::Get()->get<CharRagdollType>( "rt" );
				SceneObject* r = new SceneObject();
				CharRagdoll* ragdoll = new CharRagdoll( ragdolltype, r );
				r->setPosOri( PosOri().translated( Vec3( 0, 2, 0 ) ) );
				new CharRagdollSkin( ragdoll, material, r );
				_area1->addChild( r );
				charragdoll2 = ragdoll;
			}
		}
	}
    Scene::run();
}
