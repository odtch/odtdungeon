#include "stdodt.h"
//#include "PhysicsCharacter.h"
//#include "JoltPhysics.h"
//#include "JoltImpl.h"
//#include "Jolt/Physics/Character/Character.h"
//#include "Jolt/Physics/Collision/Shape/CapsuleShape.h"
//#include "Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h"

//static const float cCharacterHeightStanding = 1.35f;
//static const float cCharacterRadiusStanding = 0.3f;
//static const float cCharacterHeightCrouching = 0.8f;
//static const float cCharacterRadiusCrouching = 0.3f;
//static const float cJumpSpeed = 4.0f;
//static const float cCollisionTolerance = 0.05f;

//struct PhysicsCharacterData {
//	JPH::Ref<JPH::Character>			mCharacter;
//	// The different stances for the character
//	JPH::RefConst<JPH::Shape>			mStandingShape;
//	JPH::RefConst<JPH::Shape>			mCrouchingShape;
//};

//void PhysicsCharacter::Input::reset(){
//	rotateLeft = 0;
//	rotateRight = 0;
//	moveLeft = 0;
//	moveRight = 0;
//	moveForward = 0;
//	moveBackward = 0;
//	crouch = 0;
//	jump = 0;
//	fly = 0;
//}

//PropertyType PhysicsCharacter::Type( "PhysicsCharacter" );
//PhysicsCharacter::PhysicsCharacter( Object* object, JoltPhysics* physics )
//	:Property( &Type )
//	,_physics( physics )
//	,_object( object )
//	,_data( new PhysicsCharacterData )
//{
//	input.reset();
//	assert( _physics );
//	assert( _object );
//	_posori = object->getProperty<PosOriProperty>();
//	_object->addProperty( this );

//	// Create capsule shapes for all stances
//	_data->mStandingShape = JPH::RotatedTranslatedShapeSettings(JPH::Vec3(0, 0, 0.5f * cCharacterHeightStanding + cCharacterRadiusStanding),
//																JPH::Quat::sRotation( JPH::Vec3( 1, 0, 0 ), glm::radians( 90.0f) ),
//																new JPH::CapsuleShape(0.5f * cCharacterHeightStanding, cCharacterRadiusStanding)).Create().Get();
//	_data->mCrouchingShape = JPH::RotatedTranslatedShapeSettings(JPH::Vec3(0, 0, 0.5f * cCharacterHeightCrouching + cCharacterRadiusCrouching ),
//																 JPH::Quat::sRotation( JPH::Vec3( 1, 0, 0 ), glm::radians( -90.0f) ),
//																 //JPH::Quat::sIdentity(),
//																 new JPH::CapsuleShape(0.5f * cCharacterHeightCrouching, cCharacterRadiusCrouching)).Create().Get();

//	// Create 'player' character
//	JPH::Ref<JPH::CharacterSettings> settings = new JPH::CharacterSettings();
//	settings->mLayer = 1; //Layers::MOVING;
//	settings->mShape = _data->mStandingShape;
//	settings->mFriction = 0.5f;


//	JPH::Vec3 jpos( _posori->get().position().x(), _posori->get().position().y(), _posori->get().position().z() );
//	// convert matrix to quat
//	glm::quat r = glm::quat_cast( _posori->get().matrix().asGlm() );
//	//std::cout << rotation.x << ", " << rotation.y << ", " << rotation.z << ", " << rotation.w << std::endl;
//	JPH::Quat jrot( r.x, r.y, r.z, r.w );


//	_data->mCharacter = new JPH::Character(settings, jpos, jrot, 0, _physics->_physics_system );

//	_data->mCharacter->AddToPhysicsSystem( JPH::EActivation::Activate);

//	_physics->onCharacterCreated( this );

//}
//PhysicsCharacter::~PhysicsCharacter(){
//	_physics->onCharacterDestroyed( this );
//	_data->mCharacter->RemoveFromPhysicsSystem();
//	odelete( _data );
//}
//void PhysicsCharacter::bind( PropertyObject* object ){
//	Property::bind( object );
//	//object->addListener( this );
//	//_posori->addListener( this );
//}

//void PhysicsCharacter::preUpdate( float dt )
//{
//	_rotation += ( input.rotateLeft - input.rotateRight ) * dt;
//	_base_orientation.rotate( -150.0f * ( input.rotateLeft - input.rotateRight ) * dt, Vec3::Up );
//	// Get the state of the character
//	JPH::Character::EGroundState ground_state = _data->mCharacter->GetGroundState();

//	Vec3 movement = _base_orientation.right() * ( input.moveRight - input.moveLeft )
//					+ _base_orientation.direction() * ( input.moveForward - input.moveBackward );
//	// Determine controller input
//	JPH::Vec3 control_input = JPH::Vec3::sZero();
//	control_input.SetX( movement.x() ); //input.moveLeft * -1 + input.moveRight );
//	control_input.SetY( movement.y() ); //input.moveBackward * -1 + input.moveForward );
//	if (control_input != JPH::Vec3::sZero())
//		control_input = control_input.Normalized();

//	// Cancel movement in opposite direction of normal when sliding
////	if (ground_state == JPH::Character::EGroundState::Sliding)
////	{
////		JPH::Vec3 normal = _data->mCharacter->GetGroundNormal();
////		normal.SetZ(0);
////		if (normal.Dot(control_input) <= 0.0f)
////			control_input = JPH::Vec3::sZero();
////	}

//	// Update velocity
//	const float cMaxSpeed = 6 * 10 * 0.1f;
//	JPH::Vec3 current_velocity = _data->mCharacter->GetLinearVelocity();
//	JPH::Vec3 desired_velocity = cMaxSpeed * control_input;
//	desired_velocity.SetZ(current_velocity.GetZ());
//	if( input.fly ){
//		desired_velocity += JPH::Vec3( 0, 0, 1 ) * 10;
//	} else if( desired_velocity.GetZ() > 1 ){
//		desired_velocity.SetZ( 1 );
//	}
//	JPH::Vec3 new_velocity = 0.75f * current_velocity + 0.25f * desired_velocity;

//	// Check actions
//	{
//		// Stance switch
//		if( input.crouch < 0.5f ){
//			if( _data->mCharacter->GetShape() != _data->mStandingShape ){
//				logDebug( "stand" );
//				_data->mCharacter->SetShape( _data->mStandingShape, 1.5f * _physics->_physics_system->GetPhysicsSettings().mPenetrationSlop );
//			}
//		} else {
//			if( _data->mCharacter->GetShape() != _data->mCrouchingShape ){
//				logDebug( "crouch" );
//				_data->mCharacter->SetShape( _data->mCrouchingShape, 1.5f * _physics->_physics_system->GetPhysicsSettings().mPenetrationSlop );
//			}
//		}
//	}
//	{
//		if( input.jump > 0.5f ){
//			// Jump
//			if( ground_state == JPH::Character::EGroundState::OnGround ){
//				input.jump = 0;
//				new_velocity += JPH::Vec3(0, 0, cJumpSpeed * 1 );
//			}
//		}
//	}

//	// Update the velocity
//	_data->mCharacter->SetLinearVelocity(new_velocity);

////	// Get properties
////	JPH::Vec3 position;
////	JPH::Quat rotation;
////	_data->mCharacter->GetPositionAndRotation(position, rotation);

//	/*
//	// Draw current location
//	// Drawing prior to update since the physics system state is also that prior to the simulation step (so that all detected collisions etc. make sense)
//	mDebugRenderer->DrawCoordinateSystem(Mat44::sRotationTranslation(rotation, position));

//	if (ground_state != Character::EGroundState::InAir)
//	{
//		Vec3 ground_position = _data->mCharacter->GetGroundPosition();
//		Vec3 ground_normal = _data->mCharacter->GetGroundNormal();
//		const PhysicsMaterial *ground_material = _data->mCharacter->GetGroundMaterial();

//		// Draw ground position
//		mDebugRenderer->DrawWireSphere(ground_position, 0.1f, Color::sRed);
//		mDebugRenderer->DrawArrow(ground_position, ground_position + 2.0f * ground_normal, Color::sGreen, 0.1f);

//		// Draw ground material
//		mDebugRenderer->DrawText3D(ground_position, ground_material->GetDebugName());
//	}
//	*/
//}

//void PhysicsCharacter::postUpdate()
//{
//	// Fetch the new ground properties
//	_data->mCharacter->PostSimulation(cCollisionTolerance);

//	JPH::Vec3 outPosition;
//	JPH::Quat outRotation;
//	_data->mCharacter->GetPositionAndRotation( outPosition, outRotation, true );

//	JPH::Mat44 m = JPH::Mat44::sRotationTranslation( outRotation, outPosition );
//	PosOri po;
//	setPosOriFromJPH( po, m );
//	po.setOrientation( _base_orientation );
//	_posori->set( po );
//}



///*
//#include <Jolt/Physics/PhysicsScene.h>
//#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
//#include <Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h>
//#include <Jolt/Core/StringTools.h>
//#include <Layers.h>
//#include <Utils/Log.h>
//#include <Renderer/DebugRendererImp.h>

//CharacterTest::~CharacterTest()
//{
//void CharacterTest::GetInitialCamera(CameraState &ioState) const
//{
//	// Position camera behind character
//	Vec3 cam_tgt = Vec3(0, cCharacterHeightStanding, 0);
//	ioState.mPos = Vec3(0, 2.5f, 5);
//	ioState.mForward = (cam_tgt - ioState.mPos).Normalized();
//}

//Mat44 CharacterTest::GetCameraPivot(float inCameraHeading, float inCameraPitch) const
//{
//	// Get properties
//	Vec3 position;
//	Quat rotation;
//	_data->mCharacter->GetPositionAndRotation(position, rotation);
//	return Mat44::sRotationTranslation(rotation, position);
//}

//void CharacterTest::CreateSettingsMenu(DebugUI *inUI, UIElement *inSubMenu)
//{
//	inUI->CreateTextButton(inSubMenu, "Select Scene", [this, inUI]() {
//		UIElement *scene_name = inUI->CreateMenu();
//		for (uint i = 0; i < size(sScenes); ++i)
//			inUI->CreateTextButton(scene_name, sScenes[i], [this, i]() { sSceneName = sScenes[i]; RestartTest(); });
//		inUI->ShowMenu(scene_name);
//	});
//}
// */


//PhysicsCharacterController::PhysicsCharacterController( PhysicsCharacter* character )
//	:_character( character )
//{
//	assert( _character);
//	rotation = Vec2( 10, 30 );
//}
//PhysicsCharacterController::~PhysicsCharacterController(){
//}
//bool PhysicsCharacterController::control( float dt ){
//	_character->input.rotateLeft = ( keyboard().isKeyDown( Keys::Q ) || keyboard().isKeyDown( Keys::LEFT ) ? 1 : 0 );
//	_character->input.rotateRight = ( keyboard().isKeyDown( Keys::E )  || keyboard().isKeyDown( Keys::RIGHT )  ? 1 : 0 );
//	_character->input.moveLeft = ( keyboard().isKeyDown( Keys::A ) ? 1 : 0 );
//	_character->input.moveRight = ( keyboard().isKeyDown( Keys::D ) ? 1 : 0 );
//	_character->input.moveForward = ( keyboard().isKeyDown( Keys::W ) ? 1 : 0 );
//	_character->input.moveBackward = ( keyboard().isKeyDown( Keys::S ) ? 1 : 0 );
//	_character->input.crouch = ( keyboard().isKeyDown( Keys::C ) ? 1 : 0 );
//	_character->input.jump = ( keyboard().isKeyDown( Keys::SPACE ) ? 1 : 0 );
//	_character->input.fly = ( keyboard().isKeyDown( Keys::SPACE ) ? 1 : 0 );
//	float mz = ( keyboard().isKeyDown( Keys::Z ) ? -1 : 0 ) + ( keyboard().isKeyDown( Keys::Q ) ? 1 : 0 );
//	float rx = 0; //( keyboard().isKeyDown( Keys::LEFT ) ? 1 : 0 ) + ( keyboard().isKeyDown( Keys::RIGHT ) ? -1 : 0 );
//	float ry = ( keyboard().isKeyDown( Keys::UP ) ? 1 : 0 ) + ( keyboard().isKeyDown( Keys::DOWN ) ? -1 : 0 );
//	_distance += ( keyboard().isKeyDown( Keys::R ) ? -1 : 0 ) + ( keyboard().isKeyDown( Keys::F ) ? 1 : 0 ) * dt * 30;
//	_distance *= ( keyboard().isKeyDown( Keys::T ) ? 0.9f : 1 ) * ( keyboard().isKeyDown( Keys::G ) ? 1.1f : 1 );
//	if( _distance < 1 )_distance = 1;
//	float ms = 100.5f * dt * 0.5f;
//	float rs = 81.5f * dt * 0.5f;
//	//camera().translate( ( camera().right() * mx + camera().direction() * my + camera().up() * mz ) * ms );
//	rotation.data.x += rx * rs;
//	rotation.data.y += ry * rs;
//	applyCamera();
////	if( std::abs( mx ) > 0.1f ){
////		//std::cout << "PhysicsCharacterController::animate keypressed\n";
////	}
////	if( keyboard().isKeyDown( Keys::V ) ){
////		//std::cout << "_autopilot->addTarget( " << _drone->posori().position().toCode() << ", " << _drone->posori().direction().toCode() << ", " << _drone->camera_rotation() << " );\n";
////		//std::cout << "_cameraanimator.add( " << camera().posori().toCode() << " );\n";
////		std::cout << "camera().setPosOri( PosOri( " << camera().posori().position().toString() << ", Orientation() ) ); ";
////		std::cout << "_rotation = vec2( " << rotation.x() << ", " << rotation.y() << " );\n";
////		std::cout.flush();
////	}
//	return Controller::control( dt );
//}
//void PhysicsCharacterController::applyCamera(){
//	Orientation ori;
//	ori = _character->_base_orientation;
//	ori.rotate( rotation.x(), Vec3::Up );
//	ori.rotate( rotation.y(), ori.right() );
//	PosOri po = _character->_posori->get();
//	po.setOrientation( ori );
//	po.translateLocal( Vec3::Backward * _distance );
//	po.translate( Vec3::Up * 0.3f );
//	//camera().setPosOri( PosOri( camera().posori().position(), ori ) );
//	camera().setPosOri( po );
//}

