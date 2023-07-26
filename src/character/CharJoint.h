#pragma once

#include "CharCore.h"

class CharJointType {
	DEBUGCOUNTER(CharJointType)
private:
	CharRagdollType* _ragdoll = null;
private:
	int _index = -1;
private:
	String _name;
private:
	CharJointType* _parent = null;
	List<CharJointType*> _childs;
private:
	PosOri _relativeTposori;
//	Mat4 _absolute_matrix;
//	Mat4 _relative_matrix;
private:
	MeshPNT* _mesh = null;
private:
	explicit CharJointType();
	~CharJointType();
public:
	const CharRagdollType* ragdoll() const{ return _ragdoll; }
public:
	int index() const{ return _index; }
public:
	const String& name() const{ return _name; }
public:
	bool hasParent() const{ return _parent != null; }
	CharJointType* parent() const{ return asserted( _parent ); }
public:
	const PosOri& relativeTposori() const{ return _relativeTposori; }
//	const Mat4& absoluteMatrix() const{ return _absolute_matrix; }
//	const Mat4& relativeMatrix() const{ return _relative_matrix; }
public:
	bool hasMesh() const{ return _mesh != null; }
	MeshPNT* mesh() const{ ASSERT( _mesh ); return _mesh; }
public:
	void trace( int level ) const;
public:
	friend class CharRagdollType;
	friend class CharImporter;
};

class CharJoint {
	DEBUGCOUNTER(CharJoint)
private:
	CharJointType* _type = null;
private:
	CharJoint* _parent = null;
	List<CharJoint*> _childs;
private:
	mutable bool _absolutematrixvalid = false;
	mutable Mat4 _absolutematrix;
//	mutable bool _relativematrixvalid = false;
//	mutable Mat4 _relativematrix;
	bool _hasrotation = false;
	glm::quat _rotation;
	mutable bool _posorivalid = false;
	mutable PosOri _posori;
private:
	explicit CharJoint();
	~CharJoint();
public:
	const CharJointType* type() const{ return _type; }
	int index() const { return _type->index(); }
	const String& name() const { return _type->name(); }
public:
	bool hasParent() const{ return _parent != null; }
	CharJoint* parent() const{ return asserted( _parent ); }
public:
	const PosOri& posori() const;
	const Mat4& absolutematrix() const;
public:
	const glm::quat& rotation() const{ return _rotation; }
	void setRotation( const glm::quat& rotation );
//	void resetToRelative();
	void setAbsoluteMatrix( const Mat4& absolutematrix );
//	void setRelativeMatrix( const Mat4& relativematrix );
private:
	void invalidateAbsoluteMatrix();
public:
	friend class CharRagdoll;
};

