#include "stdodt.h"
#include "Skeleton.h"
#include "utils/Logger.h"

SkeletonJoint::SkeletonJoint(){
}
SkeletonJoint::~SkeletonJoint(){
}
bool SkeletonJoint::isChildOfRecursive( const SkeletonJoint* parent ) const {
	SkeletonJoint* p = _parent;
	while( p ){
		if( p == parent )
			return true;
		p = p->_parent;
	}
	return false;
}
const Mat4& SkeletonJoint::absolutematrix() const {
	if( !_absolutematrixvalid ){
		assert( _relativematrixvalid );
		if( !hasParent() ){
			_absolutematrix = _relativematrix;
		} else {
			_absolutematrix = parent()->absolutematrix() * _relativematrix;
		}
		_absolutematrixvalid = true;
	}
	return _absolutematrix;
}
void SkeletonJoint::setAbsoluteMatrix( const Mat4& absolutematrix ){
	_absolutematrix = absolutematrix;
	invalidateAbsoluteMatrix();
}
void SkeletonJoint::setRelativeMatrix( const Mat4& relativematrix ){
	_relativematrix = relativematrix;
	_relativematrixvalid = true;
	invalidateAbsoluteMatrix();
}
void SkeletonJoint::invalidateAbsoluteMatrix(){
	_absolutematrixvalid = false;
	for( SkeletonJoint* child : _childs ){
		child->invalidateAbsoluteMatrix();
	}
}
void SkeletonJoint::trace( int level ) const {
	String line;
	line += "  ";
	for( int l = 0; l < level; l++ ){
		line += "| ";
	}
	line += "+-";
	line += _name;	
	logDebug( line, absolutematrix().map( Vec3::Null ) );
	for( SkeletonJoint* child : _childs ){
		child->trace( level + 1 );
	}

}
//const PosOri&Joint::posori() const {
//	if( !_posorivalid ){
//		_posori.set( absolutematrix() );
//		_posorivalid = true;
//	}
//	return _posori;
//}
//const Mat4&Joint::absolutematrix() const{
//	if( !_absolutematrixvalid ){
//		assert( ! ( _relativematrixvalid && _hasrotation ) );
//		if( _relativematrixvalid ){
//			assert( !_hasrotation );
//		}  else {
//			_relativematrix = type()->relativeMatrix();
//		}
//		if( _hasrotation ){
//			assert( false );
//			_relativematrix *= Mat4::Rotation( _rotation );
//		}
//		if( _parent == null ){
//			_absolutematrix = _relativematrix;
//		} else {
//			_absolutematrix = _parent->absolutematrix() * _relativematrix;
//		}
//		_absolutematrixvalid = true;
//	}
//	return _absolutematrix;
//}
//void Joint::resetToRelative(){
//	_posorivalid = false;
//	_absolutematrixvalid = false;
//	_hasrotation = false;
//	for( Joint* child : _childs ){
//		child->resetToRelative();
//	}
//}
//void Joint::setAbsoluteMatrix( const Mat4& absolutematrix ){
//	//assert( !_posorivalid && !_absolutematrixvalid );
//	_absolutematrix = absolutematrix;
//	_absolutematrixvalid = true;
//	_posorivalid = false;
//}
//void Joint::setRelativeMatrix( const Mat4& relativematrix ){
//	assert( !_posorivalid && !_absolutematrixvalid );
//	_relativematrix = relativematrix;
//	_relativematrixvalid = true;
//	_absolutematrixvalid = false;
//	_posorivalid = false;
//}


Skeleton::Skeleton(){
}
Skeleton::~Skeleton(){
	_root = null;
	while( !_joints.isEmpty() ){
		SkeletonJoint* joint = _joints.takeLast();
		joint->_childs.removeAll();
		odelete( joint );
	}
}
void Skeleton::recalcAbsoluteMatrixFromRelative(){
	for( SkeletonJoint* joint : _joints ){
		if( joint->_parent == null ){
			joint->_absolutematrix = joint->_relativematrix;
		} else {
			joint->_absolutematrix = joint->_parent->_absolutematrix * joint->_relativematrix;
		}
	}
}
void Skeleton::transform( const Mat4& transform ){
	recalcAbsoluteMatrixFromRelative();
	for( SkeletonJoint* joint : _joints ){
		joint->_absolutematrix = PosOri( transform * joint->_absolutematrix ).matrix();
	}
	for( SkeletonJoint* joint : _joints ){
		if( joint->_parent == null ){
			joint->_relativematrix = joint->_absolutematrix;
		} else {
			joint->_relativematrix = joint->_parent->_absolutematrix.inverted() * joint->_absolutematrix;
		}
	}

}
void Skeleton::trace() const{
	logDebug( "AssImpSkeleton" );
	_root->trace( 0 );
}
SkeletonJoint* Skeleton::findJointByName( const String& name ) const {
	for( SkeletonJoint* joint : _joints ){
		if( joint->name().equalsIgnoreCase( name ) )
			return joint;
	}
	return null;
}
SkeletonJoint* Skeleton::getJointByName( const String& name ) const {
	SkeletonJoint* joint = findJointByName( name );
	if( joint )
		return joint;
	for( auto j : _joints ){
		logDebug( "  ", j->name() );
	}
	logError( "AssImpSkeleton.getJointByName", name, "not found" );
	assert( false );
}
SkeletonJoint*Skeleton::createJoint( const String& name, SkeletonJoint* parent, const Mat4& relativematrix ){
	SkeletonJoint* joint = new SkeletonJoint();
	joint->_index = _joints.size();
	_joints.add( joint );
	joint->_name = name;
	joint->_parent = parent;
	if( parent == null ){
		assert( _root == null );
		_root = joint;
	} else {
		assert( _root );
		parent->_childs.add( joint );
	}
	joint->_relativematrix = relativematrix;
	joint->_relativematrixvalid = true;
	return joint;
}
Mat4 Skeleton::calcRelativeMatrix( const String& childname, const String& parentname ) const {
	SkeletonJoint* child = getJointByName( childname );
	if( parentname.isEmpty() ){
		return child->absolutematrix();
	} else {
		SkeletonJoint* parent = getJointByName( parentname );
		assert( child->isChildOfRecursive( parent ) );
		PosOri c( child->absolutematrix() );
		PosOri p( parent->absolutematrix() );
		logDebug( "        calcRelativeMatrix ", childname, child->absolutematrix().map( Vec3::Null ), "to", parentname, parent->absolutematrix().map( Vec3::Null ),
			( parent->absolutematrix().inverted() * child->absolutematrix() ).map( Vec3::Null ),
				  ( p.matrix().inverted() * c.matrix().inverted() ).map( Vec3::Null )

				  );
		return parent->absolutematrix().inverted() * child->absolutematrix();
	}
}
//void AssImpSkeleton::setRootPosOri( const PosOri& rootposori ){
//	_root->resetToRelative();
//	_root->setAbsoluteMatrix( rootposori.matrix() );
//}
//void AssImpSkeleton::loadPose( const CharPose& pose ){
//	_root->resetToRelative();
//	for( Joint* joint : _joints ){
//		joint->_hasrotation = true;
//		joint->_rotation = pose.rotation( joint->index() );
//	}
//}
//void AssImpSkeleton::load( const AssImpSkeleton& skeleton ){
//	//assert( skeleton._type == _type );
//	assert( skeleton._joints.size() == _joints.size() );
//	for( int j = 0; j < _joints.size(); j++ ){
//		Joint* joint = _joints.get( j );
//		const Joint* srcjoint = skeleton._joints.get( j );
//		joint->setAbsoluteMatrix( srcjoint->absolutematrix() );
//	}
//}
//JointType* AssImpSkeletonType::findJointByName( const String& name ) const {
//	for( JointType* joint : _joints ){
//		if( joint->name() == name )
//			return joint;
//	}
//	return null;
//}
//JointType* AssImpSkeletonType::getJointByName( const String& name ) const {
//	JointType* joint = findJointByName( name );
//	if( joint )
//		return joint;
//	logError( "AssImpSkeletonType", id(), "getJointByName", name, "not found" );
//	assert( false );
//}


