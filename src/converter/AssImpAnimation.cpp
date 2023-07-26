#include "stdodt.h"
#include "AssImpAnimation.h"

AssImpAnimation::AssImpAnimation(){
}
AssImpAnimation::~AssImpAnimation(){
	while( !_frames.isEmpty() ){
		AssImpAnimationFrame* frame = _frames.takeLast();
		odelete( frame->_skeleton );
		odelete( frame );
	}
}
AssImpAnimationFrame* AssImpAnimation::findFrameStartingAt( float starttime ) const {
	float starttimemin = starttime - 0.0001f;
	float starttimemax = starttime + 0.0001f;
	for( AssImpAnimationFrame* frame : _frames ){
		if( starttimemin < frame->_starttime && frame->_starttime <= starttimemax )
			return frame;
	}
	return null;
}
AssImpAnimationFrame*AssImpAnimation::findFrameAt( float time ) const {
	for( AssImpAnimationFrame* frame : _frames ){
		if( frame->_starttime <= time && time <= frame->_endtime )
			return frame;
	}
	return null;
}

//AssImpAnimationRenderer::AssImpAnimationRenderer( AssImpAnimation* animation, Object* object )
//	:ObjectProperty( object )
//	,_animation( asserted( animation ) )
//	,_time( 0 )
//{
//	_renderer = new SkeletonRenderer( null, object );
//	AssImpAnimationFrame* frame = _animation->frames().first();
//	_renderer->load( frame->skeleton() );
//}
//AssImpAnimationRenderer::~AssImpAnimationRenderer(){
//	odelete( _animation );
//}
//void AssImpAnimationRenderer::animate( float dt ){
//	_time += dt * 1.0f;
//	if( _animation->duration() < _time ){
//		_time = 0;
//	}
//	AssImpAnimationFrame* frame = _animation->findFrameAt( _time );
//	if( frame ){
//		_renderer->load( frame->skeleton() );
//	}
//	/*
//	AssImpPose pose;
//	_animation->getPose( _time, pose );
//	_skeleton->loadPose( pose );
//	*/
//}
