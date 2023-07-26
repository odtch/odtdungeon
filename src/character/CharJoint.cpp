#include "stdodt.h"
#include "CharRagdoll.h"
#include "CharJoint.h"
#include "CharResource.h"
#include "CharPose.h"

CharJointType::CharJointType(){
}
CharJointType::~CharJointType(){
	_childs.removeAll();
	odelete( _mesh );
}
void CharJointType::trace( int level ) const {
	String line;
	line += "  ";
	for( int l = 0; l < level; l++ ){
		line += "| ";
	}
	line += "+-";
	line += _name;
	logDebug( line );
	for( CharJointType* child : _childs ){
		child->trace( level + 1 );
	}
}

CharJoint::CharJoint()
	:_rotation( 0, 0, 0, 0 )
{
}
CharJoint::~CharJoint(){
	_childs.removeAll();
}
const PosOri&CharJoint::posori() const {
	if( !_posorivalid ){
		_posori.set( absolutematrix() );
		_posorivalid = true;
	}
	return _posori;
}
const Mat4&CharJoint::absolutematrix() const{
	if( !_absolutematrixvalid ){
//		assert( ! ( _relativematrixvalid && _hasrotation ) );
//		if( _relativematrixvalid ){
//			assert( !_hasrotation );
//		}  else {
//			_relativematrix = type()->relativeMatrix();
//		}
//		if( _relativematrixvalid ){
//		} else {
const Mat4&			_relativematrix = _type->relativeTposori().matrix();
//		}
//		if( _hasrotation ){
//			_relativematrix *= Mat4::Rotation( _rotation );
//		}
		if( _parent == null ){
			_absolutematrix = _relativematrix;
		} else {
			_absolutematrix = _parent->absolutematrix() * _relativematrix;
		}
		//if( _hasrotation )
		{
			_absolutematrix *= Mat4::Rotation( _rotation );
		}
//_absolutematrix = _relativematrix;
		_absolutematrixvalid = true;
	}
	return _absolutematrix;
}
void CharJoint::setRotation( const glm::quat& rotation ){
	_rotation = rotation;
	_hasrotation = true;
	invalidateAbsoluteMatrix();
}
void CharJoint::setAbsoluteMatrix( const Mat4& absolutematrix ){
	invalidateAbsoluteMatrix();
	_absolutematrix = absolutematrix;
	_absolutematrixvalid = true;
}
//void CharJoint::setRelativeMatrix( const Mat4& relativematrix ){
//	invalidateAbsoluteMatrix();
//	_relativematrix = relativematrix;
//	_relativematrixvalid = true;
//}
void CharJoint::invalidateAbsoluteMatrix(){
	_posorivalid = false;
	_absolutematrixvalid = false;
	for( CharJoint* child : _childs ){
		child->invalidateAbsoluteMatrix();
	}
}
//void CharJoint::resetToRelative(){
//	_posorivalid = false;
//	_absolutematrixvalid = false;
//	_hasrotation = false;
//	for( CharJoint* child : _childs ){
//		child->resetToRelative();
//	}
//}
//void CharJoint::setAbsoluteMatrix( const Mat4& absolutematrix ){
//	//assert( !_posorivalid && !_absolutematrixvalid );
//	_absolutematrix = absolutematrix;
//	_absolutematrixvalid = true;
//	_posorivalid = false;
//}
//void CharJoint::setRelativeMatrix( const Mat4& relativematrix ){
//	assert( !_posorivalid && !_absolutematrixvalid );
//	_relativematrix = relativematrix;
//	_relativematrixvalid = true;
//	_absolutematrixvalid = false;
//	_posorivalid = false;
//}
