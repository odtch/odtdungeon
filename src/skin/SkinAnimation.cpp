#include "stdodt.h"
#include "SkinAnimation.h"
//#include "Skin.h"

//SkinPose::SkinPose()
//	:_rotationCount( 0 )
//	,_rotations( null )
//{
//	//std::cout << "SkinPose()\n";
//}
//SkinPose::SkinPose( uint rotationCount )
//	:_rotationCount( 0 )
//	,_rotations( null )
//{
//	//std::cout << "SkinPose( " << rotationCount << " )\n";
//	reserve( rotationCount );
//}
//SkinPose::~SkinPose(){
//	//std::cout << "~SkinPose()" << _rotationCount << "\n";
//	if( _rotations ){
//		delete _rotations;
//		_rotations = null;
//	}
//	_rotationCount = 0;
//}
//void SkinPose::setRotation( uint channel, const glm::quat& rotation ){
//	assert( channel < _rotationCount );
//	assert( _rotations );
//	_rotations[ channel ] = rotation;
//}
//void SkinPose::operator=( const SkinPose& src ){
//	if( _rotationCount == src._rotationCount ){
//	} else if( _rotationCount == 0 ){
//		reserve( src._rotationCount );
//	} else {
//		assert( false );
//	}
//	for( uint c = 0; c < _rotationCount; c++ ){
//		_rotations[ c ] = src._rotations[ c ];
//	}
//}
//void SkinPose::reserve( uint rotationCount ){
//	//std::cout << "SkinPose::reserve( rotationCount )\n";
//	assert( 0 < rotationCount );
//	assert( _rotationCount == 0 );
//	assert( _rotations == null );
//	_rotations = new glm::quat[ rotationCount ];
//	assert( _rotations );
//	_rotationCount = rotationCount;
//	for( uint c = 0; c < _rotationCount; c++ ){
//		_rotations[ c ] = glm::quat( 1.0f, 0.0f, 0.0f, 0.0f );
//	}
//}


//SkinAnimationChannel::SkinAnimationChannel( uint index, const String& name )
//	:_index( index )
//	,_name( name )
//{
//}
//SkinAnimationChannel::~SkinAnimationChannel(){
//}


//SkinAnimationFrame::SkinAnimationFrame( float time )
//	:TimelineEntry( time )
//	,_skeleton( null )
//{
//	assert( 0.0f <= time );
//}
//SkinAnimationFrame::~SkinAnimationFrame(){
//	if( _skeleton ){
//		delete _skeleton;
//		_skeleton = null;
//	}
//}


//SkinAnimation::SkinAnimation( SkinSkeleton* skeleton )
//	:_skeleton( skeleton )
//{
//	assert( _skeleton );
//}
//SkinAnimation::~SkinAnimation(){
//	_timeline.clear();
//	_channels.deleteAll();
//	delete _skeleton;
//}
//const SkinAnimationFrame* SkinAnimation::getFrame( float time ) const {
//	const SkinAnimationFrame* result = _timeline.entries().first();
//	for( SkinAnimationFrame* frame : _timeline.entries() ){
//		if( frame->time() < time ){
//			result = frame;
//		} else {
//			return result;
//		}
//	}
//	return result;
//}
////void SkinAnimation::getPoseAt( float time, SkinPose& pose ) const {
////	pose = result->pose();
////}
//void SkinAnimation::createFrameIfNeeded( float time ){
//	SkinAnimationFrame* frame = _timeline.findAt( time );
//	if( frame )
//		return;
//	_timeline.insert( new SkinAnimationFrame( time ) );
//}


///*
//#include "stdodt.h"
//#include "RnSkeletonAnimation.h"
//#include "RnSkeleton.h"
//#include "RnSkeletonJoint.h"

//RnSkeletonAnimation::RnSkeletonAnimation( const QString& name, float duration, RnSkeleton* skeleton )
//	:_name( name )
//	,_duration( duration )
//	,_skeleton( skeleton )
//	,_tmpskeleton( null )
//{
//	ASSERT( !_name.isEmpty() );
//	ASSERT( 0.0f <= _duration );
//	ASSERT( _skeleton );
//	_timeline.insert( new Step( 0.0f, _skeleton->clone() ) );
//}
//RnSkeletonAnimation::~RnSkeletonAnimation(){
//	_timeline.clear();
//	delete _skeleton;
//	if( _tmpskeleton )
//		delete _tmpskeleton;
//}
//void RnSkeletonAnimation::createStep( float time, RnSkeleton *skeleton ){
//	ASSERT( duration() < time );
//	_duration = time;
//	ASSERT( skeleton );
//	_timeline.insert( new Step( time, skeleton ) );
//}
//void RnSkeletonAnimation::mapToSameSkeletonType(float time, RnSkeleton &target) const {
//	ASSERT( _skeleton->joints().size() == target.joints().size() );
//	RnSteppedTimelineInterpolation<Step> interpolation;
//	_timeline.calcInterpolation( time, interpolation );
//	target.rootJoint()->recalcRelativeMatrix();
//	for( int j = 0; j < target.joints().size(); j++ ){
//		RnSkeletonJoint* targetJoint = target.joints().at( j );
//		RnSkeletonJoint* sourceJoint0 = null;
//		RnSkeletonJoint* sourceJoint1 = null;
//		if( interpolation.hasStep0() ){
//			sourceJoint0 = interpolation.step0()->_skeleton->joints().at( j );
//			ASSERT( sourceJoint0->name() == targetJoint->name() );
//		}
//		if( interpolation.hasStep1() ){
//			sourceJoint1 = interpolation.step1()->_skeleton->joints().at( j );
//			ASSERT( sourceJoint1->name() == targetJoint->name() );
//		}
//		QMatrix4x4 m;
//		if( interpolation.hasStep0and1() ){
//			m = ( sourceJoint0->relativeMatrix() * interpolation.factor01() )
//				+ ( sourceJoint1->relativeMatrix() * interpolation.factor10() );
//		} else if( sourceJoint0 ){
//			m = sourceJoint0->relativeMatrix();
//		} else if( sourceJoint1 ){
//			m = sourceJoint1->relativeMatrix();
//		} else {
//			ASSERT( false );
//		}
//		targetJoint->setRelativeMatrix( m );
//	}
//	target.update();
//}
//void RnSkeletonAnimation::mapToSkinSkeleton(float time, RnSkeleton &target) const{
//	if( _tmpskeleton == null )
//		_tmpskeleton = _skeleton->clone();
//	mapToSameSkeletonType( time, *_tmpskeleton );
//	mapAnimToSkinSkeleton( *_tmpskeleton, target );
//}
//void RnSkeletonAnimation::mapAnimToSkinSkeleton( const RnSkeleton &animSkeleton, RnSkeleton &target ){
//	target.rootJoint()->recalcRelativeMatrix();
//	for( int j = 0; j < target.joints().size(); j++ ){
//		RnSkeletonJoint* targetJoint = target.joints().at( j );
//		RnSkeletonJoint* animJoint = animSkeleton.findJointByName( targetJoint->name() );
//		if( animJoint ){
//			targetJoint->setAbsoluteMatrix( animJoint->absoluteMatrix() );
//		}
//	}
//	target.update();
//}
//////QVector3D o = RnVec3::Null;
////void RnSkeletonAnimation::mapTo( float time, RnSkeleton &target ){
////	//o += RnVec3::Right * 0.01f;
////	updateSkeletonToTime( time );
////	target.rootJoint()->recalcRelativeMatrix();
////	for( int j = 0; j < target.joints().size(); j++ ){
////		RnSkeletonJoint* renderJoint = target.joints().at( j );
////		RnSkeletonJoint* animJoint = _skeleton->findJointByName( renderJoint->name() );
////		if( animJoint ){
////			RnPosOri posori = animJoint->calcPositionOrientation();
////			QMatrix4x4 m;
////			m.scale( 0.1f );
////			m.rotate( 180, RnVec3::Up );
////			//m.rotate( 180, RnVec3::Forward );
////			m.scale( -1, 1, 1 );
////			posori = RnPosOri::map( m, posori );
////			//posori.translate( o );
////			renderJoint->setAbsoluteMatrix( posori.matrix() );
////		}
////	}
////	target.update();
////}
////void RnSkeletonAnimation::updateSkeletonToTime( float time ){
////	foreach( auto node, nodes() ){
////		QMatrix4x4 m;
////		node->calcRelativeMatrixAt( time, m );
////		auto joint = _skeleton->getJointByName( node->name() );
////		joint->setRelativeMatrix( m );
////	}
////	_skeleton->update();
////}

// */




