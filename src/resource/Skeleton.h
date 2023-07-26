#pragma once

#include "ResourceCore.h"

class SkeletonJoint {
	DEBUGCOUNTER(SkeletonJoint)
private:
	int _index = -1;
	String _name;
private:
	SkeletonJoint* _parent = null;
	List<SkeletonJoint*> _childs;
private:
	mutable bool _absolutematrixvalid = false;
	mutable Mat4 _absolutematrix;
	mutable bool _relativematrixvalid = false;
	mutable Mat4 _relativematrix;
private:
	SkeletonJoint();
	~SkeletonJoint();
public:
	int index() const{ return _index; }
	const String& name() const{ return _name; }
public:
	bool hasParent() const{ return _parent != null; }
	SkeletonJoint* parent() const{ return asserted( _parent ); }
	bool isChildOfRecursive( const SkeletonJoint* parent ) const;
public:
	const Mat4& absolutematrix() const;
	const Mat4& relativematrix() const;
public:
	void setAbsoluteMatrix( const Mat4& absolutematrix );
	void setRelativeMatrix( const Mat4& relativematrix );
private:
	void invalidateAbsoluteMatrix();
public:
	void trace( int level ) const;
public:
	friend class Skeleton;
};

class Skeleton
{
	DEBUGCOUNTER(Skeleton)
public:
private:
	SkeletonJoint* _root = null;
	List<SkeletonJoint*> _joints;
public:
	explicit Skeleton();
	virtual ~Skeleton();
public:
	SkeletonJoint* root(){ return _root; }
	const List<SkeletonJoint*>& joints() const{ return _joints; }
	SkeletonJoint* findJointByName( const String& name ) const;
	SkeletonJoint* getJointByName( const String& name ) const;
public:
	SkeletonJoint* createJoint( const String& name, SkeletonJoint* parent, const Mat4& relativematrix );
public:
	Mat4 calcRelativeMatrix( const String& childname, const String& parentname ) const;
private:
	void recalcAbsoluteMatrixFromRelative();
public:
	void transform( const Mat4& transform );
public:
	void trace() const;
public:
};

