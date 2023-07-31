#include "stdodt.h"
#include "CharRagdoll.h"
#include "CharJoint.h"
#include "CharResource.h"
#include "CharPose.h"
#include "skin/SkinMesh.h"
#include "scene/Scene.h"

CharRagdollType::CharRagdollType(){
}
CharRagdollType::~CharRagdollType(){
	_root = null;
	while( !_joints.isEmpty() ){
		delete _joints.takeLast();
	}
	_joint2skins.deleteAll();
	odelete( _skin );
}
CharJointType* CharRagdollType::findJointByName( const String& name ) const {
	for( CharJointType* joint : _joints ){
		if( joint->name() == name )
			return joint;
	}
	return null;
}
CharJointType* CharRagdollType::getJointByName( const String& name ) const {
	CharJointType* joint = findJointByName( name );
	if( joint )
		return joint;
	logError( "CharRagdollType", "getJointByName", name, "not found" );
	assert( false );
}
CharJointType* CharRagdollType::createJoint( CharJointType* parent ){
	CharJointType* joint = new CharJointType();
	joint->_ragdoll = this;
	joint->_index = _joints.size();
	_joints.add( joint );
	joint->_parent = parent;
	if( parent == null ){
		assert( _root == null );
		_root = joint;
	} else {
		assert( _root );
		assert( parent );
		assert( parent->ragdoll() == this );
		parent->_childs.add( joint );
	}
	return joint;
}
void CharRagdollType::collectPreLoad( List<AbstractMesh*>& meshes ){
	meshes.add( &_skin->mesh() );
}
//CharJointType* CharRagdollType::createJoint( const String& name, const Mat4& absoluteMatrix, CharJointType* parent ){
//	CharJointType* joint = createJoint( parent );
//	joint->_name = name;
//	joint->_absolute_matrix = absoluteMatrix;
//	if( parent == null ){
//		joint->_relative_matrix = joint->_absolute_matrix;
//	} else {
//		joint->_relative_matrix = joint->_parent->_absolute_matrix.inverted() * joint->_absolute_matrix;
//	}
//	return joint;
//}
ResourceType*CharRagdollType::type() const {
	return Singleton::Get<CharRagdollResourceType>();
}
void CharRagdollType::load( BinaryFileReader& reader ){
	//Resource::load( reader );
	uint jointcount = reader.read_uint32();
	while( _joints.size() < jointcount ){
		CharJointType* parent = null;
		if( _root == null ){
		} else {
			int parent_index = reader.read_uint32();
			parent = _joints.get( parent_index );
		}
		CharJointType* joint = createJoint( parent );
		reader.read_string( joint->_name );
		joint->_relativeTposori.load( reader );
		assert( joint->_mesh == null );
		if( reader.read_bool() ){
			joint->_mesh = new MeshPNT();
			joint->_mesh->load( reader );
		}
	}
	assert( _skin == null );
	_skin = new SkinType();
	_skin->load( reader );
	uint32_t joint2skin_count = reader.read_uint32();
	while( _joint2skins.size() < joint2skin_count ){
		JointToSkin* joint2skin = new JointToSkin();
		_joint2skins.add( joint2skin );
		uint32_t joint_index = reader.read_uint32();
		joint2skin->joint = _joints.get( joint_index );
		joint2skin->relativematrix.load( reader );
	}
	reader.read_magicnumber( 'e' );
}
void CharRagdollType::save( BinaryFileWriter& writer ) const {
	//Resource::save( writer );
	writer.write_uint32( _joints.size() );
	for( CharJointType* joint : _joints ){
		if( joint == _root ){
		} else {
			writer.write_uint32( joint->parent()->index() );
		}
		writer.write_string( joint->_name );
		joint->_relativeTposori.save( writer );
		if( joint->_mesh == null ){
			writer.write_bool( false );
		} else {
			writer.write_bool( true );
			joint->_mesh->save( writer );
		}
	}
	assert( _skin );
	_skin->save( writer );
	writer.write_uint32( _joint2skins.size() );
	for( JointToSkin* joint2skin : _joint2skins ){
		assert( joint2skin->joint );
		assert( _joints.contains( joint2skin->joint ) );
		writer.write_uint32( joint2skin->joint->index() );
		joint2skin->relativematrix.write( writer );
	}
	writer.write_uint32( 'e' );
}
void CharRagdollType::trace() const{
	logDebug( "CharRagdoll" );
	_root->trace( 0 );
}

CharRagdoll::CharRagdoll( CharRagdollType *type, SceneObject* object )
	:SceneProperty( object )
	,_type( asserted( type ) )
{
	for( CharJointType* jointtype : _type->joints() ){
		CharJoint* joint = new CharJoint();
		joint->_type = jointtype;
		if( _joints.isEmpty() ){
			_root = joint;
		} else {
			joint->_parent = _joints.get( jointtype->parent()->index() );
			joint->_parent->_childs.add( joint );
		}
		_joints.add( joint );
	}
	ASSERT( _root );
	ASSERT( _root->type() == _type->root() );
}
CharRagdoll::~CharRagdoll(){
	_type = null;
	while( !_joints.isEmpty() ){
		delete _joints.takeLast();
	}
}
CharJoint* CharRagdoll::getJoint( CharJointType* jointtype ) const {
	ASSERT( jointtype );
	ASSERT( jointtype->ragdoll() == _type );
	CharJoint* joint = _joints.get( jointtype->index() );
	ASSERT( joint->type() == jointtype );
	return joint;
}
CharJoint*CharRagdoll::findJointByName( const String& name ) const {
	for( CharJoint* joint : _joints ){
		if( joint->name() == name )
			return joint;
	}
	return null;
}
CharJoint*CharRagdoll::getJointByName( const String& name ) const {
	CharJoint* joint = findJointByName( name );
	if( joint )
		return joint;
	logError( "CharRagdoll", _type, "getJointByName", name, "not found" );
	assert( false );
}
void CharRagdoll::loadPose( const CharPose& pose ){
	//_root->invalidateAbsoluteMatrix();
	ASSERT( _joints.size() == CharBoneCount );
	for( CharJoint* joint : _joints ){
		joint->setRotation( pose.rotation( joint->index() ) );
	}
	//_root->setAbsoluteMatrix( Mat4::Translation( pose.translation() ) );
	_root->setAbsoluteMatrix( Mat4::Translation( pose.translation() ) * _root->absolutematrix() );
}
void CharRagdoll::savePose( CharPose& pose ) const {
	ASSERT( _root );
	pose.setTranslation( _root->absolutematrix().map( Vec3::Null ) );
	ASSERT( _joints.size() == CharBoneCount );
	for( CharJoint* joint : _joints ){
		pose.setRotation( joint->index(), joint->rotation() );
	}
}
//void CharRagdoll::setRootPosOri( const PosOri& rootposori ){
//	_root->resetToRelative();
//	_root->setAbsoluteMatrix( rootposori.matrix() );
//}
//void CharRagdoll::load( const CharRagdoll& ragdoll ){
//	//assert( ragdoll._type == _type );
//	assert( ragdoll._joints.size() == _joints.size() );
//	for( int j = 0; j < _joints.size(); j++ ){
//		CharJoint* joint = _joints.get( j );
//		const CharJoint* srcjoint = ragdoll._joints.get( j );
//		joint->setAbsoluteMatrix( srcjoint->absolutematrix() );
//	}
//}

//CharRagdollSkeletonRenderer::CharRagdollSkeletonRenderer( CharRagdoll* ragdoll, Object* object )
//	:ObjectRenderableProperty( &_mesh, &_material, object )
//	,_ragdoll( asserted( ragdoll ) )
//{
//	rebuild();
//}
//CharRagdollSkeletonRenderer::~CharRagdollSkeletonRenderer(){
//	odelete( _ragdoll );
//	_ragdoll = null;
//}
//void CharRagdollSkeletonRenderer::animate( float dt ){
//	rebuild();
//	ObjectRenderableProperty::animate( dt );
//}
//void CharRagdollSkeletonRenderer::rebuild(){
//	_mesh.clear();
////	static float o = 0;
////	o += 0.1f;
////	if( o > 1 )o = 0;
//	for( CharJoint* joint : _ragdoll->joints() ){
//		PosOri posori( joint->posori() );
//		posori.setPosition( posori.position() );
////		posori.translate( Vec3::Up * o );
//		MeshBuilder::CreatePosOri( _mesh, posori, 0.4f, 0.005f, 6, true, VertexPNT() );
//		if( joint->type()->hasParent() ){
//			CharJoint* parent = joint->parent();
//			PosOri parentposori( parent->posori() );
//			parentposori.setPosition( parentposori.position() );
//			if( 0.0001f < ( parentposori.position() - posori.position() ).length() ){
//				MeshBuilder::CreateLine( _mesh, parentposori.position(), posori.position(), 0.01f, 6, true, VertexPNT() );
//			}
//		}
//	}
//}


CharRagdollSkin::CharRagdollSkin( CharRagdoll* ragdoll, Material* material, SceneObject* object )
	:Skin( asserted(ragdoll)->type()->skin(), material, object )
	,_ragdoll( asserted( ragdoll ) )
{
	assert( _ragdoll->node() == node() );
//	for( CharJoint* joint : _ragdoll->joints() ){
//		if( !joint->type()->hasMesh() ){
//			logDebug( "CharRagdoll joint no mesh", joint->type()->name() );
//			continue;
//		}
//		Renderer* renderer = new Renderer();
//		renderer->_joint = joint;
//		renderer->_renderer = new ObjectRenderableProperty( joint->type()->mesh(), material, object );
//		_renderers.add( renderer );
//	}
}
CharRagdollSkin::~CharRagdollSkin(){
	//	_renderers.deleteAll();
}
void CharRagdollSkin::animate( float dt ){

	Mat4* bonematrix = & this->bone_matrix( 0 );
	int boneindex = 0;
	for( CharRagdollType::JointToSkin* joint2skin : _ragdoll->type()->joint2skins() ){
		CharJoint* joint = _ragdoll->getJoint( joint2skin->joint );
		ASSERT( bonematrix == &this->bone_matrix( boneindex ) );
		(*bonematrix) = joint->absolutematrix() * joint2skin->relativematrix * this->type()->bones().get( boneindex )->bone2mesh;
		bonematrix++;
		boneindex++;
	}
	this->setBonesModified();
	Skin::animate( dt );
//	for( Renderer* renderer : _renderers ){
//		renderer->_renderer->setPosOri( PosOri( _posori.matrix() * renderer->_joint->posori().matrix() ) );
//	}
}

