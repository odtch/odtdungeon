#include "stdodt.h"
#include "Physics.h"
#include "OdtPhysics.h"
#include "window/Window.h"
#include "JoltImpl.h"
#include <Jolt/Core/Factory.h>


//JPH::Body* myfloor = null;

// Callback for traces, connect this to your own trace function if you have one
static void TraceImpl(const char *inFMT, ...)
{
	// Format the message
	va_list list;
	va_start(list, inFMT);
	char buffer[1024];
	vsnprintf(buffer, sizeof(buffer), inFMT, list);

	// Print to the TTY
	//logDebug( buffer );
	logDebug( "jolt", buffer );
}

#ifdef JPH_ENABLE_ASSERTS

// Callback for asserts, connect this to your own assert handler if you have one
static bool AssertFailedImpl(const char *inExpression, const char *inMessage, const char *inFile, uint inLine)
{
	// Print to the TTY
	//logDebug( inFile << ":" << inLine << ": (" << inExpression << ") " << (inMessage != nullptr? inMessage : "") );
	logError( "jolt", inFile, inLine, inExpression, inMessage );
	// Breakpoint
	return true;
};

#endif // JPH_ENABLE_ASSERTS

// Layer that objects can be in, determines which other objects it can collide with
// Typically you at least want to have 1 layer for moving bodies and 1 layer for static bodies, but you can have more
// layers if you want. E.g. you could have a layer for high detail collision (which is not used by the physics simulation
// but only if you do collision testing).
namespace Layers
{
	static constexpr JPH::uint8 NON_MOVING = 0;
	static constexpr JPH::uint8 MOVING = 1;
	static constexpr JPH::uint8 NUM_LAYERS = 2;
};

class MyObjectLayerPairFilter : public JPH::ObjectLayerPairFilter
{
public:
	explicit MyObjectLayerPairFilter(){}
	virtual ~MyObjectLayerPairFilter() override{}
	virtual bool ShouldCollide( JPH::ObjectLayer inLayer1, JPH::ObjectLayer inLayer2 ) const override
	{
		switch (inLayer1)
		{
		case Layers::NON_MOVING:
			return inLayer1 == Layers::MOVING; // Non moving only collides with moving
		case Layers::MOVING:
			return true; // Moving collides with everything
		default:
			assert(false);
			return false;
		}
		return true;
	}

};

MyObjectLayerPairFilter MyObjectCanCollide;

// Each broadphase layer results in a separate bounding volume tree in the broad phase. You at least want to have
// a layer for non-moving and moving objects to avoid having to update a tree full of static objects every frame.
// You can have a 1-on-1 mapping between object layers and broadphase layers (like in this case) but if you have
// many object layers you'll be creating many broad phase trees, which is not efficient. If you want to fine tune
// your broadphase layers define JPH_TRACK_BROADPHASE_STATS and look at the stats reported on the TTY.
namespace BroadPhaseLayers
{
	static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
	static constexpr JPH::BroadPhaseLayer MOVING(1);
	static constexpr JPH::uint NUM_LAYERS(2);
};

// BroadPhaseLayerInterface implementation
// This defines a mapping between object and broadphase layers.
class MyBPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
{
public:
									MyBPLayerInterfaceImpl()
	{
		// Create a mapping table from object to broad phase layer
		mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
		mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
	}

	virtual JPH::uint					GetNumBroadPhaseLayers() const override
	{
		return BroadPhaseLayers::NUM_LAYERS;
	}

	virtual JPH::BroadPhaseLayer			GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override
	{
		assert(inLayer < Layers::NUM_LAYERS);
		return mObjectToBroadPhase[inLayer];
	}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
	virtual const char *			GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override
	{
		switch ((JPH::BroadPhaseLayer::Type)inLayer)
		{
		case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:	return "NON_MOVING";
		case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:		return "MOVING";
		default:
			ASSERT(false); return "INVALID";
		}
	}
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

private:
	JPH::BroadPhaseLayer					mObjectToBroadPhase[Layers::NUM_LAYERS];
};

class MyObjectVsBroadPhaseLayerFilter : public JPH::ObjectVsBroadPhaseLayerFilter
{
public:
	explicit MyObjectVsBroadPhaseLayerFilter(){}
	virtual ~MyObjectVsBroadPhaseLayerFilter() override{}
public:
	/// Returns true if an object layer should collide with a broadphase layer
	virtual bool					ShouldCollide( JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override
	{
		switch (inLayer1)
		{
		case Layers::NON_MOVING:
			return inLayer2 == BroadPhaseLayers::MOVING;
		case Layers::MOVING:
			return true;
		default:
			ASSERT(false);
			return false;
		}

	}
};

// Function that determines if two broadphase layers can collide
MyObjectVsBroadPhaseLayerFilter MyBroadPhaseCanCollide;

// An example contact listener
class MyContactListener : public JPH::ContactListener
{
public:
	// See: ContactListener
	virtual JPH::ValidateResult	OnContactValidate(const JPH::Body &inBody1, const JPH::Body &inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult &inCollisionResult) override
	{
		//logDebug( "Contact validate callback" );
		if( 0 < inBody1.GetUserData() ){
			if( 0 < inBody2.GetUserData() ){
				PhysicsBody* body1 = (PhysicsBody*) inBody1.GetUserData();
				PhysicsBody* body2 = (PhysicsBody*) inBody2.GetUserData();
				if( body1->_ignoreCollisionsWith == body2
					||
					body2->_ignoreCollisionsWith == body1
				){
					//logDebug( "ignoreCollisionsWith" );
					return JPH::ValidateResult::RejectAllContactsForThisBodyPair;
				}
			}

		}

		// Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
		return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
	}

	virtual void			OnContactAdded(const JPH::Body &inBody1, const JPH::Body &inBody2, const JPH::ContactManifold &inManifold, JPH::ContactSettings &ioSettings) override
	{
		//logDebug( "A contact was added" );
	}

	virtual void			OnContactPersisted(const JPH::Body &inBody1, const JPH::Body &inBody2, const JPH::ContactManifold &inManifold, JPH::ContactSettings &ioSettings) override
	{
		//logDebug( "A contact was persisted" );
	}

	virtual void			OnContactRemoved(const JPH::SubShapeIDPair &inSubShapePair) override
	{
		//logDebug( "A contact was removed" );
	}
};

// An example activation listener
class MyBodyActivationListener : public JPH::BodyActivationListener
{
public:
	virtual void		OnBodyActivated(const JPH::BodyID &inBodyID, JPH::uint64 inBodyUserData) override
	{
		//logDebug( "A body got activated" );
	}

	virtual void		OnBodyDeactivated(const JPH::BodyID &inBodyID, JPH::uint64 inBodyUserData) override
	{
		//logDebug( "A body went to sleep" );
	}
};

//class JoltWindowListener : public AbstractWindowListener {
//public:
//	static JoltWindowListener* Instance;
//public:
//	Physics* _physics;
//	Window* _window = null;
//public:
//	explicit JoltWindowListener( Window* window, Physics* physics )
//		:_physics( physics )
//		,_window( window )
//	{
//		assert( _physics );
//		assert( _window );

//	}
//	virtual ~JoltWindowListener(){
//		ASSERT( _window == null );
//		ASSERT( _physics == null );

//	}
//protected:
//	virtual void onWindowIdle( float dt ) override{
//		_physics->update( dt );
//	}
//protected:
//	virtual void onWindowCreated( Window* window ) override{
//		assert( false );
//		assert( window ); assert( _window == null );
//		_window = window;
//	}
//	virtual void onWindowDestroy() override {
//		assert( _window );
//		_window->removeListener( this );
//		_window = null;
//		_physics = null;
//	}
//};
//JoltWindowListener* JoltWindowListener::Instance = null;

Physics* Physics::_Global = null;
const char* Physics::Id = "JoltPhysics";
Physics* Physics::Get(){
	assert( _Global );
	return _Global;
}

Physics::Physics()
	:Module( Id )
{
	assert( _Global == null );
	_Global = this;
	_jolt_pc = PerformanceCounter::Create( "Physics Jolt" );
	_outside_pc = PerformanceCounter::Create( "Physics Outside" );
	// Register allocation hook
	JPH::RegisterDefaultAllocator();
	// Install callbacks
	JPH::Trace = TraceImpl;
	JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = AssertFailedImpl;)
	// Create a factory
	JPH::Factory::sInstance = new JPH::Factory();
	// Register all Jolt physics types
	JPH::RegisterTypes();
	// We need a temp allocator for temporary allocations during the physics update. We're
	// pre-allocating 10 MB to avoid having to do allocations during the physics update.
	// B.t.w. 10 MB is way too much for this example but it is a typical value you can use.
	// If you don't want to pre-allocate you can also use TempAllocatorMalloc to fall back to
	// malloc / free.
	_temp_allocator = new JPH::TempAllocatorImpl(1024 * 1024 * 1024);
	// We need a job system that will execute physics jobs on multiple threads. Typically
	// you would implement the JobSystem interface yourself and let Jolt Physics run on top
	// of your own job scheduler. JobSystemThreadPool is an example implementation.
	_job_system = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);
}
Physics::~Physics(){
	ASSERT( _Global == this );
	_Global = null;
}
void Physics::start(){
//	new PhysicsShapeType();
//	assert( window );
//	assert( JoltWindowListener::Instance == null );
//	JoltWindowListener::Instance = new JoltWindowListener( window, _Global );
//	window->addListener( JoltWindowListener::Instance );
	// This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an error.
	// Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
	const uint cMaxBodies = 65536;
	// This determines how many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for the default settings.
	const uint cNumBodyMutexes = 0;
	// This is the max amount of body pairs that can be queued at any time (the broad phase will detect overlapping
	// body pairs based on their bounding boxes and will insert them into a queue for the narrowphase). If you make this buffer
	// too small the queue will fill up and the broad phase jobs will start to do narrow phase work. This is slightly less efficient.
	// Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
	const uint cMaxBodyPairs = 65536;
	// This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected than this
	// number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
	// Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
	const uint cMaxContactConstraints = 65536;
	// Create mapping table from object layer to broadphase layer
	// Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
	_broad_phase_layer_interface = new MyBPLayerInterfaceImpl();
	// Now we can create the actual physics system.
	_physics_system = new JPH::PhysicsSystem();
	_physics_system->Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, *_broad_phase_layer_interface, MyBroadPhaseCanCollide, MyObjectCanCollide);
	_physics_system->SetGravity( JPH::Vec3( 0, 0, 0 ) ); // 9.81
	_physics_system->SetGravity( JPH::Vec3( 0, 0, -9.81f ) ); // 9.81
	_physics_system->SetGravity( JPH::Vec3( 0, 0, -9.81f * 10 ) ); // 9.81
	// A body activation listener gets notified when bodies activate and go to sleep
	// Note that this is called from a job so whatever you do here needs to be thread safe.
	// Registering one is entirely optional.
	_body_activation_listener = new MyBodyActivationListener();
	_physics_system->SetBodyActivationListener( _body_activation_listener );
	// A contact listener gets notified when bodies (are about to) collide, and when they separate again.
	// Note that this is called from a job so whatever you do here needs to be thread safe.
	// Registering one is entirely optional.
	_contact_listener = new MyContactListener();
	_physics_system->SetContactListener( _contact_listener);
	// The main way to interact with the bodies in the physics system is through the body interface. There is a locking and a non-locking
	// variant of this. We're going to use the locking version (even though we're not planning to access bodies from multiple threads)
	_body_interface = &_physics_system->GetBodyInterface();
}
void Physics::stop(){
	//	if( JoltWindowListener::Instance  ){
	//		odelete( JoltWindowListener::Instance  );
	//	}
}
void Physics::update( float dt ){
//	static float startup_sleep = 0.10f;
//	if( 0 < startup_sleep ){
//		startup_sleep -= dt;
//		return;
//	}
	_outside_pc->start();
	// We simulate the physics world in discrete time steps. 60 Hz is a good rate to update the physics system.
	const float cDeltaTime = 1.0f / 60.0f;
	assert( fequals( dt, cDeltaTime ) );
//	for( PhysicsCharacter* character : _characters ){
//		character->preUpdate( dt );
//	}
	// Next step
	++_step;
	// If you take larger steps than 1 / 60th of a second you need to do multiple collision steps in order to keep the simulation stable. Do 1 collision step per 1 / 60th of a second (round up).
	const int cCollisionSteps = 1;
	// Step the world
	_outside_pc->stop();
	_jolt_pc->start();
	_physics_system->Update( cDeltaTime, cCollisionSteps, _temp_allocator, _job_system );
	_jolt_pc->stop();
	_outside_pc->start();
	//_debugrenderer->Draw();
	for( auto body : _bodies ){
		body->onPhysicsUpdated();
	}
//	for( PhysicsCharacter* character : _characters ){
//		character->postUpdate();
//	}
	_outside_pc->stop();
}

////PhysicsShape* Physics::createSphereShape( float radius ){
////	return Phy
////	assert( 0 < radius );
////	return new PhysicsShape( new JPH::SphereShape( radius ) );
////}
////PhysicsShape* Physics::createBoxShape(const Vec3& radius){
////	return new PhysicsShape( new JPH::BoxShape( JPH::Vec3Arg(  radius.x(), radius.y(), radius.z() ) ) );
////}
////PhysicsShape* Physics::createConvexHullShape( const Mesh<VertexPNT>& mesh ){
////	return PhysicsShape::CreateConvexHullShape( mesh );
////}
////PhysicsShape* Physics::createConvexHullShape( const Mesh<VertexPNT>& mesh, std::function<bool( const VertexPNT& vertex )> filter ){
////	return PhysicsShape::CreateConvexHullShape( mesh, filter );
////}
////PhysicsShape* Physics::createMeshShape( const Mesh<VertexPNT>& mesh ){
////	return PhysicsShape::CreateMesh( mesh );
////}

////void JoltPhysics::onCharacterCreated( PhysicsCharacter* character ){
////	ASSERT( character );
////	_characters.add( character );
////}
////void JoltPhysics::onCharacterDestroyed( PhysicsCharacter* character ){
////	_characters.remove( character );
////}



