#include "stdodt.h"
#include "Skin.h"
#include "SkinMesh.h"
#include "SkinMesher.h"
//#include "SkinAnimation.h"
//#include "resource/Lines3D.h"


//SkinJoint::SkinJoint( const String& name )
//	:_name( name )
//	,_parent( null )
//	,_relativeMatrixValid( false )
//	,_absoluteMatrixValid( false )
//	,_skinMatrixIndex( -1 )
//{
//}
//SkinJoint::~SkinJoint(){
//	_parent = null;
//	_childs.deleteAll();
//}
//bool SkinJoint::isChildOf( SkinJoint* joint ) const {
//	SkinJoint* j = _parent;
//	while( j != null ){
//		if( j == joint )
//			return true;
//		j = j->_parent;
//	}
//	return false;
//}
//void SkinJoint::addChild( SkinJoint* joint ){
//	ASSERT( joint );
//	ASSERT( joint->_parent == null );
//	joint->_parent = this;
//	_childs.add( joint );
//}
//const Mat4& SkinJoint::relativeMatrix() const {
//	if( !_relativeMatrixValid ){
//		assert( _absoluteMatrixValid );
//		if( hasParent() ){
//			_relativeMatrix = parent()->absoluteMatrix().inverted() * _absoluteMatrix;
//		} else {
//			_relativeMatrix = _absoluteMatrix;
//		}
//		_relativeMatrixValid = true;
//	}
//	return _relativeMatrix;
//}
//void SkinJoint::setRelativeMatrix( const Mat4& relativeMatrix ){
//	_relativeMatrix = relativeMatrix;
//	_relativeMatrixValid = true;
//	_absoluteMatrixValid = false;
//}
//const Mat4 &SkinJoint::absoluteMatrix() const {
//	if( !_absoluteMatrixValid ){
//		assert( _relativeMatrixValid );
//		if( hasParent() ){
//			_absoluteMatrix = parent()->absoluteMatrix() * _relativeMatrix;
//		} else {
//			_absoluteMatrix = _relativeMatrix;
//		}
//		_relativeMatrixValid = true;
//	}
//	return _absoluteMatrix;
//}
//void SkinJoint::setAbsoluteMatrix( const Mat4& absoluteMatrix ){
//	_relativeMatrixValid = false;
//	_absoluteMatrixValid = true;
//	_absoluteMatrix = absoluteMatrix;
//}
//void SkinJoint::recalcToRelativeMatrix(){
//	relativeMatrix();
//	for( SkinJoint* child : _childs ){
//		child->recalcToRelativeMatrix();
//	}
//	_absoluteMatrixValid = false;
//}
//void SkinJoint::setSkinMatrixIndex( int skinMatrixIndex ){
//	assert( _skinMatrixIndex == -1 );
//	assert( 0 <= skinMatrixIndex );
//	_skinMatrixIndex = skinMatrixIndex;
//}
//int SkinJoint::maxSkinMatrixIndex() const{
//	int m = _skinMatrixIndex;
//	for( SkinJoint* child : _childs ){
//		int mc = child->maxSkinMatrixIndex();
//		if( m < mc )
//			m = mc;
//	}
//	return m;
//}
//SkinJoint* SkinJoint::findJointByName( const String& name ){
//	if( _name == name )
//		return this;
//	for( SkinJoint* child : _childs ){
//		SkinJoint* result = child->findJointByName( name );
//		if( result )
//			return result;
//	}
//	return null;
//}
//SkinJoint *SkinJoint::findJointBySkinIndex(int skinIndex){
//	if( _skinMatrixIndex == skinIndex )
//		return this;
//	for( SkinJoint* child : _childs ){
//		SkinJoint* result = child->findJointBySkinIndex( skinIndex );
//		if( result )
//			return result;
//	}
//	return null;
//}
//SkinJoint* SkinJoint::clone() const {
//	SkinJoint* joint = new SkinJoint( _name );
//	joint->_relativeMatrixValid = _relativeMatrixValid;
//	joint->_relativeMatrix = _relativeMatrix;
//	joint->_absoluteMatrixValid = _absoluteMatrixValid;
//	joint->_absoluteMatrix = _absoluteMatrix;
//	joint->_skinMatrixIndex = _skinMatrixIndex;
//	for( SkinJoint* child : _childs ){
//		joint->addChild( child->clone() );
//	}
//	return joint;
//}
//void SkinJoint::transform( const Mat4& transform ){
//	for( SkinJoint* child : _childs ){
//		child->transform( transform );
//	}
//	setAbsoluteMatrix( absoluteMatrix() * transform );
//}
//void SkinJoint::foreachRecursive( const std::function<void (SkinJoint*)>& handler ){
//	handler( this );
//	for( auto child : _childs ){
//		child->foreachRecursive( handler );
//	}
//}
//void SkinJoint::foreachRecursiveConst( const std::function<void (const SkinJoint*)>& handler ) const {
//	handler( this );
//	for( auto child : _childs ){
//		child->foreachRecursiveConst( handler );
//	}
//}
//void SkinJoint::traceDebug( int level ){
//	for( int l = 0; l < level; l++ ){
//		std::cout << "  ";
//	}
//	std::cout << _name;
//	if( 0 <= _skinMatrixIndex )
//		std::cout << " skin " << _skinMatrixIndex;
//	std::cout << "\n";
//	for( auto child : _childs ){
//		child->traceDebug( level + 1 );
//	}
//}


//SkinSkeleton::SkinSkeleton(SkinJoint *root)
//	:_root( root)
//{
//	assert( _root );
//	loadJoint( _root );
//}
//SkinSkeleton::~SkinSkeleton(){
//	_joints.removeAll();
//	delete _root;
//}
//SkinJoint* SkinSkeleton::findJointByName(const String& name) const{
//	return _root->findJointByName( name );
//}
//SkinJoint* SkinSkeleton::getJointByName(const String &name) const {
//	SkinJoint* result = findJointByName( name );
//	if( result == null ){
//		std::cout << "SkinSkeleton::getJointByName not found " << name << "\n";
//		std::cout << " exists: ";
//		for( const SkinJoint* j : joints() ){
//			std::cout << j->name() << ", ";
//		}
//		std::cout << "\n"; std::cout.flush();
//		assert( false );
//	}
//	return result;
//}
//SkinJoint *SkinSkeleton::getJointBySkinIndex(int skinIndex) const {
//	assert( 0 <= skinIndex );
//	SkinJoint* joint = _root->findJointBySkinIndex( skinIndex );
//	assert( joint );
//	return joint;
//}
//void SkinSkeleton::loadJoint( SkinJoint* joint ){
//	_joints.add( joint );
//	for( SkinJoint* child : joint->childs() ){
//		loadJoint( child );
//	}
//}
//int SkinSkeleton::getOrCreateSkinMatrixId( SkinJoint* joint ){
//	assert( joint );
//	if( !joint->influencesSkin() ){
//		int skinMatrixIndex = _root->maxSkinMatrixIndex();
//		skinMatrixIndex += 1;
//		assert( 0 <= skinMatrixIndex );
//		joint->setSkinMatrixIndex( skinMatrixIndex );
//	}
//	return joint->skinMatrixIndex();
//}
//void SkinSkeleton::recalcRelativeMatrices(){
//	for( SkinJoint* joint : _joints ){
//		joint->relativeMatrix();
//	}
//}
//void SkinSkeleton::recalcToRelativeMatrices(){
//	_root->recalcToRelativeMatrix();
//}
//void SkinSkeleton::recalcAbsoluteMatrices(){
//	for( SkinJoint* joint : _joints ){
//		joint->absoluteMatrix();
//	}
//}
//void SkinSkeleton::load( const SkinSkeleton& source ){
//	for( SkinJoint* joint : _joints ){
//		const SkinJoint* src_joint = source.findJointByName( joint->name() );
//		assert( src_joint );
//		joint->setAbsoluteMatrix( src_joint->absoluteMatrix() );
//	}
//}
//void SkinSkeleton::transform( const Mat4& transform ){
//	recalcRelativeMatrices();
//	root()->setAbsoluteMatrix( root()->absoluteMatrix() * transform );
//	recalcAbsoluteMatrices();
//	root()->transform( transform );
//	for( SkinJoint* joint : joints() ){
//		joint->setAbsoluteMatrix( PosOri( joint->absoluteMatrix() ).matrix() );
//	}
//	recalcRelativeMatrices();
//}
//SkinSkeleton* SkinSkeleton::clone() const {
//	return new SkinSkeleton( _root->clone() );
//}
//void SkinSkeleton::createLines( Lines3D& lines ) const {
//	const float lw = 0.001f;
//	const float lw3 = lw * 4;
////	const float l = lw3 * 10;
//	for( SkinJoint* joint : _joints ){
//		PosOri joint_po( joint->absoluteMatrix() );
//		lines.addPosOri( joint_po, lw3, lw );
//		if( !joint->hasParent() ){
//		} else {
//			lines.add( PosOri( joint->parent()->absoluteMatrix() ).position(), joint_po.position(), lw, vec4( 0, 0, 1, 1 ) );
//		}
////		{
////			Vec3 p = joint->posori().position();
////			Orientation jo = joint->posori().orientation();
////			lines.add( p, p+jo.direction()*l, lw3, vec4( 1, 1, 1, 1 ) );
////			lines.add( p, p+jo.up()*l, lw3, vec4( 0, 1, 0, 1 ) );
////			lines.add( p, p+jo.right()*l, lw3, vec4( 0, 0, 1, 1 ) );
////		}
////		if( renderJointLimits ){
////			if( joint->hasParent() ){
////				//if( joint->type()->_limits.twist_min < joint->type()->_limits.twist_max )
////				{
////					const int step_count = 10;
////					Vec3 prev_pos;
////					for( int step = 0; step <= step_count; step++ ){
////						float ry = ((float)step)/ step_count;
////						Vec3 dir = joint->calcGlobalUp( Vec3( ry, 0.5f, 0.5f ) );
////						//std::cout << " " << step << " " << rz << " " << dir << "\n";
////						Vec3 pos = joint->posori().position()  + dir * 0.1f;
////						lines.add( pos, joint->posori().position(), lw, vec4( 0, 0.5f, 0, 1 ) );
////						if( 0 < step ){
////							lines.add( pos, prev_pos, lw, vec4( 0, 1, 0, 1 ) );
////						}
////						prev_pos = pos;
////					}
////				}
////				{
////					const int step_count = 10;
////					Vec3 prev_pos;
////					for( int step = 0; step <= step_count; step++ ){
////						float r = ((float)step)/ step_count;
////						Vec3 dir = joint->calcGlobalDirection( Vec3( 0.5f, r, 0.5f ) );
////						//std::cout << " " << step << " " << rz << " " << dir << "\n";
////						Vec3 pos = joint->posori().position()  + dir * 0.1f;
////						lines.add( pos, joint->posori().position(), lw, vec4( 0, 0.5f, 0, 1 ) );
////						if( 0 < step ){
////							lines.add( pos, prev_pos, lw, vec4( 0, 1, 0, 1 ) );
////						}
////						prev_pos = pos;
////					}
////				}
////				{
////					const int step_count = 10;
////					Vec3 prev_pos;
////					for( int step = 0; step <= step_count; step++ ){
////						float r = ((float)step)/ step_count;
////						Vec3 dir = joint->calcGlobalDirection( Vec3( 0.5f, 0.5f, r ) );
////						//std::cout << " " << step << " " << rz << " " << dir << "\n";
////						Vec3 pos = joint->posori().position()  + dir * 0.1f;
////						lines.add( pos, joint->posori().position(), lw, vec4( 0, 0.5f, 0, 1 ) );
////						if( 0 < step ){
////							lines.add( pos, prev_pos, lw, vec4( 0, 1, 0, 1 ) );
////						}
////						prev_pos = pos;
////					}
////				}
////				//if( joint->canRotateAtAxis( 0 ) || joint->canRotateAtAxis( 2 ) )
////				{
////					const int step_count = 30;
////					Vec3 prev_pos;
////					for( int step = 0; step <= step_count; step++ ){
////						float r = ((float)step * 360.0f )/ step_count;
////						Mat4 m = Mat4::RotationAtAxis( r, Vec3::Up );
////						Vec3 d = m.map( Vec3::Forward ).normalized();
////						Vec3 dir = joint->calcGlobalDirection( Vec3( 0.5f, ( 1.0f + d.x() ) / 2, ( 1.0f + d.y() ) / 2 ) );
////						Vec3 pos = joint->posori().position()  + dir * 0.1f;
////						lines.add( pos, joint->posori().position(), lw, vec4( 0, 0.5f, 0.5f, 1 ) );
////						if( 0 < step ){
////							lines.add( pos, prev_pos, lw, vec4( 0, 1, 1, 1 ) );
////						}
////						prev_pos = pos;
////					}
////				}
////			}
////		}
//	}

//}
////void SkinSkeleton::loadPose( const SkinAnimation& animation, const SkinPose& pose ) const {
////	_root->recalcByRelativeMatrix();
////	for( const SkinAnimationChannel* channel : animation.channels() ){
////		SkinJoint* joint = findJointByName( channel->name() );
////		if( joint == null ){
////			std::cout << "SkinSkeleton::loadPose joint not found " << channel->name() << "\n";
////		} else {
////			glm::quat rotation = pose.getRotation( channel->index() );
////			Mat4 rm = Mat4( glm::mat4_cast( rotation ) );
////			Mat4 jm = joint->relativeMatrix();
//////			for( int x = 0; x < 3; x++ ){
//////				for( int y = 0; y < 3; y++ ){
//////					float rv = rm(x,y);
//////					float& jv = jm(x,y);
//////					jv = rv;
//////				}
//////			}
////			joint->setRelativeMatrix( jm * rm );
////		}
////	}
////}

//Skin::Skin( SkinSkeleton* skeleton )
//	:_skeleton( skeleton )
//{
//	assert( _skeleton );
//}
//Skin::~Skin(){
//	delete _skeleton;
//	_skeleton = null;
//	_meshes.deleteAll();
//}
//void Skin::setInvMatrices(){
//	for( SkinJoint* joint : skeleton()->joints() ){
//		if( !joint->influencesSkin() )
//			continue;
//		_inv_matrices[ joint->skinMatrixIndex() ] = joint->absoluteMatrix().inverted();
//	}
//}
//void Skin::mapMeshesToSkeleton( const SkinSkeleton* skeleton ){
//	assert( false ); // geht nicht (mehr?)
////	const int MaxSkinMatrixCount = 60;
//	Mat4 skin_matrix[ MaxSkinMatrixCount ];
//	for( SkinJoint* my_joint : _skeleton->joints() ){
//		if( !my_joint->influencesSkin() )
//			continue;
//		int skin_matrix_index = my_joint->skinMatrixIndex();
//		SkinJoint* anim_joint = skeleton->getJointByName( my_joint->name() );
//		skin_matrix[ skin_matrix_index ] = _inv_matrices[ skin_matrix_index ] * anim_joint->absoluteMatrix();
//	}
////	skeleton->foreachJoint( [&]( SkinJoint* joint ){
////		if( joint->influencesSkin() ){
////			skim_m
////		}
////	});
//	for( auto mesh : _meshes ){
//		for( uint v = 0; v < mesh->vertexCount(); v++ ){
//			SkinVertex& vertex = mesh->vertex( v );
//			Vec3 position( vertex.position );
//			Vec3 normal( vertex.normal );
//			vertex.position = vec3( 0, 0, 0 );
//			vertex.normal = vec3( 0, 0, 0 );
//			for( int b = 0; b < 3; b++ ){
//				int bone_index = vertex.bone_indexes[b];
//				float bone_weight = vertex.bone_weights[b];
//				if( 0 <= bone_index && 0 < bone_weight ){
//					const Mat4& joint_matrix = skin_matrix[ bone_index ];
//					vertex.position += ( joint_matrix.map( position ) * bone_weight ).asGlm();
//					vertex.normal += ( joint_matrix.map( normal ) * bone_weight ).asGlm();
//				}
//			}
//			vertex.normal = Vec3( vertex.normal ).normalized();
//		}
//	}
//}
//void Skin::transform( const Mat4& transform ){
//	for( Mesh<SkinVertex>* skin_mesh : _meshes ){
//		for( uint v = 0; v < skin_mesh->vertexCount(); v++ ){
//			SkinVertex& vertex = skin_mesh->vertex( v );
//			vertex.position = transform.map( Vec3( vertex.position ) );
//			vertex.normal = transform.mapNormal( Vec3( vertex.normal ) );
//		}
//	}
//	_skeleton->transform( transform );
//}

Skin::Skin( SkinType* type, Material* material ) // , Object* object
	:/*ObjectProperty( object )
	,*/_type( asserted( type ) )
	,_material( asserted( material ) )
	,_mesh( "Skin", type->mesh().vertexCount(), type->mesh().indexCount() )
{
	for( uint v = 0; v < type->mesh().vertexCount(); v++ ){
		const SkinVertex& src = type->mesh().vertex( v );
		_mesh.addVertex( VertexPNT( src.position, src.normal, src.texcoord ) );
	}
	for( uint i = 0; i < type->mesh().indexCount(); i++ ){
		_mesh.addIndex( type->mesh().index( i ) );
	}
}
Skin::~Skin(){
}
void Skin::loadBones( const Skeleton& skeleton ){
	for( const SkinBoneType* bone : _type->bones() ){
		const SkeletonJoint* joint = skeleton.getJointByName( bone->jointname );
		ASSERT( 0 <= bone->index && bone->index < SkinMaxBoneCount );
		_bone_matrix[ bone->index ] = joint->absolutematrix() * bone->bone2mesh;
	}
	setBonesModified();
}
void Skin::setBonesModified(){
	_bones_modified = true;
}
