#include "stdodt.h"
#include "CharAnimation.h"
#include "CharResource.h"


//CharAnimationSystem::CharAnimationSystem()
//	:_stand( "Stand" )
//	,_walk_f( "Walk F" )
//	,_run_f( "Run F" )
//	,_crouch( "Crouch" )
//{
//}
//CharAnimationSystem::~CharAnimationSystem(){
//	qDeleteAll( _sequences );
//	//qDeleteAll( _states );
//	qDeleteAll( _transitions );
//}

//CharAnimationState::CharAnimationState( const CharPose& pose, float time )
//	:RnSteppedTimelineStep( time )
//	,_pose( pose )
//{
//}
//CharAnimationState::~CharAnimationState(){
//	_out_transitions.clear();
//}

//CharAnimationTransition* CharAnimationState::findOutTransition(CharAnimationAction* action) const
//{
//	CharAnimationTransition* result = null;
//	foreach( auto transition, _out_transitions ){
//		if( transition->_action == action ){
//			assert( result == null );
//			result = transition;
//		}
//	}
//	return result;
//}

//CharAnimationTransition* CharAnimationState::getOutTransition(CharAnimationAction* action) const
//{
//	CharAnimationTransition* result = findOutTransition( action );
//	if( result == null ){
//		qDebug() << "no transition to" << action->name();
//		assert( false );
//		throw Exception( "no out-animation-transition" );
//	}
//	return result;
//}

//CharAnimationAction::CharAnimationAction(const QString& name )
//	:_name( name )
//{
//}
//CharAnimationAction::~CharAnimationAction(){
//}
//void CharAnimationSequence::calcPose( float time, CharPose& pose ) const {
//	if( time <= 0.0f ){
//		if( time < 0.0f ){
//			qDebug() << "CharAnimationSequence::calcPose time < 0" << time;
//		}
//		pose.set( _states.first()->_pose );
//		return;
//	}
//	if( duration() <= time ){
//		if( duration() < time ){
//			qDebug() << "CharAnimationSequence::calcPose time > duration()" << time << duration();
//		}
//		pose.set( _states.last()->_pose );
//		return;
//	}
//	RnSteppedTimelineInterpolation<CharAnimationState> i;
//	calcInterpolation( time, i );
//	assert( i.hasStep0() && i.hasStep1() );
//	pose.interpolate( i.step0()->_pose, i.factor01(), i.step1()->_pose );
//}

CharAnimation::CharAnimation(){
}
CharAnimation::~CharAnimation(){
	_frames.deleteAll();
}
//ResourceType* CharAnimation::type() const {
//	return Singleton::Get<CharAnimationResourceType>();
//}
CharAnimationFrame* CharAnimation::findFrameStartingAt( float starttime ) const {
	float starttimemin = starttime - 0.0001f;
	float starttimemax = starttime + 0.0001f;
	for( CharAnimationFrame* frame : _frames ){
		if( starttimemin < frame->_starttime && frame->_starttime <= starttimemax )
			return frame;
	}
	return null;
}
CharAnimationFrame*CharAnimation::findFrameAt( float time ) const {
	for( CharAnimationFrame* frame : _frames ){
		if( frame->_starttime <= time && time <= frame->_endtime )
			return frame;
	}
	return null;
}
void CharAnimation::getPose( float time, CharPose& pose ) const {
	for( CharAnimationFrame* frame : _frames ){
		if( frame->_starttime <= time && time <= frame->_endtime ){
			ASSERT( 0 < frame->_duration );
			if( frame->_next == null ){
				ASSERT( false );
				pose.set( frame->_pose );
				return;
			}
			float f = ( time - frame->_starttime ) / frame->_duration;
			//logDebug( "f", time, frame->_starttime, f, frame->_pose.translation() );
			pose.interpolate( frame->_pose, 1.0f - f, frame->_next->_pose );
			return;
		}
	}
	assert( false );
}
void CharAnimation::load( BinaryFileReader& reader ){
	Resource::load( reader );
	int ac = reader.read_uint32();
	while( _frames.size() < ac ){
		CharAnimationFrame* frame = new CharAnimationFrame();
		frame->_duration = reader.read_float();
		frame->_pose.load( reader );
		_frames.add( frame );
	}
	float exp_duration = reader.read_float();
	recalc();
	assert( exp_duration == _duration );
}
void CharAnimation::save( BinaryFileWriter& writer ) const {
	Resource::save( writer );
	writer.write_uint32( _frames.size() );
	for( CharAnimationFrame* frame : _frames ){
		writer.write_float( frame->_duration );
		frame->_pose.save( writer );
	}
	writer.write_float( _duration );
}
void CharAnimation::recalc(){
	CharAnimationFrame* prev = null;
	for( CharAnimationFrame* frame : _frames ){
		if( prev == null ){
			frame->_starttime = 0;
		} else {
			assert( 0 < prev->_duration );
			prev->_next = frame;
			frame->_starttime = prev->_endtime;
		}
		frame->_endtime = frame->_starttime + frame->_duration;
		frame->_next = null;
		prev = frame;
	}
	assert( prev );
	_duration = prev->_endtime;
	assert( 0 < _duration );
}
