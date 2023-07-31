#include "stdodt.h"
#include "CharCharacter.h"
#include "CharCharacterType.h"
#include "CharPose.h"
#include "CharAnimation.h"
#include "CharRagdoll.h"
//#include "CharSegment.h"
//#include "CharFootStepCalcer.h"
//#include "CharBehavior.h"
//#include "brain/CharBrainControl.h"

CharCharacter::CharCharacter( CharRagdollType* ragdolltype, Material* material ){
	_ragdoll = new CharRagdoll( ragdolltype, this );
	new CharRagdollSkin( _ragdoll, material, this );
}
CharCharacter::~CharCharacter(){
}

void CharCharacter::animate( float dt ){
//	Vec2 accel( 0, 0 );
//	if( 0 < control.move_y ){
//		accel += Vec2( 0, 1.19f );
//	} else if( _velocity.y() > 0 ){
//		accel += Vec2( 0, -_velocity.y() );
//	}
////	float maxaccell = 1.9635f;
////	if( accel.length() > maxaccell )accel = accel.normalized() * maxaccell;
//	_velocity += accel * dt ;//* 0.5f;
//	float max_vel = 1.28195f;
//	if( _velocity.length() > max_vel )_velocity = _velocity.normalized() * max_vel;
//	Vec2 t = _velocity * dt;
//	PosOri posori = this->posori();
//	posori.translate( posori.direction() * t.y() );
//	//posori.translate( posori.direction() * control.move_y * dt * 10  );

//	setPosOri( posori );
	SceneObject::animate( dt );
}




//const int CharCharacter::ClassId = CharCharacter::CreateClassId();

//CharCharacter::CharCharacter( CharCharacterType* type )
//	:RnObject( type )
//////	PhArticulation( type )
////	,_action( &CharAnimationSystem::Get()->_stand )
////	,_transition( null )
////	,_time_in_transition( 0 )
////	,_firstAnimate( true )
////	,_controlMode( UnhandledControlMode )
//	,_controlMode( ControlControlMode )
//	,_navigator( null )
//	,_notCharacterFilter( this )
//	,_articulation( null )
//	,_locomotion_time_factor( 0 )
//	,_renderSkin( true )
//	,_renderDebug( false )
//	,_debugMesh( null )
//	,_debugLines( null )
//{
//	for( int i = 0; i < RnSkin::MaxPartVariants; i++ )
//		_skinPartVariants[ i ] = 0;
//	for( int m = 0; m < RnSkin::Max_Matrix_Count; m++ )
//		_skinMatrices[ m ].setToIdentity();
//	foreach( CharBoneType* bone_type, type->bones() ){
//		CharBone* parent = null;
//		if( bone_type->hasParent() ){
//			parent = getBoneForType( bone_type->parent() );
//		}
//		CharBone* bone = new CharBone( parent, bone_type );
//		assert( bone->type()->index_in_character() == _bones.size() );
//		_bones.append( bone );
//		foreach( int skmi, bone->type()->skinMatrixIndexes() ){
//			bone->_skin_matrixes.append( &( _skinMatrices[ skmi ] ) );
//		}
//	}
//	_articulation = new PhArticulation( type->_articulation );
//	add( _articulation );
//	_articulation->setJointsStiffnessDamping( 1000, 50 );
//	for( int b = 0; b < _bones.size(); b++ ){
//		CharBone* bone = _bones.at( b );
//		bone->_link = _articulation->getLinkForType( bone->type()->_link );
//		assert( bone->_link->type() == bone->type()->_link );
//		bone->_prev_posori = bone->type()->joint_T_posori_in_skin();
//		bone->_next_posori = bone->type()->joint_T_posori_in_skin();
//		auto cs = new PhContactSensors( bone->_link );
//		cs->setFilter( &_notCharacterFilter );
//	}
//	foreach( auto sequence, CharAnimationSystem::Get()->_sequences ){
//		_animationBlender.createEntry( sequence );
//	}
//	_animationBlender.setPower( 0, 1.0f );
////	initEndPoint( _leftFoot, type->_leftFoot );
////	initEndPoint( _rightFoot, type->_rightFoot );
////////	foreach( auto pointType, type->points() ){
////////		auto link = getLinkForType( pointType->link() );
////////		CharPoint* point = new CharPoint( link, pointType );
////////		//qDebug() << link->type()->name();
////////		_points.append( point );
////////		add( point );
////////	}
////////	foreach( auto segmentType, type->segments() ){
////////		CharSegment* segment = new CharSegment( this, segmentType );
////////		_segments.append( segment );
////////	}
//////	init_side( _l, "left" );
//////	init_side( _r, "right" );
////////	foreach( CharAnimPointType* animPointType, type->animPoints() ){
////////		CharAnimPoint* animPoint = new CharAnimPoint();
////////		animPoint->_time_until_breakable = 1.0f;
////////		animPoint->_type = animPointType;
////////		animPoint->_link = getLinkForType( animPointType->_link );
////////		animPoint->_controlPoint = new PhControlPoint( animPoint->_link, animPointType->_posori_in_link, type->animControlPointType() );
////////		animPoint->_controlPoint->setEnabled( true );
////////		addAnimationPoint( animPoint );
////////	}
////////	new CharWalkBehavior( this );
///////
//////auto cpt = PhControlPointType::CreateSphere( 0.5f, QVector4D( 1, 1, 1, 1 ), 100000, 100000 );
//////_cp_center = new PhControlPoint( getBoneForType( type->getBoneForName( "Spine_1" ) )->_link, PosOri(), cpt );
//////add( _cp_center );
//////_cp_foot_l = new PhControlPoint( getBoneForType( type->getBoneForName( "LeftFoot" ) )->_link, PosOri(), cpt );
//////add( _cp_foot_l );
//////_cp_foot_r = new PhControlPoint( getBoneForType( type->getBoneForName( "RightFoot" ) )->_link, PosOri(), cpt );
//////add( _cp_foot_r );

//////_cp_center->setEnabled( false );
//////_cp_foot_l->setEnabled( false );
//////_cp_foot_r->setEnabled( false );
////	_sweeps.lower_down = createSweep( type->_sweeps.lower );
//	_sweep = createSweep( type->_sweeps.lower );
//	_sweep->setFilter( &_notCharacterFilter );
////	_sweeps.full = createSweep( type->_sweeps.full );
//}
//CharCharacter::~CharCharacter(){
//	if( _navigator ){
//		remove( _navigator );
//		delete _navigator;
//	}
//	remove( _articulation );
//	delete _articulation;
//	_articulation = null;
//	qDeleteAll( _bones );
//	_bones.clear();
//	clear();
//////	if( _temp_pose ){
//////		delete _temp_pose;
//////		_temp_pose = null;
//////	}
//////	if( _pose ){
//////		delete _pose;
//////		_pose = null;
//////	}
//////	foreach( auto point, _control_points ){
//////		remove( point );
//////		delete point;
//////	}
//////	_control_points.clear();
//////	destroy_side( _l );
//////	destroy_side( _r );
//////	if( _brainControl ){
//////		delete _brainControl;
//////		_brainControl = null;
//////	}
//////	qDeleteAll( _segments );
//////	_segments.clear();
//////	while( !_points.isEmpty() ){
//////		auto point = _points.takeLast();
//////		remove( point );
//////		delete point;
//////	}
//////	qDeleteAll( _behaviors );
//////	_behaviors.clear();
//////	while( !_animationPoints.isEmpty() ){
//////		auto animPoint = _animationPoints.takeLast();
//////		remove( animPoint->_controlPoint );
//////		delete animPoint->_controlPoint;
//////		delete animPoint;
//////	}
//	delete _debugMesh;
//	delete _debugLines;
//}
//PhSweep* CharCharacter::createSweep( PhSweepType* type ){
//	auto sweep = new PhSweep( type );
//	sweep->setManualUpdate();
//	add( sweep );
//	return sweep;
//}
////void CharCharacter::initEndPoint(CharCharacter::EndPoint& endpoint, CharEndPointType& endpointtype)
////{
////	endpoint.bone = getBoneForType( endpointtype.bone );
////	endpoint.current_sweep_down = new PhSweep( endpointtype.sweepType );
////	endpoint.current_sweep_down->setSweepDirection( RnVec3::Down );
////	endpoint.current_sweep_down->setFilter( &_notCharacterFilter );
////	endpoint.current_sweep_down->setAutoUpdate( false );
////	endpoint.curr_contact_count = 0;
////	endpoint.in_ground = 0;
////	endpoint.current_contact = false;
////	add( endpoint.current_sweep_down );

////}
//CharBone* CharCharacter::getBoneForType(CharBoneType* bone_type) const {
//	assert( bone_type );
//	assert( 0 <= bone_type->index_in_character() && bone_type->index_in_character() < _bones.size() );
//	CharBone* bone = _bones.at( bone_type->index_in_character() );
//	assert( bone->type() == bone_type );
//	return bone;
//}
//void CharCharacter::switchToCommandControl(){
//	_controlMode = CommandControlMode;
//}
////void CharCharacter::translate(const Vec3& positionOffset, bool moveChilds){
////	RnObject::translate( positionOffset, moveChilds );
////	if( moveChilds ){
////		_firstAnimate = true;
////		for( int t = 0; t < CharBoneCount; t++ ){
////			CharBone* bone = _bones.at( t );
////			bone->_prev_posori.translate( positionOffset );
////			bone->_next_posori.translate( positionOffset );
////		}
////	}
////}
//////void CharCharacter::init_side( CharCharacter::Side& side, const QString& side_link_name ){
//////	side._foot_link = getLinkForName( side_link_name + "foot" );
//////	side._foot_index = links().indexOf( side._foot_link );
//////	side._lowerleg_link = getLinkForName( side_link_name + "lowerleg" );
//////	side._lowerleg_index = links().indexOf( side._lowerleg_link );
//////	side._upperleg_link = getLinkForName( side_link_name + "upperleg" );
//////	side._upperleg_index = links().indexOf( side._upperleg_link );
//////	side._foot_down_sweep = new PhSweep( type()->_sweep_foot_down );
//////	side._foot_down_sweep->setFilter( &_notCharacterFilter );
//////	side._foot_down_sweep->setManualUpdate();
//////	side._foot_down_sweep ->setOrientation( RnOrientation( RnVec3::Down, RnVec3::Forward ) );
//////	add( side._foot_down_sweep  );
//////}
//////void CharCharacter::destroy_side(CharCharacter::Side& side){
//////	if( side._foot_down_sweep ){
//////		remove( side._foot_down_sweep );
//////		delete side._foot_down_sweep;
//////		side._foot_down_sweep = null;
//////	}

//////}
////////CharPoint *CharCharacter::getPointForName(const QString &name) const {
////////	foreach( auto point, _points ){
////////		if( point->link()->type()->name() == name )
////////			return point;
////////	}
////////	qDebug() << "CharCharacter::getPointForName not found" << name;
////////	ASSERT( false );
////////	throw Exception( "CharCharacter::getPointForName not found" );
////////}
////////CharPoint *CharCharacter::getPointForType(const CharPointType *type) const{
////////	assert( type );
////////	foreach( auto point, _points ){
////////		if( point->type() == type )
////////			return point;
////////	}
////////	qDebug() << "CharCharacter::getPointForType not found";
////////	ASSERT( false );
////////	throw Exception( "CharCharacter::getPointForType not found" );
////////}
//////void CharCharacter::switchToUnControl(){
//////	_controlMode = UnControlMode;
//////	setJointsStiffnessDamping( 0.0f, 100.0f );
//////	foreach( auto point, _control_points ){
//////		point->setEnabled( false );
//////	}
//////}
//////void CharCharacter::switchToPointControl(){
//////	_controlMode = PointControlMode;
//////	setJointsStiffnessDamping( 0.0f, 100.0f );
//////	foreach( PhArticulationJoint* joint, joints() ){
//////		joint->pxJoint()->setSwingLimitEnabled( false );
//////		joint->pxJoint()->setTwistLimitEnabled( false );
//////	}
//////	if( _control_points.isEmpty() ){
//////		for( int l = 0; l < links().size(); l++ ){
//////			PhArticulationLink* link = links().at( l );
//////			PhControlPointType* control_point_type = type()->_control_point_type_soft;
//////			if( link->type()->childJoints().isEmpty() )
//////				control_point_type = type()->_control_point_type_hard;
//////			PhControlPoint* control_point = new PhControlPoint( link, PosOri(), control_point_type );
//////			_control_points.append( control_point );
//////			add( control_point );
//////		}
//////	}
//////	foreach( auto point, _control_points ){
//////		point->setEnabled( true );
//////	}
//////}
//////void CharCharacter::loadPose( const CharPose& pose ){
//////	if( _pose == null ){
//////		_pose = pose.clone();
//////	} else {
//////		_pose->set( pose );
//////	}
//////}
//////void CharCharacter::loadAnimationStateToControlPoints( const CharAnimationState& state ){
//////	loadPose( * state._pose );
//////}
////////	for( int p = 0; p < _control_points.size(); p++ ){
////////		PhControlPoint* point = _control_points.at( p );
////////		point->setPositionOrientation( state._link_positions.at( p )->translated( position() ) );
////////	}
////////}
////////void CharCharacter::switchToPointControl(){
////////}
////////void CharCharacter::reposPointsByAttachment(){
////////	foreach( auto point, _points ){
////////		point->resetPosition();
////////	}
////////}
////////void CharCharacter::switchToRotationControl(){
////////	_controlMode = RotationControlMode;
////////	setJointsStiffnessDamping( 40000.0f, 10000.0f );
////////	foreach( auto point, _points ){
////////		point->setEnabled( false );
////////	}
////////}
////////void CharCharacter::switchToBehaviorControl(){
////////	_controlMode = BehaviorControlMode;
////////	setJointsStiffnessDamping( 0.0f, 1.0f );
////////	foreach( auto point, _points ){
////////		point->setEnabled( true );
////////	}
////////}
////////void CharCharacter::internalAddBehavior( CharBehavior* behavior ){
////////	ASSERT( behavior );
////////	_behaviors.append( behavior );
////////}
////////void CharCharacter::switchToBrainControl(){
////////	_controlMode = BrainControlMode;
////////	setJointsStiffnessDamping( 400000.0f, 200000.0f );
////////	foreach( auto point, _points ){
////////		point->setEnabled( false );
////////	}
////////	if( _brainControl == null ){
////////		_brainControl = new CharBrainControl( this, type()->brainControl() );
////////	}
////////}
////////void CharCharacter::addAnimationPoint(CharAnimPoint* animPoint){
////////	assert( animPoint );
////////	_animationPoints.append( animPoint );
////////	add( animPoint->_controlPoint );
////////}
////////void CharCharacter::switchToAnimationControl(){
////////	_controlMode = AnimationControlMode;
////////	setJointsStiffnessDamping( 0.0f, 100.0f );
////////	foreach( auto point, _points ){
////////		point->setEnabled( false );
////////	}
////////}
////////void CharCharacter::startAnimation( CharAnimTransition* animationTransition ){
////////	assert( animationTransition );
////////	assert( _controlMode == AnimationControlMode );
////////	_animationTransition = animationTransition;
////////	_timeInAnimTransition = 0;
////////}
////////void CharCharacter::loadStateToAnimPoints( const CharAnimState& state ){
////////	for( int p = 0; p < _animationPoints.size(); p++ ){
////////		auto cp = _animationPoints.at( p );
////////		Vec3 animPos = state._animPoint_positions.at( p );
////////		Vec3 globalPos = animPos + position();
////////		cp->_controlPoint->setPosition( globalPos + RnVec3::Up * 0.3f );
////////	}
////////}
////////void CharCharacter::animatePoints(float dt){
////////	if( _animationTransition == null )
////////		return;
////////	float preTime = _timeInAnimTransition;
////////	float pstTime = preTime + dt * 0.01f;
////////	if( _animationTransition->_duration < pstTime ){
////////		float restTime = _animationTransition->_duration - preTime;
////////		applyAnimationTransitionTransform( restTime );
////////		auto next_state = _animationTransition->_toState;
////////		if( next_state->_transitions.isEmpty() ){
////////			_animationTransition = null;
////////		} else {
////////			_animationTransition = next_state->_transitions.first();
////////		}
////////		_timeInAnimTransition = 0;
////////		animatePoints( dt - restTime );
////////		return;
////////	}
////////	applyAnimationTransitionTransform( dt );
////////	_timeInAnimTransition += dt;
////////	assert( 0 < _animationTransition->_duration );
////////	float f1 = ( _timeInAnimTransition / _animationTransition->_duration );
////////	float f0 = 1.0f - f1;
////////	//_poseCurrent.interpolate( _posePreTransition, ( _timeInCurrentTransition / _animTransition->duration() ), _animTransition->toState()->pose() );
////////	//_body->loadPose( _poseCurrent );
////////	for( int p = 0; p < _animationPoints.size(); p++ ){
////////		auto cp = _animationPoints.at( p );
////////		//Vec3 globalPos = animPos + animchar->_character->position();
////////		Vec3 p0 = _animationTransition->_fromState->_animPoint_positions.at( p );
////////		Vec3 p1 = _animationTransition->_toState->_animPoint_positions.at( p );
////////		Vec3 animPos = p0 * f0 + p1 * f1;
////////		Vec3 globalPos = animPos + position();
////////		cp->_controlPoint->setPosition( globalPos + RnVec3::Up * 0.3f );
////////	}
////////	PhArticulationLink* foot_l = getLinkForName( "leftfoot" );
////////	_sweep_foot_l_down->setPosition( foot_l->position() + RnVec3::Up * 5 );
////////	_sweep_foot_l_down->update();
////////	float error = _sweep_foot_l_down->minDistance() - 5.0f - foot_l->type()->startPosOri().position().z();
////////	if( error < 0 ){
////////	}
////////	if( _debugMesh ){
////////		_debugMesh->createCircleWithDirection(
////////					PosOri( foot_l->position(), orientation() )
////////					, 1.0f, 1.2f, QColor( 100, 100, 255 ) );
////////	}

////////}
////////void CharCharacter::applyAnimationTransitionTransform( float dt ){
////////	assert( _animationTransition );
////////	if( _animationTransition->_duration <= 0 )
////////		return;
////////	Vec3 new_position = position() + ( _animationTransition->_translation * dt ) / _animationTransition->_duration;
////////	setPosition( new_position );
////////}
////////void CharCharacter::updateAnimPoints(float dt){
////////	for( int p = 0; p < _animationPoints.size(); p++ ){
////////		auto cp = _animationPoints.at( p );
////////		cp->_current_position = cp->_link->positionOrientation().matrix().map( cp->_type->_posori_in_link.position() );
////////		Vec3 offset = cp->_current_position - cp->_controlPoint->position();
////////		if( 0 < cp->_time_until_breakable ){
////////			cp->_time_until_breakable -= dt;
////////		} else {
////////			if( offset.length() > 0.400045f ){
//////////				cp->_controlPoint->setEnabled( false );
////////			}
////////		}
////////	}

////////}
////////CharCharacterType *CharCharacter::type() const {
////////	return (CharCharacterType*)RnObject::type();
////////}
////////void CharCharacter::onAdded( RnScene* scene ){
////////	PhArticulation::onAdded( scene );
////////}
////////void CharCharacter::onRemoved( RnScene* scene ){
////////	PhArticulation::onRemoved( scene );
////////}
//void CharCharacter::animate( float t ){
//	RnObject::animate( t );

////	int todo;
////	CharAnimationSequence* as = CharAnimationSystem::Get()->_sequences.at( 0 );
////	static float at = 0;
////	at += t * 0.3f;
////	if( at > as->duration() )at = 0;
////	RnSteppedTimelineInterpolation<CharAnimationState> ai;
////	as->calcInterpolation( at, ai );
////	if( ai.hasStep0() && ai.hasStep1() ){
////		CharPose p;
////		p.interpolate( ai.step0()->_pose, ai.factor01(), ai.step1()->_pose );
////		loadPose( p );
////	}
////	{
////		_locomotion_time_factor += t / 1.92f;
////		while( _locomotion_time_factor > 1 )
////			_locomotion_time_factor -= 1;
////		_animationBlender.setAllTimeInSequenceByFactor( _locomotion_time_factor );

////		_animationBlender.setPower( 0, qMax( 0.0f, 1.0f - ( control.translation().length() + qAbs( control.rotation() ) )* 5 ) );
////		if( control.translation().y() < 0 ){
////			_animationBlender.setPower( 1, qMax( 0.0f, control.translation().y() ) );
////			_animationBlender.setPower( 7, 0 );
////		} else if( control.translation().y() < 0.5f ){
////			_animationBlender.setPower( 1, qMax( 0.0f, control.translation().y() * 2 ) );
////			_animationBlender.setPower( 7, 0 );
////		} else {
////			_animationBlender.setPower( 1, qMax( 0.0f, 1.0f - ( control.translation().y() - 0.5f )* 2 ) );
////			_animationBlender.setPower( 7, qMax( 0.0f, ( control.translation().y() -0.5f ) * 2 ) * 2 );
////		}
////		_animationBlender.setPower( 2, qMax( 0.0f, -control.translation().y() ) );
////		_animationBlender.setPower( 3, qMax( 0.0f, -control.translation().x() ) );
////		_animationBlender.setPower( 4, qMax( 0.0f, control.translation().x() ) );
////		_animationBlender.setPower( 5, qMax( 0.0f, control.rotation() ) );
////		_animationBlender.setPower( 6, qMax( 0.0f, -control.rotation() ) );
////	}
//	{
//		CharPose p;
//		_animationBlender.calcPose( p );
//		loadPose( p );
//	}
//	{

//	}
//	if( _controlMode == UnhandledControlMode ){
//	} else if( _controlMode == CommandControlMode ){
//		_commander.run();
//		animateControl( t );
//	} else if( _controlMode == ControlControlMode ){
//		animateControl( t );
////	} else if( _controlMode == AnimationControlMode ){
////		animateByAnimation( t );
//	} else {
//		assert( false );
//	}
//////	static float r = 0;
//////	static float rd = 1;
//////	r += rd;
////	//_bones.at( 0 )->_rotation = glm::quat::fromAxisAndAngle( RnVec3::Forward, r * 0.15f );
////	//_bones.at( 1 )->_rotation = glm::quat::fromAxisAndAngle( RnVec3::Forward, r * -0.15f );
//////	_bones.at( 2 )->_rotation = glm::quat::fromAxisAndAngle( RnVec3::Forward, r * 0.15f );
//////	_bones.at( 3 )->_rotation = glm::quat::fromAxisAndAngle( RnVec3::Forward, r * 0.15f );
//////	_bones.at( 4 )->_rotation = glm::quat::fromAxisAndAngle( RnVec3::Forward, r * 0.15f );
//////	_bones.at( 5 )->_rotation = glm::quat::fromAxisAndAngle( RnVec3::Up, r * 0.15f );
//////	_bones.at( 6 )->_rotation = glm::quat::fromAxisAndAngle( RnVec3::Forward, -r );
////	//Mat4 parent_bone_t;

//////	PhArticulation::animate( t );
////////	if( _controlMode != AnimationControlMode ){
////////		const float speedY = 15.0f;
////////		const float speedX = 7.0f;
////////		const float speedR = 45.0f;
////////		_targetPosOri.translate( _targetPosOri.direction() * control.moveY() * t * speedY
////////								 + _targetPosOri.right() * control.moveX() * t * speedX );
////////		_targetPosOri.setOrientation( _targetPosOri.orientation().rotated( control.rotation() * speedR * t, RnVec3::Up ) );
////////	}
////////	if( _controlMode == BehaviorControlMode ){
////////		foreach( auto behavior, _behaviors ){
////////			behavior->setTargets();
////////		}
////////		foreach( auto point, _points ){
////////			point->animateTarget( t );
////////		}
////////	}
////////	if( _controlMode == BrainControlMode ){
////////		_brainControl->animate( t );
////////	}
////////	if( _controlMode == AnimationControlMode ){
////////		updateAnimPoints( t );
////////		animatePoints( t );
////////	}
//////	if( _controlMode == PointControlMode ){
//////		if( _pose ){
//////			if( _temp_pose == null )_temp_pose = _pose->clone();
//////			Mat4 m = positionOrientation().matrix();
//////			for( int l = 0; l < links().size(); l++ ){
//////				PosOri link_posori = PosOri::map( m, _pose->getLinkPosition( l ) );
//////				_temp_pose->setLinkPosition( l, link_posori );
//////			}
//////			animate_side_1( _l );
//////			animate_side_1( _r );
//////			Vec3 pos = position();
//////			pos.setZ( qMin( _l._ground_z, _r._ground_z ) );
//////			setPosition( pos );
//////			animate_side_2( _l );
//////			animate_side_2( _r );
//////			for( int l = 0; l < links().size(); l++ ){
//////				PhControlPoint* point = _control_points.at( l );
//////				point->setPositionOrientation( _temp_pose->getLinkPosition( l ).translated( RnVec3::Up * 0 ) );
//////			}
//////		}
//	//	//	}
//}
//void CharCharacter::animateControl( float t ){
//	if( _debugLines )_debugLines->clear();
//	//if( _debugLines )_debugLines->createBox( positionOrientation().translated( RnVec3::Up * 7.1f ), Vec3( 2, 2, 7 ), QColor( 100, 100, 255 ) );

//	control.tilt_y += control.rotation() * 1.5f * t;
//	control.tilt_y *= 1.0f - ( t * 4.0f );
//	if( control.tilt_y < -1 )control.tilt_y = -1;
//	if( control.tilt_y > 1 )control.tilt_y = 1;

//	Vec3 up = RnVec3::Up;
//	if( qAbs( control.tilt_y ) > 0.01f ){
//		Mat4 m;
//		m.rotate( control.tilt_y * -40.5f, orientation().direction() );
//		up = m.mapVector( RnVec3::Up );
//	}


//	Vec3 position = this->position();
//	if( control.translation().y() > 0 ){
//		control.velocity += orientation().direction() * control.translation().y() * 15.38f * t * type()->scale() * 3;
//	} else {
//		control.velocity += orientation().direction() * control.translation().y() * 12.26f * t * type()->scale();
//	}
//	control.velocity += orientation().right() * control.translation().x() * 12.26f * t * type()->scale();

//	//qDebug() << "control.prepare_jump" << control.prepare_jump << "control.jump_power" << control.jump_power;
//	if( control.prepare_jump ){
//		control.jump_power += t * 0.5f;
//		if( control.jump_power > 1 )control.jump_power = 1;
//	} else {
//		if( 0 < control.jump_power ){
//			control.velocity += RnVec3::Up * control.jump_power * 40;
//			control.jump_power = 0;
//		}
//	}
//	//control.velocity += scene()->asPhScene()->gravity() * t;
//	//control.velocity += RnVec3::Down * 5.0f * t;
//	if( _debugLines )_debugLines->createSphere( position + up * 2.3f, 2.0f, 5, QColor( 100, 100, 200 ) );
//	if( _debugLines )_debugLines->createSphere( position + up * 7.6f, 2.0f, 5, QColor( 100, 100, 200 ) );
//	if( _debugLines )_debugLines->createSphere( position + up * ( 7.6f + 6.3f * ( 1.0f - control.jump_power )), 2.0f, 5, QColor( 100, 100, 200 ) );
//	//qDebug() << "---";
//	if( control.velocity.length() > 0 ){
//		int rc = 0;
//		while( true ){
//			if( _debugLines )_debugLines->createRay( position + RnVec3::Up * 4.1f, control.velocity * 10, QColor( 255, 255, 0, 100 ) );
//			Vec3 velocity_dir = control.velocity.normalized();
//			_sweep->update( position + RnVec3::Up * 2.3f + velocity_dir * 0.0f, velocity_dir );
////			qDebug() << " _sweep->distance()" <<  _sweep->distance();
//			if( _sweep->distance() < control.velocity.length() * t + 0.1f ){
//				Vec3 velocity_right = Vec3::crossProduct( RnVec3::Up, velocity_dir );
////				qDebugV3( "velocity", control.velocity );
//				//if( _debugLines )_debugLines->createSphere( PosOri( _sweep->positionAtHit(), RnOrientation() ), 2.0f, 5, QColor( 255, 0, 0 ) );
////				qDebugV3( "hit_norm", _sweep->hitNormal() );
////				qDebug() << "d" << Vec3::dotProduct( velocity_dir, _sweep->hitNormal() );
////				qDebug() << "du" << Vec3::dotProduct( _sweep->hitNormal(), RnVec3::Up );
//				if( _debugLines )_debugLines->createSphere( _sweep->positionAtHit(), 2.0f, 5, QColor( 255, 100, 100, 10 ) );
//				if( _debugLines )_debugLines->createRay( _sweep->hitPoint(), _sweep->hitNormal(), 5, QColor( 255, 100, 100 ) );
//				//_debugLines->createRay( position + RnVec3::Up * 4.4f , _sweep->hitNormal(), 3, QColor( 255, 0, 0 ) );

//				//_debugLines->createLine( _sweep->position(), _sweep->positionAtHit(), QColor( 0, 0, 0 ) );
//				//.createLine( p2qVector( hit.position ), p2qVector( hit.position ) + p2qVector( hit.normal ), QColor( 255, 255, 0 ) );

//				if( qAbs( Vec3::dotProduct( RnVec3::Up, _sweep->hitNormal() ) ) < 0.3f ){
//					// scheint eine Wand zu sein
//					qDebug() << "time to wall-run?" << control.time_in_air;

//					//qDebug() << "wand" << "dot-vel-hitnorm" << Vec3::dotProduct( velocity_dir, _sweep->hitNormal() );
//					if( qAbs( Vec3::dotProduct( velocity_dir, _sweep->hitNormal() ) ) < 0.5f ){
//						// an der Wand entlang slide
//						///qDebug() << "slide";
//						Mat4 m;
//						float grad = 3;
//						if( Vec3::dotProduct( velocity_right, _sweep->hitNormal() ) < 0.0f )
//							grad *= -1;
//						m.rotate( grad, RnVec3::Up );
//						control.velocity = m.mapVector( velocity_dir ) * control.velocity.length() * 0.97f;
//					} else {
//						// uff,
//						control.velocity *= 0.0f;
//					}
//				} else {
//					// keine wand?
//					Mat4 m;
//					m.rotate( -3, velocity_right );
//					control.velocity = m.mapVector( velocity_dir ) * control.velocity.length() * 0.97f;
//				}




////				float dot_vel_hitnormal = Vec3::dotProduct( control.velocity_dir, _sweep->hitNormal() );
////				qDebug() << "v:" << Vec32string( velocity ) << "vd:" << Vec32string( velocity_dir ) << " hn:" << Vec32string( _sweep->hitNormal() ) << "hn-l: " << _sweep->hitNormal().length() << " vhnd:" << dot_vel_hitnormal;


//				//if( true || Vec3::dotProduct( _sweep->hitNormal(), RnVec3::Up ) > 0.5f )
//				{
////					rc ++;
////					if( rc > 2 )velocity *= 0;
////					velocity *= 0.98f;
////					velocity += RnVec3::Up * 0.02f;
////				} else {
////					velocity = RnVec3::Null;
//				}
////				if( Vec3::dotProduct( velocity_dir, _sweep->hitNormal() ) < -0.5f ){
////					velocity = RnVec3::Null;
////					break;
////				}
//				//if( Vec3( velocity.x(), velocity.y(), 0 ).length() < 0.001 ){
//				if( control.velocity.length() < 0.001 ){
//					control.velocity = RnVec3::Null;
//					break;
//				}
//			} else {
//				break;
//			}
//		}
//	}
//	position += control.velocity * t;
//	_sweep->update( position + RnVec3::Up * 3.0f, RnVec3::Down );
//	float height_from_ground = ( _sweep->distance() - 3.0f  + 2.0f );
//	if( -0.0001 < height_from_ground && height_from_ground < 0.001 ){
//	} else {
//		//qDebug() << "height_from_ground" << height_from_ground;
//	}
//	/*if( height_from_ground <= -4.0f ){
//	} else */
//	if( height_from_ground <= 0.00f ){
//		control.time_in_air = 0;
//	//		position += RnVec3::Down * height_from_ground;
//	} else {
//		float grav = 9.0f * t;
//		if( height_from_ground < grav )
//			grav = height_from_ground;
//		position += RnVec3::Down * grav;
//		control.velocity += RnVec3::Down * grav * t;
//		control.time_in_air += t;
//	}
//	control.velocity *= ( 1.0f - 1.50f * t );
//	setPosition( position );


//	//position += RnVec3::Down * 48.0f * t;
////	_sweeps.lower_down->update( position + RnVec3::Up * 8.0f, RnVec3::Down );
////	//position.setZ( position.z() - 3 + _sweeps.lower_down->distance() );
//////	if( 3.3f < _sweeps.lower_down->distance() ){
//////		position += RnVec3::Down * 98.8f * t;
//////	} else if( 3.0f < _sweeps.lower_down->distance() ){
//////		position.setZ( )
//////	} else {
//////		position += RnVec3::Up * ( 3.0f - _sweeps.lower_down->distance() );
//////	}
////	Vec3 velocity = orientation().direction() * control.moveY * 20.6f * t
////						 + orientation().right() * control.moveX * 7.6f * t;
////	if( velocity.length() > 0 ){
////		Vec3 velocity_dir = velocity.normalized();
////		_sweep->update( position + RnVec3::Up * 4 + velocity_dir * 0.4f, velocity.normalized() );
////		qDebug() << " _sweep->distance()" <<  _sweep->distance();
////		if( _sweep->distance() + velocity.length() < 0.51f ){
////			qDebug() << "block" << _sweep->distance();
////		} else {
////			position += velocity;
////		}
////	}

//	//translate( translation, truqe );
////	control.moveY *= qMax( 1.0f - t * 10.93f, 0.0f );
////	control.moveX *= qMax( 1.0f - t * 10.93f, 0.0f );
//	setOrientation( orientation().rotated( control.rotation() * 90.0f * t, RnVec3::Up ) );
////	control.setRotation( control.rotation() * qMax( 1.0f - t * 10.93f, 0.0f ) );


//}
////void CharCharacter::setAction( CharAnimationAction* action ){
////	assert( action );
////	_action = action;
////	if( _transition == null ){
////		foreach( CharAnimationTransition* transition, CharAnimationSystem::Get()->_transitions ){
////			if( transition->_action == _action ){
////				_transition = transition;
////				_time_in_transition = 0;
////				return;
////			}
////		}
////	}
////}
////float total_time_in_transition = 0;
////void CharCharacter::animateByAnimation(float t){
////	//t *= 0.4f;
////	if( _transition ){
////		while( 0 < t ){
////			if( _transition->_duration < _time_in_transition + t ){
////				float rest_time = _transition->_duration - _time_in_transition;
////				applyAnimationTransitionTransform( rest_time );
////				//assert( qAbs( total_time_in_transition - _transition->_duration ) < 0.001f );
////				total_time_in_transition = 0;
////				//setPosition( position() + _transition->_translation );
////				CharAnimationState* state = _transition->_to_state;
////				_prev_pose = state->_pose;
////				CharAnimationTransition* prev_trans = _transition;
////				_transition = state->findOutTransition( _action );
////				if( _transition == null ){
////					qDebug() << "no transition to" << _action->name();
////					_transition = state->getOutTransition( prev_trans->_action );
////				}
////				qDebug() << "transition to" << _transition->_to_state->_name << " in " << _action->name();
////				assert( 0 < _transition->_duration );
////				_time_in_transition = 0;
////				t -= rest_time;
////			} else {
////				applyAnimationTransitionTransform( t );
////				_time_in_transition += t;
////				t = 0;
////			}
////		}
////		_current_pose.interpolate( _prev_pose, 1.0f - ( _time_in_transition / _transition->_duration ), _transition->_to_state->_pose );
////		_current_pose = _prev_pose;
////	}
////	loadPose( _current_pose );
//////	_translation = RnVec3::Null;
//////	_force = RnVec3::Null;
//////	animateEndPoint( _leftFoot );
//////	animateEndPoint( _rightFoot );
//////	if( _leftFoot.current_contact && _rightFoot.current_contact ){
//////		_force /= 2;
//////		_translation /= 2;
//////		_velocity = RnVec3::Null;
//////	} else if( _leftFoot.current_contact || _rightFoot.current_contact ){
//////		_velocity = RnVec3::Null;
//////	} else {
//////		_force += RnVec3::Down * 19.8f;
//////	}
//////	_velocity += _force * t;
//////	if( _firstAnimate ){
//////		_firstAnimate = false;
//////		_velocity = RnVec3::Null;
//////		_translation = RnVec3::Null;
//////	} else {
//////		_translation += _velocity * t;
//////	}
//////	if( _velocity.length() > 3 )
//////		_velocity = _velocity.normalized() * 3;
//////	_velocity *= 0.99f;
////////	this->translate( _translation, false );
////	loadBonesToSkin();
////	foreach( CharBone* bone, _bones ){
////////		bone->_next_posori.translate( _translation );
////		bone->_prev_posori = bone->_next_posori;
////////		bone->_link->pxLink()->setLinearVelocity( bone->_link->pxLink()->getLinearVelocity() * 0.9f );
////////		bone->_link->pxLink()->setAngularVelocity( bone->_link->pxLink()->getAngularVelocity() * 0.9f );
////	}
////}
//void CharCharacter::loadPose(const CharPose& pose){
//	{
//		for( int t = 0; t < CharBoneCount; t++ ){
//			CharBone* bone = _bones.at( t );
//			bone->setRotation( pose.rotation( t ) );
//			if( bone->hasParent() ){
//				auto j = _articulation->getJointForType( bone->_link->type()->parentJoint() );
//				auto pxj = j->pxJoint();
//				glm::quat rot = pose.rotation( t );
//				PxQuat pxrot(  rot.x(), rot.y(), rot.z(), rot.scalar() );
//				pxj->setTargetOrientation( pxrot );
//				pxj->setDriveType( PxArticulationJointDriveType::eTARGET );
//			}
//		}
//	}
//	foreach( CharBone* bone, _bones ){
//		if( bone->isRoot() ){
//			//parent_bone_t = positionOrientation().matrix();
//			bone->_parent_matrix = positionOrientation().matrix();
//			//qDebugV3( "pos", position() );
//			bone->_parent_matrix.translate( RnVec3::Down * bone->type()->joint_T_posori_in_skin().position().z() );
//			bone->_parent_matrix.translate( pose.rootPosition() * type()->scale() );
//		} else {
//			assert( _bones.indexOf( bone->parent() ) < _bones.indexOf( bone ) );
//			bone->_parent_matrix = bone->parent()->_absolute_matrix;
//		}
////		Mat4 client_bone_t = bone->type()->joint_T_posori_in_skin().matrix();
////		Mat4 parent_bone_to_client_bone = parent_bone_t.inverted() * client_bone_t;
////		Mat4 client_bone_t_calced_from_parent_bone_t = parent_bone_t * parent_bone_to_client_bone;
////		Mat4 m = client_bone_t_calced_from_parent_bone_t;
//		//Mat4 m = parent_bone_t * bone->type()->joint_T_posori_in_parent().matrix();
////		m.translate( bone->type()->joint_T_posori_in_parent().position() );
////		m.rotate( bone->type()->joint_T_posori_in_parent().orientation().quaternion() );
//		//m.rotate( bone->_rotation );
////		//m = bone->type()->joint_T_posori_in_skin().matrix();
//		//bone->_relative_matrix = bone->_type_relative_matrix * bone->_this_relative_matrix;
//		//bone->_relative_matrix = bone->_type_relative_matrix;
//		bone->_relative_matrix = bone->_type->_relative_T_matrix;
//		bone->_relative_matrix.rotate( bone->_rotation );
//		bone->_absolute_matrix = bone->_parent_matrix * bone->_relative_matrix;
////bone->_absolute_matrix = bone->_link->positionOrientation().matrix();
//		bone->_next_posori = bone->_absolute_matrix;
//		if( bone->_link ){
//			bone->_link->setPositionOrientation( bone->_absolute_matrix );
//		}
//	}
//}
//void CharCharacter::loadBonesToSkin(){
//	foreach( CharBone* bone, _bones ){
//		bone->_absolute_matrix = bone->_next_posori.matrix();
//		bone->_skin_matrix = bone->_absolute_matrix * bone->type()->skinInvMatrix();
//		foreach( auto sm, bone->_skin_matrixes ){
//			(*sm) = bone->_skin_matrix;
//		}
//	}
//}
////void CharCharacter::applyAnimationTransitionTransform(float t){
////	if( _transition ){
////		total_time_in_transition += t;
////		Vec3 o =  _transition->_translation * t * type()->scale();
////		//qDebugV3( "o", o );
////		//translate( o, false );
////	}
////}

////void CharCharacter::animateEndPoint(CharCharacter::EndPoint& endpoint)
////{
////	endpoint.current_posori = endpoint.bone->_next_posori;
////	endpoint.current_sweep_down->setPositionOrientation( endpoint.current_posori.translated( RnVec3::Up * 3 ) );
////	endpoint.current_sweep_down->update();
////	endpoint.in_ground = 3.0f - endpoint.current_sweep_down->distance();
////	if( qAbs( endpoint.bone->_next_posori.direction().z() ) > 0.4f ){
////		endpoint.in_ground -= 0.9f;
////	}
////	static const float threshold = 0.1f;
////	if( endpoint.in_ground < 0.0f ){
////		endpoint.current_contact = false;
////	} else if( endpoint.in_ground < threshold ){
////		endpoint.current_contact = true;
////		_translation += RnVec3::Down * ( threshold - endpoint.in_ground );
////	} else {
////		endpoint.current_contact = true;
////		_translation += RnVec3::Up * ( endpoint.in_ground - threshold );
////	}
////	endpoint.curr_contact_count -= 0.3f;
////	if( endpoint.current_contact ){
////		endpoint.curr_contact_count += 0.5f;
////	} else {
////		endpoint.curr_contact_count -= 0.23f;
////	}
////	if( endpoint.curr_contact_count < 0 ) endpoint.curr_contact_count = 0;
////	if( endpoint.curr_contact_count > 1 ) endpoint.curr_contact_count = 1;
////	endpoint.current_contact = endpoint.curr_contact_count > 0.4f;
////	if( endpoint.current_contact ){
////		_translation -= endpoint.bone->_next_posori.position() - endpoint.bone->_prev_posori.position();
////	}
////}
//////void CharCharacter::animate_side_1( CharCharacter::Side& side ){
//////	PosOri foot_l_posori = _temp_pose->getLinkPosition( side._foot_index );
//////	side._foot_down_sweep->setPosition( foot_l_posori.position() + RnVec3::Up * 5 );
//////	side._foot_down_sweep->update();
//////	side._ground_z = side._foot_down_sweep->position().z() - side._foot_down_sweep->distance();
//////}
//////void CharCharacter::animate_side_2(CharCharacter::Side& side){
//////	float soll_z = side._ground_z + _pose->getLinkPosition( side._foot_index ).position().z();
//////	float ist_z = _temp_pose->getLinkPosition( side._foot_index ).position().z();
//////	float offset_z = soll_z - ist_z + 0.05f;
//////	//offset_z *= 0.000001f;
//////	//if( offset_z > 0 )
//////	{
//////		_temp_pose->translateLink( side._foot_index, RnVec3::Up * offset_z * 1.01f );
//////		_temp_pose->translateLink( side._lowerleg_index, RnVec3::Up * offset_z * 0.8f + orientation().direction() * offset_z );
//////		_temp_pose->translateLink( side._upperleg_index, RnVec3::Up * offset_z * 0.6f + orientation().direction() * offset_z );
//////		Vec3 hit_normal = side._foot_down_sweep->hitNormal();
//////		if( &side == &_r ){
//////			float dot_right = Vec3::dotProduct( orientation().right(), hit_normal );
//////			if( dot_right > 0.05f ){
//////				float r = ( dot_right - 0.05f ) * -90 * 2;
//////				if( r < -80.0f )r = -80;
//////				//qDebug() << r;
//////				RnOrientation foot_orientation = _temp_pose->getLinkPosition( side._foot_index ).orientation();
//////				foot_orientation.rotate( r, RnVec3::Up );
//////				_temp_pose->setLinkOrientation( side._foot_index, foot_orientation );
//////			} else {
//////				//_temp_pose->translateLink( side._foot_index, RnVec3::Up * 1.01f );
//////			}
//////		}
//////		if( &side == &_l ){
//////			float dot_right = Vec3::dotProduct( -orientation().right(), hit_normal );
//////			if( dot_right > 0.05f ){
//////				float r = ( dot_right - 0.05f ) * 90 * 2;
//////				if( r < -80.0f )r = -80;
//////				//qDebug() << r;
//////				RnOrientation foot_orientation = _temp_pose->getLinkPosition( side._foot_index ).orientation();
//////				foot_orientation.rotate( r, RnVec3::Up );
//////				_temp_pose->setLinkOrientation( side._foot_index, foot_orientation );
//////			} else {
//////				//_temp_pose->translateLink( side._foot_index, RnVec3::Up * 1.01f );
//////			}
//////		}

//////		/*
//////		Vec3 foot_dir = _temp_pose->getLinkPosition( side._foot_index ).direction();
//////		if( qAbs( Vec3::dotProduct( hit_normal, foot_dir ) ) < 0.8f ){
////////			hit_normal = RnVec3::Up;
//////			Vec3 foot_right  = Vec3::crossProduct( foot_dir, hit_normal );
//////			Vec3 foot_up = -Vec3::crossProduct( foot_dir, foot_right );
//////			foot_dir = Vec3::crossProduct( foot_up, foot_right );
////////			_temp_pose->setLinkOrientation( side._foot_index, RnOrientation( foot_dir, foot_right, foot_up  ) );
//////		}

//////	//} else {
//////		//qDebug() << "CharChacter animate_side_2 invalid_z" << offset_z;
//////		*/
//////	}

//////}

////////	Vec3 soll_position = foot_l_posori.position() + RnVec3::Up * 5 + RnVec3::Down * ( _sweep_foot_l_down->minDistance() );
////////	//if( soll_position.z() < foot_l_posori.position().z() )
////////	{
////////		foot_l_posori.setPosition( Vec3( foot_l_posori.position().x(), foot_l_posori.position().y(), soll_position.z() ) );
////////		_temp_pose->setLinkPosition( foot_l_index, foot_l_posori );
////////	}

//void CharCharacter::render( RnRenderer& renderer ){
//	RnObject::render( renderer );
////	//PhArticulation::render( renderer );
//	if( _renderSkin ){
//		if( !QApplication::queryKeyboardModifiers().testFlag( Qt::AltModifier ) ){
//			Mat4 sm = positionOrientation().matrix();
//			if( qAbs( control.tilt_y ) > 0.01f ){
//				Mat4 md, mr, mu;
//				md.translate( RnVec3::Down * -10 );
//				mr.rotate( control.tilt_y * -40.5f, RnVec3::Forward );
//				mu.translate( RnVec3::Down * 10 );
//				sm = sm * md * mr * mu;
//			}
//			for( int m = 0; m < RnSkin::Max_Matrix_Count; m++ ){
//				_skinMatrices[ m ] = sm;
//			}
//			loadBonesToSkin();
//			type()->skin()->render( renderer, _skinMatrices, 0, _skinPartVariants );
//		}
//	}
//	if( _renderDebug ){
//		if( renderer.pass() == RnRenderPass_Forward ){
//			if( _debugLines == null )
//				_debugLines = new RnLines( 100000 );
////			_debugLines->clear();
//////			foreach( CharBone* bone, _bones ){
//////				if( bone->isRoot() ){
//////					_debugLines->createLine( position(), bone->_global_joint_posori.position(), QColor( 0, 0, 150 ) );
//////				} else {
//////					_debugLines->createLine( bone->_global_joint_posori.position(), bone->parent()->_global_joint_posori.position(), QColor( 150, 150, 150 ) );
//////				}
//////			}
//			if( !_debugLines->isEmpty() ){
//				_debugLines->render( renderer );
//			}
//			if( _debugMesh == null )
//				_debugMesh = new RnMeshBuffer<RnVertexPositionColor>( GL_TRIANGLES, 100000, 500000 );
//			_debugMesh->clear();
////////			//if( !_debugMesh->hasSpaceFor( 5000 ) )_debugMesh->clear();
//			_debugMesh->createCircleWithDirection( positionOrientation().translated( RnVec3::Up * 0.1f ), 2.0f, 2.5f, QColor( 100, 100, 255 ) );
////			if( _leftFoot.current_contact ){
////				_debugMesh->createCircleWithDirection( _leftFoot.current_posori, 1.5f, 1.95f, QColor( 100, 100, 255 ) );
////			}
////			if( _rightFoot.current_contact ){
////				_debugMesh->createCircleWithDirection( _rightFoot.current_posori, 1.5f, 1.95f, QColor( 100, 100, 255 ) );
////			}

////////			//_debugMesh->createCircleWithDirection( targetPosOri().translated( RnVec3::Up * 0.1f ), 2.0f, 2.5f, QColor( 100, 100, 255 ) );
//////////			Vec3 com = calcCenterOfMass();
//////////			float comBoxLength = boundingBox().size().x() * 0.3f;
//////////			float comBoxRadius = boundingBox().size().x() * 0.03f;
//////////			_debugMesh->createLine(
//////////				com - RnVec3::Forward * comBoxLength,
//////////				com + RnVec3::Forward * comBoxLength,
//////////				comBoxRadius, QColor( 100, 255, 100 ) );
////////			if( _controlMode == BehaviorControlMode ){
////////				foreach( auto behavior, _behaviors ){
////////					behavior->renderDebug( _debugMesh );
////////				}
////////			}
//			if( !_debugMesh->isEmpty() ){
//				RnRenderState state;
//				//state.setBlendMode( RnBlendMode_SrcAlpha_OneMinusSrcAlpha );
//				state.setCullFaceMode( RnCullFace_Disabled );
//				state.setShader( RnShaders::Get()->get( "RnPositionColor", "FORWARD" ) );
//				renderer.setState( state );
//				_debugMesh->render( renderer );
//			}
//		}
//	}
//}

//CharBone::CharBone(CharBone* parent, CharBoneType* type)
//	:_type( type )
//	,_parent( parent )
//	,_link( null )
//{
//}
//CharBone::~CharBone(){
//}
//void CharBone::setRotation(const glm::quat& rotation){
//	_rotation = rotation;
//}



//CharCharacter::Control::Control()
//	:prepare_jump( false )
//	,jump_power( 0 )
//	,tilt_y( 0 )
//	,time_in_air( 0 )
//{
//}

