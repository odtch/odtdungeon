#pragma once

#include "PhysicsCore.h"
//#include "JoltCore.h"

//struct PhysicsCharacterData;

//class PhysicsCharacter : public Property
//{
//	DEBUGCOUNTER(PhysicsCharacter)
//	ODTNOCOPY(PhysicsCharacter);
//public:
//	static PropertyType Type;
//public:
//	struct Input {
//		float rotateLeft;
//		float rotateRight;
//		float moveLeft;
//		float moveRight;
//		float moveForward;
//		float moveBackward;
//		float crouch;
//		float jump;
//		float fly;
//	public:
//		void reset();
//	} input;
//private:
//	JoltPhysics* _physics;
//private:
//	Object* _object = null;
//	PosOriProperty* _posori = null;
//private:
//	float _rotation = 0;
//	Orientation _base_orientation;
//private:
//	PhysicsCharacterData* _data = null;
//public:
//	explicit PhysicsCharacter( Object* object, JoltPhysics* physics );
//	virtual ~PhysicsCharacter();
//protected:
//	virtual void bind( PropertyObject* object ) override;
//public:
//	void preUpdate( float dt );
//	void postUpdate();
//public:
//	friend class JoltPhysics;
//	friend class PhysicsCharacterController;
//};

//#include "controller/Controller.h"

//class PhysicsCharacterController : public Controller
//{
//private:
//	PhysicsCharacter* _character;
//private:
//	Vec2 rotation;
//	float _distance = 130;
//public:
//	explicit PhysicsCharacterController( PhysicsCharacter* character );
//	virtual ~PhysicsCharacterController() override;
//protected:
//	virtual bool control( float dt ) override;
//private:
//	void applyCamera();
//};
