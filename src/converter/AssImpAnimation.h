#pragma once

#include "AssImpCore.h"

//class AssImpAnimationFrame
//{
//private:
//	float _starttime;
//	float _duration;
//	float _endtime;
//	AssImpAnimationFrame* _next = null;
//	Skeleton* _skeleton = null;
//public:
//	float starttime() const{ return _starttime; }
//	float duration() const{ return _duration; }
//	float endtime() const{ return _endtime; }
//	Skeleton* skeleton() const{ return asserted( _skeleton ); }
//private:
//	friend class AssImpAnimation;
//	friend class AssImp;
//};

//class AssImpAnimation
//{
//	DEBUGCOUNTER(AssImpAnimation)
//private:
//	List<AssImpAnimationFrame*> _frames;
//	float _duration = 0;
//public:
//	explicit AssImpAnimation();
//	virtual ~AssImpAnimation();
//public:
//	float duration() const{ return _duration; }
//public:
//	const List<AssImpAnimationFrame*>& frames() const{ return _frames; }
//	AssImpAnimationFrame* findFrameStartingAt( float starttime ) const;
//	AssImpAnimationFrame* findFrameAt( float time ) const;
//public:
//	friend class AssImp;
//};

//class AssImpAnimationRenderer : public ObjectProperty
//{
//public:
//	AssImpAnimation* _animation;
//	float _time = 0;
//	SkeletonRenderer* _renderer = null;
//public:
//	explicit AssImpAnimationRenderer( AssImpAnimation* animation, Object* object );
//	virtual ~AssImpAnimationRenderer() override;
//public:
//	virtual void animate( float dt ) override;
//};
