#pragma once

#include "CharCore.h"
#include "CharPose.h"

class CharAnimationFrame
{
private:
	float _starttime;
	float _duration;
	float _endtime;
	CharPose _pose;
	CharAnimationFrame* _next = null;
public:
	//CharRagdoll* _skeleton = null;
private:
	friend class CharAnimation;
	friend class CharImporter;
};

class CharAnimation : public Resource
{
	DEBUGCOUNTER(CharAnimation)
private:
	List<CharAnimationFrame*> _frames;
	float _duration = 0;
public:
	explicit CharAnimation();
	virtual ~CharAnimation() override;
public:
	float duration() const{ return _duration; }
public:
	const List<CharAnimationFrame*>& frames() const{ return _frames; }
	CharAnimationFrame* findFrameStartingAt( float starttime ) const;
	CharAnimationFrame* findFrameAt( float time ) const;
public:
	void getPose( float time, CharPose& pose ) const;
public:
	virtual ResourceType* type() const override;
public:
	virtual void load( BinaryFileReader& reader ) override;
	virtual void save( BinaryFileWriter& writer ) const override;
private:
	void recalc();
public:
	friend class CharImporter;
};

//class CharAnimationAction
//{
//	DEBUGCOUNTER( CharAnimationSystem );
//private:
//	QString _name;
//public:
//	explicit CharAnimationAction( const QString& name );
//	~CharAnimationAction();
//public:
//	const QString& name() const{ return _name; }
//};

//class CharAnimationSequence : public RnSteppedTimeline<CharAnimationState>{
//public:
//	QString _name;
//	CharAnimationAction* _action;
//	QList<CharAnimationState*> _states;
//	QList<CharAnimationTransition*> _transitions;
//public:
//	void calcPose( float time, CharPose& pose ) const;
//};


//class CharAnimationSystem : public Singleton {
//	DEBUGCOUNTER( CharAnimationSystem );
//public:
//	static CharAnimationSystem* Get(){ return Singleton::Get<CharAnimationSystem>(); }
//private:
//public:
//	CharAnimationAction _stand;
//	CharAnimationAction _walk_f;
//	CharAnimationAction _run_f;
//	CharAnimationAction _crouch;
////	QList<CharAnimationAction*> _actions;
//public:
//	QList<CharAnimationSequence*> _sequences;
//	QList<CharAnimationState*> _states;
//	QList<CharAnimationTransition*> _transitions;
//public:
//	explicit CharAnimationSystem();
//	~CharAnimationSystem();
//};

//class CharAnimationState : public RnSteppedTimelineStep {
//	DEBUGCOUNTER( CharAnimationState );
//public:
//	QString _name;
//	CharPose _pose;
//	bool _l_foot_fixed;
//	bool _r_foot_fixed;
//public:
//	QList<CharAnimationTransition*> _out_transitions;
//public:
//	explicit CharAnimationState( const CharPose& pose, float time );
//	~CharAnimationState();
//public:
//	CharAnimationTransition* findOutTransition( CharAnimationAction* action ) const;
//	CharAnimationTransition* getOutTransition( CharAnimationAction* action ) const;
//};

//class CharAnimationTransition {
//	DEBUGCOUNTER( CharAnimationTransition );
//public:
//	CharAnimationAction* _action;
//	//CharAnimationState* _from_state;
//	CharAnimationState* _to_state;
//	float _duration;
//	QVector3D _translation;
//};


