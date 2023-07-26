#pragma once

#include "SkinCore.h"
//#include "utils/Timeline.h"

//struct SkinPose {
//	DEBUGCOUNTER( SkinPose );
//private:
//	uint _rotationCount;
//	glm::quat* _rotations;
//public:
//	explicit SkinPose();
//	explicit SkinPose( uint rotationCount );
//	explicit SkinPose( const SkinPose& src );
//	~SkinPose();
//private:
//	void reserve( uint rotationCount );
//public:
//	const glm::quat& getRotation( uint channel ) const {
//		assert( channel < _rotationCount );
//		return _rotations[ channel ];
//	}
//	void setRotation( uint channel, const glm::quat& rotation );
//public:
//	void operator=( const SkinPose& src );
//public:
//	friend class AssImp;
//};

//class SkinAnimationChannel {
//private:
//	uint _index;
//	String _name;
//public:
//	explicit SkinAnimationChannel( uint index, const String& name );
//	virtual ~SkinAnimationChannel();
//public:
//	uint index() const{ return _index; }
//	const String& name() const{ return _name; }
//public:
//	friend class AssImp;
//};

//class SkinAnimationFrame : public TimelineEntry {
//private:
//	SkinPose _pose;
//	SkinSkeleton* _skeleton;
//public:
//	explicit SkinAnimationFrame( float time );
//	virtual ~SkinAnimationFrame() override;
//public:
//	const SkinPose& pose() const { return _pose; }
//	const SkinSkeleton* skeleton() const{ assert( _skeleton ); return _skeleton; }
//	SkinSkeleton* skeleton(){ assert( _skeleton ); return _skeleton; }
//public:
//	friend class AssImp;
//};

//class SkinAnimation : public Resource
//{
//	DEBUGCOUNTER(SkinAnimation)
//private:
//	const SkinSkeleton* _skeleton;
//private:
//	List<SkinAnimationChannel*> _channels;
//private:
//	Timeline<SkinAnimationFrame> _timeline;
//public:
//	explicit SkinAnimation( SkinSkeleton* skeleton );
//	virtual ~SkinAnimation();
//public:
//	float duration() const{ return _timeline.duration(); }
//public:
//	const List<SkinAnimationChannel*>& channels() const{ return _channels; }
//public:
//	const List<SkinAnimationFrame*>& frames() const{ return _timeline.entries(); }
//public:
//	const SkinAnimationFrame* getFrame( float time ) const;
//	//void getPoseAt( float time, SkinPose& pose ) const;
////	const SkinSkeleton* test( float t ) const;
////public:
////	const SkinSkeleton* skeleton() const{ return _skeleton; }
////public:
////	const List<Mesh<SkinVertex>*>& meshes() const{ return _meshes; }
////private:
////	void setInvMatrices();
////public:
////	void mapMeshesToSkeleton( const SkinSkeleton* skeleton );
//private:
//	void createFrameIfNeeded( float time );
//public:
//	friend class AssImp;
//};

///*
//class RnSkeletonAnimation
//{
//	DEBUGCOUNTER( RnSkeletonAnimation );
//private:
//	QString _name;
//private:
//	float _duration;
//private:
//	RnSkeleton* _skeleton;
//private:
//	mutable RnSkeleton* _tmpskeleton;
//private:
//	class Step : public RnSteppedTimelineStep {
//	private:
//		RnSkeleton* _skeleton;
//	public:
//		explicit Step( float time, RnSkeleton* skeleton )
//			:RnSteppedTimelineStep( time )
//			,_skeleton( skeleton )
//		{
//			ASSERT( _skeleton );
//		}
//		virtual ~Step(){
//			delete _skeleton;
//		}
//	public:
//		const RnSkeleton* skeleton() const{ return _skeleton; }
//	public:
//		friend class RnSkeletonAnimation;
//		friend class RnAssImp;
//	};
//	RnSteppedTimeline<Step> _timeline;
//public:
//	explicit RnSkeletonAnimation( const QString& name, float duration, RnSkeleton* skeleton );
//	virtual ~RnSkeletonAnimation();
//public:
//	const QString& name() const { return _name; }
//public:
//	float duration() const { return _duration; }
//public:
//	const RnSkeleton* skeleton() const{ return _skeleton; }
//	//	RnSkeleton* internalSkeleton() const{ return _skeleton; }
//public:
//	int stepCount() const{ return _timeline.steps().size(); }
//	float timeAtStep( int step ) const { return _timeline.steps().at( step )->time(); }
//	const RnSkeleton* skeletonAtStep( int step ) const { return _timeline.steps().at( step )->skeleton(); }
//	void createStep( float time, RnSkeleton* skeleton );
//	const RnSteppedTimeline<Step>& timeline() const{ return _timeline; }
//public:
//	void mapToSameSkeletonType( float time, RnSkeleton& target ) const;
//	void mapToSkinSkeleton( float time, RnSkeleton& target ) const;
//	static void mapAnimToSkinSkeleton( const RnSkeleton& animSkeleton, RnSkeleton& target );
////public:
////	const QList<Key*>& keys() const { return _keys; }
////	Key* getOrCreateKeyAt( float time );
////private:
////	RnSkeletonAnimationNode* createNode( const QString& name );
////	RnSkeletonAnimationNode* getNodeByName( const QString& name ) const;
////	RnSkeletonAnimationNode* findNodeByName( const QString& name ) const;
////public:
////
////private:
////	void updateSkeletonToTime( float time );
//private:
//	void ensureStepAtExists( float time );
//public:
//	friend class RnAssImp;
//	friend class RnAnimationTest;
//};

// */
