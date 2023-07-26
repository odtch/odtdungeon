#include "stdodt.h"
//#include "CharApp.h"
//#include "CharRagdoll.h"
//#include "CharJoint.h"
//#include "CharSkin.h"
//#include "CharResource.h"
//#include "CharAnimation.h"
//#include "CharImporter.h"
//#include "CharMocapCollection.h"
//#include "CharTestCollection.h"
//#include "terrain/TerrainSimpleBiome.h"
//#include "terrain/TerrainMapBuilder.h"
//#include "converter/AssImp.h"
//#include "converter/AssImpAnimation.h"
//#include "rts/RtsCollection.h"

//#include "skin/SkinImporter.h"
//#include "skin/SkinRenderer.h"
//#include "skin/SkinMesher.h"

//class CharAnimatorTest : public ObjectProperty
//{
//private:
//	CharAnimation* _animation;
//	float _time = 0;
//	CharPose _pose;
//	CharRagdoll* _ragdoll;
//public:
//	explicit CharAnimatorTest( CharAnimation* animation, CharRagdoll* ragdoll, Object* object );
//	virtual ~CharAnimatorTest() override;
//public:
//	virtual void animate( float dt ) override;
//};
//CharAnimatorTest::CharAnimatorTest( CharAnimation* animation, CharRagdoll* ragdoll, Object* object )
//	:ObjectProperty( object )
//	,_animation( asserted( animation ) )
//	,_ragdoll( asserted( ragdoll ) )
//	,_time( 0 )
//{
//}
//CharAnimatorTest::~CharAnimatorTest(){
//}
//void CharAnimatorTest::animate( float dt ){
//	_time += dt * 1.0f;
//	if( _animation->duration() < _time ){
//		_time = 0;
//	}
//	CharAnimationFrame* frame = _animation->findFrameAt( _time );
//	if( frame ){
////		assert( false );
////		_skeleton->load( *frame->_skeleton );
//		_animation->getPose( _time, _pose );
//		_ragdoll->loadPose( _pose );
//	}
//	/*
//	*/
//}

//CharApp::CharApp()
//    :GameApp( "ODT-Space-Char" )
//{
//	new Physics();
//    new Terrain();
//    new TerrainSimpleBiome();
//	new CharModule();
//	new CharMocapCollection();
//	new CharTestCollection();
////  //    new RtsCollection();
//}
//CharApp::~CharApp(){
//}
//void CharApp::onStartup(){
//	App::onStartup();
//}
//void CharApp::start( Scene* scene, UiLayerHUD* hud, SceneArea* area, Controller*& controller ){
//	SkinMesherProgram* prgm = new SkinMesherProgram();
//	prgm->create( windowadapter()->device(), &renderer()->renderTask() );

//	_area = area;
//	Physics::Get()->initialize( window() );
//	area->layer()->createAmbientLight( vec4( 1, 1, 1, 1 ) * 0.3f );
//	area->layer()->createDirectionalLight( ::Vec3( 0.3f, -0.93f, -1.0f ).normalized(), vec4( 1, 1, 1, 0 ) * 0.7f );
//    controller = new FlyController();
//    ((FlyController*)controller)->_movespeed *= 10;
//	((FlyController*)controller)->translateLocal( Vec3( -0.15f, 10, 6 ) );
//	((FlyController*)controller)->rotation = Vec2( 20 + 180 -40 , 30 );
//    {
//        String terrain_filename( "/home/rt/odtspace.data/charterrainmap" );
//		//File::DeleteFileIfExists( terrain_filename );
//		if( File::Exists( terrain_filename ) ){
//			_terrain = TerrainMap::Load( terrain_filename );
//		} else {
//			logDebug( "build terrain" );
//			TerrainMapBuilder builder( 2, 2, 40, 5.0f ); float terrainheight = 40.0f;
//			builder.createBorder();
//            TerrainSimpleBiome::Get()->build( builder );
//			_terrain = builder.createMap();
//			_terrain->saveAs( terrain_filename );
//		}
//		area->addChild( _terrain );
//	}
//	float x = 0;
//	{
//		Object* object = new Object();
//		{
//			Material* mocap_material = CharMocapCollection::Get()->getMaterial( "Material" );
//			Material* ss_material = CharTestCollection::Get()->getMaterial( "ssm1a" );
//			CharRagdollType* ss_ragdolltype = asserted( dynamic_cast<CharRagdollType*>( CharTestCollection::Get()->getResource( "SK_Chr_Crew_Female_01" ) ) );
//			CharRagdollType* mocap_ragdolltype = asserted( dynamic_cast<CharRagdollType*>( CharMocapCollection::Get()->getResource( "MotusMan" ) ) );
//			{
//				CharRagdoll* ragdoll = new CharRagdoll( mocap_ragdolltype );
//				CharAnimation* animation = asserted( dynamic_cast<CharAnimation*>( CharMocapCollection::Get()->getResource( "NJA_Rlx_Stand_Idle" ) ) );
//				new CharAnimatorTest( animation, ragdoll, object );
//				{
//					CharRagdollRenderer* renderer = new CharRagdollRenderer( ragdoll, mocap_material, object );
//					renderer->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//					x += 1.0f;
//				}{
//					CharRagdollSkeletonRenderer* renderer = new CharRagdollSkeletonRenderer( ragdoll, object );
//					renderer->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//					x += 1.0f;
//				}{
//					CharRagdollSkinRenderer* renderer = new CharRagdollSkinRenderer( ragdoll, mocap_material, object );
//					renderer->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//				}
//				x += 1.5f;
//				{
//					CharRagdoll* ss_ragdoll = new CharRagdoll( ss_ragdolltype );
//					new CharAnimatorTest( animation, ss_ragdoll, object );
//					(new CharRagdollSkinRenderer( ss_ragdoll, ss_material, object ))->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//					x += 1.1f;
//					CharRagdoll* mocap_ragdoll = new CharRagdoll( mocap_ragdolltype );
//					new CharAnimatorTest( animation, mocap_ragdoll, object );
//					(new CharRagdollSkinRenderer( mocap_ragdoll, mocap_material, object ))->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//					x += 1.1f;

//				}
//			}
//			{
//				CharAnimation* animation = asserted( dynamic_cast<CharAnimation*>( CharMocapCollection::Get()->getResource( "MOB1_Walk_F_Loop" ) ) );
//				CharRagdoll* mocap_ragdoll = new CharRagdoll( mocap_ragdolltype );
//				new CharAnimatorTest( animation, mocap_ragdoll, object );
//				(new CharRagdollSkinRenderer( mocap_ragdoll, mocap_material, object ))->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//				x += 1.1f;
//				CharRagdoll* ss_ragdoll = new CharRagdoll( ss_ragdolltype );
//				new CharAnimatorTest( animation, ss_ragdoll, object );
//				(new CharRagdollSkinRenderer( ss_ragdoll, ss_material, object ))->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//				x += 1.1f;
//			}
//			for( int i = 0; i < 1; i++ )
//			{
//				CharAnimation* animation = asserted( dynamic_cast<CharAnimation*>( CharMocapCollection::Get()->getResource( "NJA_Cbt_Forward_Roll_v3" ) ) );
//				CharRagdoll* mocap_ragdoll = new CharRagdoll( mocap_ragdolltype );
//				new CharAnimatorTest( animation, mocap_ragdoll, object );
//				(new CharRagdollSkinRenderer( mocap_ragdoll, mocap_material, object ))->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//				x += 1.1f;
//				CharRagdoll* ss_ragdoll = new CharRagdoll( ss_ragdolltype );
//				new CharAnimatorTest( animation, ss_ragdoll, object );
//				(new CharRagdollSkinRenderer( ss_ragdoll, ss_material, object ))->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//				x += 1.1f;
//			}
//		}

//		area->addChild( object );
//	}
//#ifdef xlkjx
//	{
//		Material* material = CharMocapCollection::Get()->getMaterial( "Material" );
//		AssImp ai_T;
//		ai_T.open( "/home/rt/media/mocap/MotusMan_v55/MotusMan_v55.fbx" );
//		Skeleton* s_T = ai_T.loadSkeleton();
//		SkinType* sk = SkinImporter::Import( ai_T, 0 );
//		AssImp ai_W;
//		ai_W.open( "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Walk_F_Loop.fbx" );
//		Skeleton* s_W = ai_W.loadSkeleton();
//		( new SkinRenderer( s_W, sk, material, object ) )->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );x += 1.5f;

//		CharRagdollType* ragdolltype = asserted( dynamic_cast<CharRagdollType*>( CharMocapCollection::Get()->getResource( "MotusMan" ) ) );
//		{
//			CharAnimation* animation = asserted( dynamic_cast<CharAnimation*>( CharMocapCollection::Get()->getResource( "NJA_Rlx_Stand_Idle" ) ) );
//			CharRagdoll* ragdoll = new CharRagdoll( ragdolltype );
//			CharAnimatorTest* test = new CharAnimatorTest( animation, ragdoll, object );
//			{
//				CharRagdollRenderer* renderer = new CharRagdollRenderer( ragdoll, material, object );
//				renderer->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//			}{
//				CharRagdollSkeletonRenderer* renderer = new CharRagdollSkeletonRenderer( ragdoll, object );
//				renderer->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//			}
//			x += 1.5f;
//		}
//		{
//			CharAnimation* animation = asserted( dynamic_cast<CharAnimation*>( CharMocapCollection::Get()->getResource( "MOB1_Walk_F_Loop" ) ) );
//			CharRagdoll* ragdoll = new CharRagdoll( ragdolltype );
//			CharAnimatorTest* test = new CharAnimatorTest( animation, ragdoll, object );
//			CharRagdollRenderer* renderer = new CharRagdollRenderer( ragdoll, material, object );
//			renderer->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//			x += 1.5f;
//		}

//		area->addChild( object );
//	}
//	if( false )
//	{
//		Material* material = CharMocapCollection::Get()->getMaterial( "Material" );
//		Object* object = new Object();
//		AssImp cf_ai;
//		cf_ai.open( "/home/rt/media/Polygon_SciFi_Space/Characters/SK_Chr_Crew_Female_01.fbx" );
//		Skeleton* cf_s1 = cf_ai.loadSkeleton();
//		SkinType* cf_sk = SkinImporter::Import( cf_ai, 0 );
//		for( SkeletonJoint* j : cf_s1->joints() ){
//			PosOri p( j->absolutematrix() );
//			p.setPosition( p.position() * 2.1f );
//			j->setAbsoluteMatrix( p.matrix() );
//		}
//		( new SkinRenderer( cf_s1, cf_sk, material, object ) )->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );x += 1.5f;
//		area->addChild( object );
//	}
//	{
//		Material* material = CharMocapCollection::Get()->getMaterial( "Material" );
//		{

//			CharImporter charimporter( CharImporter::MecanimFormat );
//			charimporter.createRagdoll();
//			{
//				AssImp assimp;
//				assimp.open( "/home/rt/media/Polygon_SciFi_Space/Characters/SK_Chr_Crew_Female_01.fbx" );
//				Skeleton* skeleton = assimp.loadSkeleton();
//				charimporter.setupRagdollFromSkeleton( *skeleton );
//				charimporter.loadSkin( *skeleton, assimp, 0 );
//				odelete( skeleton );
//				{
//					Object* object = new Object();
//					ObjectRenderableProperty* renderer = new ObjectRenderableProperty( charimporter._testmesh, material, object );
//					renderer->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//					x += 1.5f;
//					area->addChild( object );
//				}
//			}
//		}
//		{
//			CharRagdollType* ragdolltype = asserted( dynamic_cast<CharRagdollType*>( CharMocapCollection::Get()->getResource( "MotusMan" ) ) );
//			{
//				CharRagdoll* ragdoll = new CharRagdoll( ragdolltype );
//				Object* object = new Object();
//				{
//					CharRagdollSkeletonRenderer* renderer = new CharRagdollSkeletonRenderer( ragdoll, object );
//					renderer->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//					//x += 1.5f;
//				}
//				{
//					CharRagdollRenderer* renderer = new CharRagdollRenderer( ragdoll, material, object );
//					renderer->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//					x += 1.5f;
//				}
//				area->addChild( object );
//			}

//			{
//				CharAnimation* animation = asserted( dynamic_cast<CharAnimation*>( CharMocapCollection::Get()->getResource( "NJA_Rlx_Stand_Idle" ) ) );
//				Object* object = new Object();
//				CharRagdoll* ragdoll = new CharRagdoll( ragdolltype );
//				CharAnimatorTest* test = new CharAnimatorTest( animation, ragdoll, object );
//				{
//					CharRagdollRenderer* renderer = new CharRagdollRenderer( ragdoll, material, object );
//					renderer->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//				}{
//					CharRagdollSkeletonRenderer* renderer = new CharRagdollSkeletonRenderer( ragdoll, object );
//					renderer->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//				}
//				x += 1.5f;
//				area->addChild( object );
//			}
//			{
//				CharAnimation* animation = asserted( dynamic_cast<CharAnimation*>( CharMocapCollection::Get()->getResource( "MOB1_Walk_F_Loop" ) ) );
//				Object* object = new Object();
//				CharRagdoll* ragdoll = new CharRagdoll( ragdolltype );
//				CharAnimatorTest* test = new CharAnimatorTest( animation, ragdoll, object );
//				CharRagdollRenderer* renderer = new CharRagdollRenderer( ragdoll, material, object );
//				renderer->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//				x += 1.5f;
//				area->addChild( object );
//			}
//			{
//				CharAnimation* animation = asserted( dynamic_cast<CharAnimation*>( CharMocapCollection::Get()->getResource( "NJA_Cbt_Forward_Roll_v3" ) ) );
//				Object* object = new Object();
//				CharRagdoll* ragdoll = new CharRagdoll( ragdolltype );
//				CharAnimatorTest* test = new CharAnimatorTest( animation, ragdoll, object );
//				CharRagdollRenderer* renderer = new CharRagdollRenderer( ragdoll, material, object );
//				renderer->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//				x += 1.5f;
//				area->addChild( object );
//			}
//		}
//	}
//	{
//		Material* material = CharTestCollection::Get()->getMaterial( "ssm1a" );
//		{
//			CharRagdollType* ragdolltype = asserted( dynamic_cast<CharRagdollType*>( CharTestCollection::Get()->getResource( "SK_Chr_Crew_Female_01" ) ) );
//			{
//				CharRagdoll* ragdoll = new CharRagdoll( ragdolltype );
//				Object* object = new Object();
//				{
//					CharRagdollSkeletonRenderer* renderer = new CharRagdollSkeletonRenderer( ragdoll, object );
//					renderer->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//					//x += 1.5f;
//				}
//				{
//					CharRagdollRenderer* renderer = new CharRagdollRenderer( ragdoll, material, object );
//					renderer->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//					x += 1.5f;
//				}
//				area->addChild( object );
//			}
//			{
//				CharAnimation* animation = asserted( dynamic_cast<CharAnimation*>( CharMocapCollection::Get()->getResource( "NJA_Rlx_Stand_Idle" ) ) );
//				Object* object = new Object();
//				CharRagdoll* ragdoll = new CharRagdoll( ragdolltype );
//				CharAnimatorTest* test = new CharAnimatorTest( animation, ragdoll, object );
//				{
//					CharRagdollRenderer* renderer = new CharRagdollRenderer( ragdoll, material, object );
//					renderer->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//				}{
//					CharRagdollSkeletonRenderer* renderer = new CharRagdollSkeletonRenderer( ragdoll, object );
//					renderer->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//				}
//				x += 1.5f;
//				area->addChild( object );
//			}
//		}
//	}

//	if( false ){
//		CharRagdollType* skeletontype = asserted( dynamic_cast<CharRagdollType*>( CharMocapCollection::Get()->getResource( "T-Skeleton" ) ) );
//		Object* object = new Object();
//		CharRagdoll* skeleton = new CharRagdoll( skeletontype );
//		CharRagdollRenderer* renderer = new CharRagdollRenderer( skeleton, object );
//		renderer->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//		area->addChild( object );
//		x += 1.5f;
//	}
//	if( false ){
//		CharRagdollType* skeletontype = asserted( dynamic_cast<CharRagdollType*>( CharMocapCollection::Get()->getResource( "T-Skeleton" ) ) );
//		CharSkinType* skintype = asserted( dynamic_cast<CharSkinType*>( CharMocapCollection::Get()->getResource( "Skin" ) ) );
//		Material* material = CharMocapCollection::Get()->getMaterial( "Material" );
//		Object* object = new Object();
//		CharRagdoll* skeleton = new CharRagdoll( skeletontype );
//		CharSkin* skin = new CharSkin( skintype, material, skeleton, object );
//		object->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//		skeleton->setRootPosOri( object->posori() );
//		area->addChild( object );
//		x += 1.5f;
//	}
//	if( false )
//	{
//		CharRagdollType* skeletontype = asserted( dynamic_cast<CharRagdollType*>( CharMocapCollection::Get()->getResource( "T-Skeleton" ) ) );
//		CharSkinType* skintype = asserted( dynamic_cast<CharSkinType*>( CharMocapCollection::Get()->getResource( "Skin" ) ) );
//		Material* material = CharMocapCollection::Get()->getMaterial( "Material" );
//		Object* object = new Object();
//		CharRagdoll* skeleton = new CharRagdoll( skeletontype );
//		CharSkin* skin = new CharSkin( skintype, material, skeleton, object );
//		object->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//		skeleton->setRootPosOri( object->posori() );
//		x += 1.5f;
//	}
//	if( false ){
//		AssImp assimp;
//		//assimp._mesh_transform = Mat4::Scale( 0.01f ) * Mat4::RotationAtAxis( -90, Vec3::Right ) * Mat4::RotationAtAxis( 180, Vec3::Forward );
//		//assimp._mesh_transform = Mat4::Scale( 0.01f );
//		//assimp._mesh_transform = Mat4();
//		//assimp._node_pretransform = assimp._mesh_transform;// * Mat4::RotationAtAxis( -90, Vec3::Right );
//		//assimp._node_posttransform = Mat4::RotationAtAxis( 90, Vec3::Right ) * Mat4::RotationAtAxis( 180, Vec3::Up );
//		//assimp.open( "/home/rt/media/mocap/FBX_Ninja_v27_Pro/Animation/Root_Motion/NJA_Cbt_Forward_Roll_v3.fbx" );
//		//assimp.open( "/home/rt/media/mocap/FBX_Ninja_v27_Pro/Animation/Root_Motion/NJA_Cbt_Run_Forward_Loop.fbx" );
//		assimp.open( "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Walk_F_Loop.fbx" );
//		CharRagdollType* newskeletontype = CharImporter::LoadSkeletonType( assimp );
//		{
//			Object* object = new Object();
//			CharRagdoll* animskeleton = new CharRagdoll( newskeletontype );
//			CharRagdollRenderer* renderer = new CharRagdollRenderer( animskeleton, object );
//			renderer->setPosOri( PosOri().translated( Vec3( x, 0, 2 ) ) );
//			x += 1.5f;
//		//	area->addChild( object );
//		}
//		{
//			Object* object = new Object();
//			CharAnimation* animation = CharImporter::LoadAnimation( assimp );
//			CharRagdoll* animskeleton = new CharRagdoll( newskeletontype );
//			CharRagdollRenderer* renderer = new CharRagdollRenderer( animskeleton, object );
//			renderer->setPosOri( PosOri().translated( Vec3( x, 0, 2 ) ) );
//			x += 1.5f;
//			new CharAnimatorTest( animation, animskeleton, object );
//			area->addChild( object );
//		}

//	}
//	{
//		{
//			AssImp assimp;
//			assimp._mesh_transform = Mat4::Scale( 0.01f ) * Mat4::RotationAtAxis( -90, Vec3::Right ) * Mat4::RotationAtAxis( 180, Vec3::Forward );
//			assimp._node_pretransform = assimp._mesh_transform;// * Mat4::RotationAtAxis( -90, Vec3::Right );
//			assimp._node_posttransform = Mat4::RotationAtAxis( 90, Vec3::Right ) * Mat4::RotationAtAxis( 180, Vec3::Up );
//			assimp.open( "/home/rt/media/Polygon_SciFi_Worlds/src/FBX/BR_Characters.fbx" );
//			//assimp.open( "/home/rt/media/Polygon_SciFi_Worlds/src/FBX/Characters.fbx" );
//			Material* material = RtsCollection::Get()->getMaterial( "m1a" );
//			for( uint m = 0; m < assimp.meshCount() && m < 11 ; m++ ){
//				MeshPNT* mesh = assimp.loadMeshPNT( m );
//				{
//					Object* object = new Object();
//					ObjectRenderableProperty* renderer = new ObjectRenderableProperty( mesh, material, object );
//					renderer->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//					area->addChild( object );
//					x += mesh->calcBoundingBox().size().x() + 0.5f;
//				}
//				{
//					Object* object = new Object();
//					CharRagdollType* skeletontype = CharImporter::LoadSkeletonType( assimp );
//					CharRagdoll* skeleton = new CharRagdoll( skeletontype );
//					CharRagdollRenderer* renderer = new CharRagdollRenderer( skeleton, object );
//					renderer->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//					area->addChild( object );
//					x += 2.5f;
//				}
//			}
//		}
//		{
//			AssImp assimp;
//			assimp._mesh_transform = Mat4::Scale( 0.01f ) * Mat4::RotationAtAxis( -90, Vec3::Right ) * Mat4::RotationAtAxis( 180, Vec3::Forward );
//			assimp._node_pretransform = assimp._mesh_transform;// * Mat4::RotationAtAxis( -90, Vec3::Right );
//			assimp._node_posttransform = Mat4::RotationAtAxis( 90, Vec3::Right ) * Mat4::RotationAtAxis( 180, Vec3::Up );
//			assimp.open( "/home/rt/media/Polygon_SciFi_Worlds/src/FBX/Characters.fbx" );
//			Material* material = RtsCollection::Get()->getMaterial( "m1a" );
//			for( uint m = 0; m < assimp.meshCount() && m < 11 ; m++ ){
//				MeshPNT* mesh = assimp.loadMeshPNT( m );
//				{
//					Object* object = new Object();
//					ObjectRenderableProperty* renderer = new ObjectRenderableProperty( mesh, material, object );
//					renderer->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//					area->addChild( object );
//					x += mesh->calcBoundingBox().size().x() + 0.5f;
//				}
//				{
//					Object* object = new Object();
//					CharRagdollType* skeletontype = CharImporter::LoadSkeletonType( assimp );
//					CharRagdoll* skeleton = new CharRagdoll( skeletontype );
//					CharRagdollRenderer* renderer = new CharRagdollRenderer( skeleton, object );
//					renderer->setPosOri( PosOri().translated( Vec3( x, 0, 0 ) ) );
//					area->addChild( object );
//					x += 2.5f;
//				}
//			}
//		}
//		{
//            MeshPNT* mesh = new MeshPNT();
//			MeshBuilder::CreatePosOri( *mesh, PosOri(), 0.5f, 0.03f, 6, true, VertexPNT() );
//            Object* object = new Object();
//            ObjectRenderableProperty* renderer = new ObjectRenderableProperty( mesh, material, object );
//            renderer->setPosOri( PosOri().translated( Vec3( x, 0, 0.3f ) ) );
//            area->addChild( object );
//        }
//        assimp.close();
//    }
//#endif
//}

