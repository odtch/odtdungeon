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
#include "character/CharMocapCollection.h"

#include "brain/Brain.h"

CharRagdoll* charragdoll1 = null;
SceneObject* char2 = null;
CharRagdoll* charragdoll2 = null;
CharCharacter* character = null;
Brain* brain = null;
List<BrainState*> states;

struct LearnControl {
	float t0 = 0;
	float t1 = 0;
	float move_y = 0;
};
struct LearnAnimation {
	CharAnimation* animation;
	List<LearnControl*> controls;
};
List<LearnAnimation*> learnanimations;
void loadLearnAnimation( CharAnimation* animation ){
	LearnAnimation* la = new LearnAnimation();
	la->animation = asserted( animation );
	LearnControl* c = new LearnControl();
	c->t0 = 0;
	c->t1 = animation->duration();
	la->controls.add( c );
	learnanimations.add( la );
}
float learntime = 0;
int learnanimationindex = 0;

DungeonScene::DungeonScene( Renderer* renderer )
    :Scene( renderer )
{
	_camera = new Camera();
}
DungeonScene::~DungeonScene(){
	states.deleteAll();
	_area1->deleteAllChilds();
	deleteAllChilds();
	_resources2delete.deleteAll();
	odelete( _camera );
	while( !learnanimations.isEmpty() ){
		LearnAnimation* anim = learnanimations.takeLast();
		anim->controls.deleteAll();
		delete anim;
	}
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
		learntime += dt;
		LearnAnimation* learnanimation = learnanimations.get( learnanimationindex );
		CharPose p;
		if( learntime < learnanimation->animation->duration() ){
		} else {
			learnanimation->animation->getPose( learnanimation->animation->duration(), p );
			char2->setPosOri( char2->posori().translated( p.translation() ));
			learntime = 0;
			learnanimationindex++;
			if( learnanimationindex >= learnanimations.size() ){
				learnanimationindex = 0;
				char2->setPosOri( PosOri() );
				character->setPosOri( PosOri().translated( Vec3( 4, 0, 0 ) ) );
			}
			learnanimation = learnanimations.get( learnanimationindex );
			learnanimation->animation->getPose( 0, p );
			char2->setPosOri( char2->posori().translated( p.translation() * -1 ));
		}
		learnanimation->animation->getPose( learntime, p );
		bool cf = false;
		for( LearnControl* c : learnanimation->controls ){
			if( c->t0 <= learntime && learntime < c->t1 ){
				assert( !cf );
				cf = true;
				character->control.move_y = c->move_y;
				//logDebug( character->control.move_y );
			}
		}
		assert( cf );
		charragdoll2->loadPose( p );
	}
	if( brain ){
		static float lt = 1;
		lt -= dt;
		if( lt < 0 ){
			//brain->learn( states, 1, 0.1f );
			lt = 0.2f;
		}
		static CharPose* pose = new CharPose();
//		if( brain_input == null ){
//			brain_input = new FloatBuffer( CharBrain::InputSize() );
//			brain_output = new FloatBuffer( CharBrain::OutputSize() );
//		}
		character->_ragdoll->savePose( *pose );
		CharBrain::LoadInput( brain->input(), character->control.move_y, *pose );
		brain->tick();

		CharBrain::LoadOutput( *pose, brain->output() );
//		logDebug( "t", pose->translation().y() );
//		static float x = 0;
//		x += 0.01f;
//		pose->setTranslation( pose->translation().yed( x ) );
		character->_ragdoll->loadPose( *pose );
	}


	Scene::animate( dt );
}
void DungeonScene::run(){
	{
		_camera->setPosOri( PosOri().translated( Vec3( 0, 12, 2 ) ).rotated( -180, Vec3::Up ) );
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
		Vec3 gr( 10, 50, 0.1f );
//		createStaticBox( Vec3( 0, gr.y(), 0 ), Vec3( gr.x(), 0.2f, 1.4f ), material );
//		//createStaticBox( Vec3( 0,-gr.y(), 0 ), Vec3( gr.x(), 0.2f, 0.2f ), material );
//		createStaticBox( Vec3( gr.x(), 0, 0 ), Vec3( 0.2f, gr.y(), 1.4f ), material );
//		createStaticBox( Vec3(-gr.x(), 0, 0 ), Vec3( 0.2f, gr.y(), 1.4f ), material );
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
		char2 = new SceneObject();
		CharRagdoll* ragdoll = new CharRagdoll( ragdolltype, char2 );
		char2->setPosOri( PosOri().translated( Vec3( 0, 2, 0 ) ) );
		new CharRagdollSkin( ragdoll, material, char2 );
		//new SceneRenderInstancePNTProperty( DungeonCollection::Get()->getMeshPNT( "banner02" ), DungeonCollection::Get()->getMaterial( "dt01" ), char2 );
		_area1->addChild( char2 );
		charragdoll2 = ragdoll;
	}
	character = new CharCharacter( DungeonCollection::Get()->get<CharRagdollType>( "rt" ), DungeonCollection::Get()->getMaterial( "fk01" ) );
	character->setPosOri( PosOri( Vec3( -9, -4, 0 ), Vec3::Right, Vec3::Up ) );
	character->setPosOri( PosOri().translated( Vec3( 2, 0, 0 ) ) );
	new SceneRenderInstancePNTProperty( DungeonCollection::Get()->getMeshPNT( "banner02" ), DungeonCollection::Get()->getMaterial( "dt01" ), character );
	_area1->addChild( character );
	{
//		loadLearnAnimation( CharMocapCollection::Get()->get<CharAnimation>( "MOB1_Stand_Relaxed_To_Walk_F" ) );
//		learnanimations.last()->controls.last()->move_y = 1;
//		loadLearnAnimation( CharMocapCollection::Get()->get<CharAnimation>( "MOB1_Walk_F_Loop" ) );
//		learnanimations.last()->controls.last()->move_y = 1;
//		loadLearnAnimation( CharMocapCollection::Get()->get<CharAnimation>( "MOB1_Walk_F_To_Stand_Relaxed" ) );
		//loadLearnAnimation( CharMocapCollection::Get()->get<CharAnimation>( "MOB1_Stand_Relaxed_Idle" ) );
//		loadLearnAnimation( CharMocapCollection::Get()->get<CharAnimation>( "MOB1_Stand_Relaxed_To_Walk_F" ) );
//		learnanimations.last()->controls.last()->move_y = 1;
		loadLearnAnimation( CharMocapCollection::Get()->get<CharAnimation>( "MOB1_Walk_F_Loop" ) );
		learnanimations.last()->controls.last()->move_y = 1;
//		loadLearnAnimation( CharMocapCollection::Get()->get<CharAnimation>( "MOB1_Walk_F_Loop" ) );
//		learnanimations.last()->controls.last()->move_y = 1;
//		loadLearnAnimation( CharMocapCollection::Get()->get<CharAnimation>( "MOB1_Walk_F_Loop_IPC" ) );
//		learnanimations.last()->controls.last()->move_y = 1;
//		loadLearnAnimation( CharMocapCollection::Get()->get<CharAnimation>( "MOB1_Walk_F_To_Stand_Relaxed" ) );
//		loadLearnAnimation( CharMocapCollection::Get()->get<CharAnimation>( "MOB1_Walk_F_To_Stand_Relaxed_LU" ) );
//		loadLearnAnimation( CharMocapCollection::Get()->get<CharAnimation>( "MOB1_Walk_F_To_Stand_Relaxed_RU" ) );
	}
	{
//		String txt;
//		txt += "t, x, y, dy, z\n";
		FloatBuffer input( CharBrain::InputSize() );
		FloatBuffer output( CharBrain::OutputSize() );
		logDebug( "create states for anim-count", learnanimations.size() );
		for( LearnAnimation* la : learnanimations ){
			float t0 = 0;
			float dt = 1.0f / 30;
			while( true ){
				float t1 = t0 + dt;
				if( t1 > la->animation->duration() )
					break;
				{
					CharPose p0; la->animation->getPose( t0, p0 );
					CharPose p1; la->animation->getPose( t1, p1 );
					//logDebug( "z", ( p1.translation().y() - p0.translation().y() ) );
					CharBrain::LoadInput( input, la->controls.first()->move_y, p0 );
					CharBrain::LoadOutput( output, p1 );
					BrainState* state = new BrainState( input, output );
					states.add( state );
					logDebug( p1.translation().y(), state->output().get( 0 ) );
				}
				t0 = t1;
			}
//			CharPose p;
//			la->animation->getPose( 0, p );
//			Vec3 t0 = p.translation();
//			la->animation->getPose( la->animation->duration(), p );
//			Vec3 t1 = p.translation();
//			Vec3 to = t1 - t0;
//			logDebug( "v", to.y() / la->animation->duration(), la->animation->duration(), la->animation->frames().size(), la->animation->id() );
		}
		assert( brain == null );
		brain = new Brain( CharBrain::InputSize(), CharBrain::OutputSize() );
		//brain->learn( states, 100, 0.51f );
		//brain->learn( states, 100, 0.1f );
		for( int i = 0; i < 100; i++ ){
			brain->learn( states, 5000, 0.01f );
		}
		brain->learn( states, 500, 0.001f );

//		assert( false );
//		for( CharAnimation* animation : as ){
//			CharPose pose;
//			animation->getPose( 0, pose );
//			Vec3 pt = pose.translation();
//			animation->getPose( animation->duration(), pose );
//			float sy = ( pose.translation().y() - pt.y() ) / animation->duration();
//			int s = 1;
//			while( true ){
//				float time = s * 1.0f / 60.0f;
//				if( animation->duration() < time )
//					break;
//				animation->getPose( time, pose );
//				Vec3 nt = pose.translation();
//				Vec3 t = nt - pt;
//				txt += String::FromFloat( time )
//					   + ", " + String::FromFloat( t.x() )
//					   + ", " + String::FromFloat( t.y() )
//					   + ", " + String::FromFloat( sy * time )
//					   + ", " + String::FromFloat( t.z() )
//					   + "\n";
//				s++;
//			}
//		}
////		File::CreateText( "/home/rt/temp/dungeonwalk.csv", txt );
////		assert( false );
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
