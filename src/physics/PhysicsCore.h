#pragma once

#include "utils/Debugging.h"
#include "utils/Logger.h"
#include "math/PosOri.h"
#include "resource/ResourceCore.h"
#include "scene/Scene.h"
#include "scene/SceneObject.h"
#include "scene/SceneProperty.h"
//#include "resource/OdtResource.h"
//#include "object/OdtObject.h"

class Physics;

class PhysicsShape;

class PhysicsBody;
class PhysicsCharacter;

class PhysicsCaster;
class PhysicsRayCaster;
class PhysicsShapeCaster;
class PhysicsFollower;

class Physics;


namespace JPH {
	class TempAllocatorImpl;
	class JobSystemThreadPool;
	class PhysicsSystem;
	class Shape;
	class BodyInterface;
	class Body;
	class BodyID;
	class BodyFilter;
	class Character;
}

class MyBPLayerInterfaceImpl;
class MyBodyActivationListener;
class MyContactListener;
