#include "stdodt.h"
//#include "CharCharacterType.h"
////#include "creator/CharCreator.h"
//////#include "CharCharacter.h"
//////#include "CharPoseSet.h"
//////#include "brain/CharBrainControl.h"


////CharBoneType::CharBoneType( const QString& name, CharBoneType* parent_bone, const RnPositionOrientation& joint_T_posori_in_skin )
////	:_name( name )
////	,_parent( parent_bone )
////	,_index_in_character( -1 )
////	,_joint_T_posori_in_skin( joint_T_posori_in_skin )
////	,_link( null )
////{
////	_absolute_T_matrix = joint_T_posori_in_skin.matrix();
////	if( isRoot() ){
////		_joint_T_posori_in_parent_bone = _joint_T_posori_in_skin;
////		_relative_T_matrix = _absolute_T_matrix;
////	} else {
////		_joint_T_posori_in_parent_bone = RnPosOri::map( parent()->joint_T_posori_in_skin().matrix().inverted(), _joint_T_posori_in_skin );
////		_relative_T_matrix = parent_bone->_absolute_T_matrix.inverted() * _absolute_T_matrix;
////	}
////}
////CharBoneType::~CharBoneType(){
////	_parent = null;
////}
////void CharBoneType::addSkinMatrixIndex( int skin_matrix_index ){
////	assert( 0 <= skin_matrix_index && skin_matrix_index < RnSkin::Max_Matrix_Count );
////	_skin_matrix_indexes.append( skin_matrix_index );
////}

////CharCharacterType::CharCharacterType( RnSkin* skin, float scale )
////////RnCollection* collection, const QString& skeletonName, const QString& skinName )
////	:_skin( skin )
////	,_scale( scale )
////	,_articulation( null )
////{
////	assert( _skin );
//////	assert( 0.1f < _scale );
////////	CharCreator creator( collection );
////////	creator.create( this );
//////////	max_step_length = ( getLinkByName( "leftfoot" )->startPosOri().position()
//////////			- getLinkByName( "rightfoot" )->startPosOri().position() ).length() * 2.08f;
//////////	int valueCount = countControlJointValues();
//////////	valueCount += _points.size() * 3 * 3;
//////////	_poses = new CharPoseSet( "/home/rt/cpp/game/character/bin/CharPoseSet.bin", valueCount );
//////////	_brainControl = new CharBrainControlType( this );
////////	_control_point_type_hard = PhControlPointType::CreateSphere( 0.1f, QVector4D( 0.5f, 1.0f, 0.5f, 1.0f ), 10000.0f, 10000.0f );
////////	_control_point_type_soft = PhControlPointType::CreateSphere( 0.1f, QVector4D( 0.5f, 0.5f, 1.0f, 1.0f ), 1000.0f, 1000.0f );
//////////	_animControlPointType = PhControlPointType::CreateSphere( 0.1f, QVector4D( 0.5f, 0.5f, 1.0f, 1.0f ), 10000.0f, 0.1f );
//////////	createAnimPointTypes();
////////	//_sweep_center_down = new PhSweepType::CreateSphere( 0.5f, 10.0f );
////////	_sweep_foot_down = PhSweepType::CreateSphere( 0.4f, 10 );
////	setBoundingBox( _skin->calcBoundingBox() );
////	for( int m = 0; m < RnSkin::Max_Matrix_Count; m++ )
////		_skinInvMatrices[ m ].setToIdentity();

////	_sweeps.lower = PhSweepType::CreateSphere( 2.0f, 15.0f );
////	//_sweeps.full = new PhSweepType( PhCapsuleShape::CreateZ( 2.0f, 16.0f ), 10 );
////	_sweeps.full = new PhSweepType( PhBoxShape::Create( QVector3D( 2.0f, 2.0f, 7.0f ) ), 10 );
////	//_sweeps.full = new PhSweepType( PhBoxShape::Create( QVector3D( 2.0f, 2.0f, 7.0f ) ), 10 );
////}
////CharCharacterType::~CharCharacterType(){
////	qDeleteAll( _bones );
////	_bones.clear();
//////////	qDeleteAll( _segments );
//////////	_segments.clear();
//////////	delete _poses;
//////////	_poses = null;
//////////	if( _brainControl ){
//////////		delete _brainControl;
//////////		_brainControl = null;
//////////	}
////	_skin = null;
////}
////void CharCharacterType::initialize(){
//////	_leftFoot.bone = getBoneForName( "LeftFoot" );
//////	_leftFoot.sweepType = new PhSweepType( _leftFoot.bone->_link->shape(), 6.0f );
//////	_rightFoot.bone = getBoneForName( "RightFoot" );
//////	_rightFoot.sweepType = new PhSweepType( _rightFoot.bone->_link->shape(), 6.0f );
////}
////CharBoneType* CharCharacterType::getBoneForName(const QString& name ){
////	foreach( auto bone, _bones ){
////		if( bone->_name == name )
////			return bone;
////	}
////	foreach( auto bone, _bones ){
////		qDebug() << "CharCharacterType::getBoneForName not found" << bone->_name;
////	}
////	assert( false );
////	throw Exception( "bonee for name not found" );
////}
////void CharCharacterType::addBone( CharBoneType* bone ){
////	assert( bone );
////	if( !bone->hasParent() ){
////		assert( _bones.isEmpty() );
////	} else {
////		assert( !_bones.isEmpty() );
////		assert( _bones.contains( bone->parent() ) );
////	}
////	assert( bone->_index_in_character == -1 );
////	bone->_index_in_character = _bones.size();
////	_bones.append( bone );
////	bone->_skin_inv_matrix = bone->_joint_T_posori_in_skin.matrix().inverted();
////	foreach( int skin_matrix_index, bone->skinMatrixIndexes() ){
////		assert( !_skin_matrix_index_to_bone.contains( skin_matrix_index ) );
////		_skin_matrix_index_to_bone[ skin_matrix_index ] =  bone;
////		_skinInvMatrices[ skin_matrix_index ] = bone->_skin_inv_matrix;
////	}
////}
//////////void CharCharacterType::addPoint(CharPointType *point){
//////////	ASSERT( point );
//////////	ASSERT( !_points.contains( point ) );
//////////	_points.append( point );
//////////}
//////////void CharCharacterType::createPoints( PhArticulationLinkType* link ){
//////////	ASSERT( false );
////////////	static PhShape* boxShape = PhControlPointType::CreateBoxShape( QVector3D( 0.5f, 1.5f, 0.3f ) );
////////////	static RnRenderable* boxRenderable = PhControlPointType::CreateBoxRenderable( QVector3D( 0.5f, 1.5f, 0.3f ), QVector4D( 0.5f, 0.5f, 1.0f, 1.0f ) );
////////////	static PhShape* sphereShape = PhControlPointType::CreateSphereShape( 0.5f );
////////////	static RnRenderable* sphereRenderable = PhControlPointType::CreateSphereRenderable( 0.5f, QVector4D( 0.5f, 0.5f, 1.0f, 1.0f ) );
////////////	RnPosOri anchorPosOri;
////////////	PhShape* shape = null;
////////////	RnRenderable* renderable = null;
////////////	float moveStiffness = -1;
////////////	float rotateStiffness = -1;
////////////	if( link == rootLink() ){
////////////		anchorPosOri = link->startPosOri();
////////////		moveStiffness = 100000;
////////////		rotateStiffness = 0;
////////////		shape = boxShape;
////////////		renderable = boxRenderable;
////////////	} else if( link->childJoints().size() == 0 ){
////////////		anchorPosOri = link->startPosOri();
////////////		moveStiffness = 1000;
////////////		rotateStiffness = 0;
////////////		shape = boxShape;
////////////		renderable = boxRenderable;
////////////	} else if( link->childJoints().size() == 1 ){
////////////		if( link->childJoints().first()->childLink()->childJoints().size() == 0 ){
////////////		} else {
////////////			auto joint = link->childJoints().first();
////////////			anchorPosOri = RnPosOri( joint->startPosOri().position(), RnOrientation() );
////////////			moveStiffness = 100;
////////////			rotateStiffness = 0;
////////////			shape = sphereShape;
////////////			renderable = sphereRenderable;
////////////		}
////////////	} else if( link->childJoints().size() > 1 ){
////////////		anchorPosOri = link->startPosOri();
////////////		moveStiffness = 10000;
////////////		rotateStiffness = 0;
////////////		shape = boxShape;
////////////		renderable = boxRenderable;
////////////	} else {
////////////		ASSERT( false );
////////////	}
////////////	if( link->name().startsWith( "foot" ) ){
////////////		moveStiffness = 1000;
////////////		rotateStiffness = 1000;
////////////	}
////////////	if( shape != null ){
////////////		//renderable = null;
////////////		RnPosOri posInLink = RnPosOri::map( link->startPosOri().matrix().inverted(), anchorPosOri );
////////////		CharPointType* point = new CharPointType( link, posInLink, shape, renderable, moveStiffness, rotateStiffness );
////////////		_points.append( point );
////////////	}
////////////	foreach( auto childJoint, link->childJoints() ){
////////////		createPoints( childJoint->childLink() );
//////////	//	}
//////////}
//////////void CharCharacterType::addSegment( CharSegmentType* segment ){
//////////	assert( segment );
//////////	_segments.append( segment );
//////////}
//////////CharPoseSet &CharCharacterType::poses() const {
//////////	ASSERT( _poses );
//////////	return *_poses;
//////////}
//////////void CharCharacterType::createAnimPointTypes(){
//////////	RnSkin* skin = this->skin();
//////////	const RnSkeleton* _animSkeleton = skin->skeleton();
//////////	foreach( PhArticulationLinkType* link, links() ){
//////////		//if( link->type()->childJoints().size() == 1 )
//////////		{
//////////			RnPosOri link_posori = link->startPosOri();
//////////			RnPosOri center_posori(
//////////						link_posori.position(),
//////////						RnOrientation( link_posori.up(), - link_posori.direction() ) );
//////////			//_lines.createCross( RnPosOri( center_posori.position(), center_posori.orientation() ), 5.0f );
//////////			for( int r = 0; r < 6; r++ ){
//////////				if( r == 0 ){
//////////				} else if( r < 4 ){
//////////					center_posori.setOrientation( center_posori.orientation().rotated( 90, center_posori.up() ) );
//////////				} else if( r == 4 ){
//////////					center_posori.setOrientation( center_posori.orientation().rotated( 90, center_posori.right() ) );
//////////				} else if( r == 5 ){
//////////					center_posori.setOrientation( center_posori.orientation().rotated( 180, center_posori.right() ) );
//////////				} else {
//////////					assert( false );
//////////				}
//////////				QRay3D ray( center_posori.position()
//////////				//			- _body->position()
//////////							, center_posori.direction() );
//////////				float dist = skin->meshes().at( 0 )->meshbuffer()->raycast( ray );
//////////				if( 0 < dist ){
//////////					QVector3D outerPosition = ray.point( dist );
//////////					//qDebug() << jointPosition << pointPosition;
//////////					//_lines.createCross( RnPosOri( outerPosition, center_posori.orientation() ), 5.0f );
////////////					_lines.createLine( outerPosition, center_posori.position(), QColor( 255, 255, 255 ) );
//////////					RnSkinVertex* vertex = skin->findNearestVertex( outerPosition );
//////////					bool influencesSkin = false;
//////////					foreach( PhArticulationSkinJointType* skinJointType, link->skinJoints() ){
//////////						if( skinJointType->skinJoint()->skinMatrixIndex() == vertex->getBone0Index() ){
//////////							influencesSkin = true;
//////////						}
//////////					}
//////////					if( influencesSkin ){
//////////						//_lines.createLine( outerPosition, animJoint->calcPosition(), QColor( 0, 255, 255 ) );
//////////						CharAnimPointType* animPoint = new CharAnimPointType();
//////////						_animPoints.append( animPoint );
//////////						animPoint->_globalPosition = outerPosition;
//////////						animPoint->_skinMatrixIndex = vertex->getBone0Index();
//////////						RnSkeletonJoint* animJoint = _animSkeleton->getJointBySkinMatrixIndex( animPoint->_skinMatrixIndex );
//////////						animPoint->_positionInSkinJoint = animJoint->absoluteMatrix().inverted().map( animPoint->_globalPosition );
//////////						animPoint->_link = link;
//////////						animPoint->_posori_in_link = RnPosOri::map( link->startPosOri().matrix().inverted(), RnPosOri( animPoint->_globalPosition, center_posori.orientation() ) );
//////////						//_lines.createCross( RnPosOri::map( link->startPosOri().matrix(), animPoint->_posori_in_link ), 3.0f );
//////////					}
//////////				}
//////////			}
//////////		}
//////////	}
//////////}


