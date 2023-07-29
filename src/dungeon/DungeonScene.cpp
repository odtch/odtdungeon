#include "stdodt.h"

#include "DungeonScene.h"
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
#include "resource/Skeleton.h"
#include "skin/SkinImporter.h"
#include "skin/Skin.h"

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

			Vec3 gr( 50, 5, 0.5f );
			PhysicsShape* gs = PhysicsShape::CreateBox( gr );
			MeshPNT* gm = renderer().createDynamicMeshPNT( "dungeonboxmesh2" );
			MeshBuilder::CreateBox( *gm, PosOri(), gr, VertexPNT() );
			SceneObject* go = new SceneObject();
			go->setPosOri( PosOri().translated( Vec3::Down * gr.z() ));
			new SceneRenderInstancePNTProperty( gm, material, go );
			new PhysicsBody( gs, PhysicsMotionType_Static, go );
			_area1->addChild( go );

			float br = 1.5f;
			PhysicsShape* bs = PhysicsShape::CreateSphere( br );
			MeshPNT* bm = renderer().createDynamicMeshPNT( "bm" );
			MeshBuilder::CreateSphere( *bm, Vec3::Null, br, 3, VertexPNT() );
			for( int b = 0; b < 50; b++ ){
				SceneObject* bo1 = new SceneObject();
				bo1->setPosOri( PosOri().translated( Vec3( -5, 0, 5 + b * 4 ) ) );
				new SceneRenderInstancePNTProperty( bm, material, bo1 );
				new PhysicsBody( bs, PhysicsMotionType_Dynamic, bo1 );
				_area1->addChild( bo1 );
			}





			//mesh = renderer().createDynamicMeshPNT( "dungeonboxmesh3" );
			//MeshBuilder::CreateSphere( *mesh, Vec3::Null, 1.4f, 3, VertexPNT() );
			//renderer().createInstance( layer, PosOri().translated( Vec3( -3, 0, 0 ) ), mesh, material );
		}
		{
//			MeshPNT* mesh = renderer().loadMeshPNT( "platform5" );
//			renderer().createInstance( layer, PosOri().translated( Vec3( -8, 0, 0 ) ), mesh, material );
//			renderer().createInstance( layer, PosOri().translated( Vec3( -9.9, 0.1f, -0.1f ) ), mesh, material );
//			renderer().createInstance( layer, PosOri().translated( Vec3( -11.8, 0, -0.0f ) ), mesh, material );
		}
		{
			MeshPNT* mesh = renderer().loadMeshPNT( "banner02" );
			Material* material = renderer().createMaterial( "m2" );
			material->setTexture( renderer().loadTexture( "dt01" ) );
			renderer().createInstance( layer, PosOri().translated( Vec3( -8, -5, 0 ) ), mesh, material );
		}
		{
			Material* material = renderer().createMaterial( "m" );
			material->setTexture( renderer().loadTexture( "mcg_diff" ) );
			AssImp assimp;
			assimp.open( "/home/rt/media/mocap/MotusMan_v55/MotusMan_v55.fbx", AssImp::YUp_to_ZUp_Synty2() );

			//assimp.trace();
			Skeleton* skeleton = assimp.loadSkeleton();
			SkinType* skintype = SkinImporter::Import( assimp, 0 );

	//		charimporter.setupRagdollFromSkeleton( *skeleton );
	//		charimporter.loadSkin( *skeleton, assimp, 0 );
			//skeleton->trace();
			SceneObject* so1 = new SceneObject();
			so1->setPosOri( PosOri().translated( Vec3( 3, 0, 0 ) ) );
			new Skin( skintype, material, so1 );
			_area1->addChild( so1 );

			MeshPNT* sm = renderer().createDynamicMeshPNT( "sm" );
			skeleton->createMesh( *sm );
			odelete( skeleton );
			SceneObject* so = new SceneObject();
			so->setPosOri( PosOri().translated( Vec3( 0, 0, 0 ) ) );
			new SceneRenderInstancePNTProperty( sm, material, so );
			_area1->addChild( so );

		}
    }
    Scene::run();
}
