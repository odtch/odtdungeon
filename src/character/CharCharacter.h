#pragma once

#include "CharCore.h"
#include "CharPose.h"
////#include "CharAnimationBlender.h"
////#include "CharLocomotion.h"
////#include "Command.h"
////#include "Navigation.h"

////class CharAnimPoint {
////public:
////	float _time_until_breakable;
////	CharAnimPointType* _type;
////	PhArticulationLink* _link;
////	PhControlPoint* _controlPoint;
////	Vec3 _current_position;
////};

////class CharBone
////{
////	DEBUGCOUNTER( CharBone );
////private:
////	CharBoneType* _type;
////private:
////	CharBone* _parent;
////private:
////	glm::quat _rotation;
////private:
////public:
////	Mat4 _parent_matrix;
////	Mat4 _relative_matrix;
////	Mat4 _absolute_matrix;
////public:
////	PosOri _prev_posori;
////	PosOri _next_posori;
////private:
////	Mat4 _skin_matrix;
////	List<Mat4*> _skin_matrixes;
////public:
////	PhArticulationLink* _link;
////public:
////	explicit CharBone( CharBone* parent, CharBoneType* type );
////	virtual ~CharBone();
////public:
////	bool isRoot() const{ return _parent == null; }
////	bool hasParent() const{ return _parent != null; }
////	CharBone* parent() const{ assert( _parent ); return _parent; }
////public:
////	void setRotation( const glm::quat& rotation );
////public:
////	CharBoneType* type() const{ return _type; }
////public:
////	friend class CharCharacter;
////	friend class CharAnimationImporter;
////};


//class CharCharacter : public Object
//{
////public:
////	static const int ClassId;
////public:
////	//PhArticulation
//////public:
//////	RnSimpleCharacterControl control;
////public:
////	enum ControlMode {
////		UnControlMode,
////		ControlControlMode,
////		CommandControlMode,
////////		PointControlMode,
//////////		BehaviorControlMode,
//////////		RotationControlMode,
//////////		BrainControlMode,
//////		AnimationControlMode,
////		UnhandledControlMode
////	} _controlMode;
////public:
////	class Control : public CharLocomotionInput {
////	public:
//////		float moveX;
//////		float moveY;
////		//float rotation;
////		bool prepare_jump;
////		float jump_power;
//////		float _jump;
//////	public:
//////		float crouch;
//////		float speed;
////	public:
////		Vec3 velocity;
////	public:
////		float tilt_y;
////	public:
////		float time_in_air;
////	public:
////		Control();
////	public:
////	} control;
//////////private:
//////////	PosOri _targetPosOri;
//////////private:
//////////	QList<CharPoint*> _points;
//////////private:
//////////	QList<CharSegment*> _segments;
//////////private:
//////////	QList<CharBehavior*> _behaviors;
//////////private:
//////////	CharBrainControl* _brainControl;
////////private:
////////	QList<PhControlPoint*> _control_points;
////////private:
////////	CharPose* _pose;
////////	CharPose* _temp_pose;
//////////public:
//////////	QList<CharAnimPoint*> _animationPoints;
//////////	CharAnimTransition* _animationTransition;
//////////	float _timeInAnimTransition;
///////
////private:
////	Commander _commander;
////public:
////	Navigator* _navigator;
////private:
////	CharNotCharacterFilter _notCharacterFilter;
////////private:
////////	struct Side {
////////		PhArticulationLink* _foot_link;
////////		int _foot_index;
////////		PhArticulationLink* _lowerleg_link;
////////		int _lowerleg_index;
////////		PhArticulationLink* _upperleg_link;
////////		int _upperleg_index;
////////		PhSweep* _foot_down_sweep;
////////		float _ground_z;
////////	} _l, _r;
////private:
////	QList<CharBone*> _bones;
//////private:
////public:
////	PhArticulation* _articulation;
////public:
////	CharAnimationBlender _animationBlender;
////	float _locomotion_time_factor;
//////private:
//////	CharAnimationAction* _action;
//////	CharPose _prev_pose;
//////	CharPose _current_pose;
//////	CharAnimationTransition* _transition;
//////	float _time_in_transition;
//////public:
//////	struct EndPoint {
//////		CharBone* bone;
//////		PosOri current_posori;
//////		PhSweep* current_sweep_down;
//////		float in_ground;
//////		float curr_contact_count;
//////		bool current_contact;
//////	} _leftFoot, _rightFoot;
//////	bool _firstAnimate;
//////	Vec3 _translation;
//////	Vec3 _force;
//////	Vec3 _velocity;
//////public:
////////	PhControlPoint* _cp_center;
////////	PhControlPoint* _cp_foot_l;
////////	PhControlPoint* _cp_foot_r;
////public:
////	bool _renderSkin;
////private:
////	qint8 _skinPartVariants[ RnSkin::MaxPartVariants ];
////	Mat4 _absMatrices[ RnSkin::Max_Matrix_Count ];
////	Mat4 _skinMatrices[ RnSkin::Max_Matrix_Count ];
////public:
////	bool _renderDebug;
////private:
////	RnMeshBuffer<RnVertexPositionColor>* _debugMesh;
////	RnLines* _debugLines;
////public:
//////	struct Sweeps {
////////		PhSweep* lower_down;
//////		PhSweep* lower_forward;
////////		PhSweep* full;
//////	} _sweeps;
////	PhSweep* _sweep;
//public:
//	explicit CharCharacter( CharCharacterType* type );
//	virtual ~CharCharacter() override;
////private:
////	PhSweep* createSweep( PhSweepType* type );
//////private:
//////	void initEndPoint( EndPoint& endpoint, CharEndPointType& endpointtype );
////public:
////	const QList<CharBone*>& bones() const{ return _bones; }
////	CharBone* root_bone() const {  assert( !_bones.isEmpty() ); return _bones.first(); }
////	CharBone* getBoneForType( CharBoneType* bone_type ) const;
////public:
////	void switchToCommandControl();
////	Commander& commander(){ return _commander; }
////public:
////	//void loadPose( const CharPose& pose );
//////public:
//////	virtual void translate( const Vec3& positionOffset, bool moveChilds ) override;
////public:
////	virtual void animate( float t ) override;
////private:
////	void animateControl( float t );
////public:
////	virtual void render( RnRenderer& renderer ) override;
//////public:
//////	void setAction( CharAnimationAction* action );
////public:
//////	void animateByAnimation( float t );
////	void loadPose( const CharPose& pose );
////	void loadBonesToSkin();
//////	void applyAnimationTransitionTransform( float t );
//////	void animateEndPoint( EndPoint& endpoint );
////////private:
////////	void init_side( Side& side, const QString& side_name );
////////	void destroy_side( Side& side );
////////public:
////////	const PosOri& targetPosOri() const{ return _targetPosOri; }
////////public:
////////	const QList<CharPoint*>& points() const{ return _points; }
////////	CharPoint* getPointForName( const QString& name ) const;
////////	CharPoint* getPointForType( const CharPointType* type ) const;
////////public:
////////	const QList<CharSegment*>& segments() const{ return _segments; }
//////////	CharSegment* getSegmentForName( const QString& name ) const;
//////////	CharSegment* getSegmentForType( const CharSegmentType* type ) const;
////////public:
////////	void switchToUnControl();
////////public:
////////	void switchToPointControl();
////////	void loadPose( const CharPose& pose );
////////	void loadAnimationStateToControlPoints( const CharAnimationState& state );
//////////	void reposPointsByAttachment();
//////////public:
//////////	void switchToRotationControl();
//////////public:
//////////	void switchToBehaviorControl();
//////////	const QList<CharBehavior*>& behaviors() const{ return _behaviors; }
//////////	void internalAddBehavior( CharBehavior* behavior );
//////////public:
//////////	void switchToBrainControl();
//////////	CharBrainControl* brainControl() const{ ASSERT( _controlMode == BrainControlMode ); ASSERT( _brainControl ); return _brainControl; }
//////////public:
//////////	void addAnimationPoint( CharAnimPoint* animPoint );
//////////	void switchToAnimationControl();
//////////	void startAnimation( CharAnimTransition* animationTransition );
//////////	void loadStateToAnimPoints( const CharAnimState& state );
//////////	void animatePoints( float dt );
//////////	void applyAnimationTransitionTransform( float dt );
//////////	void updateAnimPoints( float dt );
//////////protected:
//////////	virtual void onAdded( RnScene* scene ) override;
//////////	virtual void onRemoved( RnScene* scene ) override;
////////private:
////////	void animate_side_1( Side& side );
////////	void animate_side_2( Side& side );
////public:
////	CharCharacterType* type() const { return (CharCharacterType*) RnObject::type(); }
////public:
////	virtual bool isKindOf( int classId ) const override { if( classId == ClassId )return true; return RnObject::isKindOf( classId ); }
//	/*
//public:
//	virtual bool isCharacter() const override { return true; }
//	virtual CharCharacter* asCharacter() override { return this; }
//	*/
//};
