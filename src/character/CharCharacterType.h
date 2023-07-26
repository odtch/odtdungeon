#pragma once

#include "CharCore.h"

//class CharBoneType
//{
//	DEBUGCOUNTER( CharBoneType )
////private:
////	String _name;
////private:
////	CharBoneType* _parent;
////private:
////	int _index_in_character;
////private:
////	RnPosOri _joint_T_posori_in_skin;
////	RnPosOri _joint_T_posori_in_parent_bone;
////public:
////	QMatrix4x4 _absolute_T_matrix;
////	QMatrix4x4 _relative_T_matrix;
////private:
////	QMatrix4x4 _skin_inv_matrix;
////	QList<int> _skin_matrix_indexes;
////public:
////	PhArticulationLinkType* _link;
//public:
//	explicit CharBoneType(); // const QString& name, CharBoneType* parent, const RnPosOri& joint_T_posori_in_skin );
//	virtual ~CharBoneType();
////public:
////	bool isRoot() const{ return _parent == null; }
////	bool hasParent() const{ return _parent != null; }
////	CharBoneType* parent() const{ assert( _parent ); return _parent; }
////public:
////	int index_in_character() const{ assert( 0 <= _index_in_character ); return _index_in_character; }
////public:
////	const RnPosOri& joint_T_posori_in_skin() const{ return _joint_T_posori_in_skin; }
////	const RnPosOri& joint_T_posori_in_parent() const{ return _joint_T_posori_in_parent_bone; }
////public:
////	const QMatrix4x4& skinInvMatrix() const{ return _skin_inv_matrix; }
////	void addSkinMatrixIndex( int skin_matrix_index );
////	const QList<int> skinMatrixIndexes() const{ return _skin_matrix_indexes; }
////public:
////	friend class CharCharacterType;
//};

//class CharCharacterType : public ObjectType
//{
//private:
////	RnSkin* _skin;
////	QMatrix4x4 _skinInvMatrices[ RnSkin::Max_Matrix_Count ];
////	QHash<int,CharBoneType*> _skin_matrix_index_to_bone;
////private:
////	QList<CharBoneType*> _bones;
////private:
////	float _scale;
////public:
////	PhArticulationType* _articulation;
////////private:
////////	QVector3D _eyesPosition;
////////private:
////////	QList<CharPointType*> _points;
////////private:
////////	QList<CharSegmentType*> _segments;
////////private:
////////	CharPoseSet* _poses;
////////public:
////////	float max_step_length;
////////private:
////////	CharBrainControlType* _brainControl;
////////public:
////////	PhControlPointType* _control_point_type_hard;
////////	PhControlPointType* _control_point_type_soft;
//////////private:
//////////	PhControlPointType* _animControlPointType;
//////////	QList<CharAnimPointType*> _animPoints;
////////public:
////////	PhSweepType* _sweep_center_down;
////////	PhSweepType* _sweep_foot_down;
//////public:
//////	CharEndPointType _leftFoot;
//////	CharEndPointType _rightFoot;
////public:
////	struct Sweeps {
////		PhSweepType* lower;
////		PhSweepType* full;
////	} _sweeps;
//public:
//	explicit CharCharacterType(); // RnSkin* skin, float scale );
//	virtual ~CharCharacterType() override;
////public:
////	void initialize();
////public:
////	RnSkin* skin() const{ return _skin; }
////public:
////	float scale() const{ return _scale; }
////public:
////	const QList<CharBoneType*>& bones() const { return _bones; }
////	CharBoneType* root_bone() const{ assert( !_bones.isEmpty() ); return _bones.first(); }
////	CharBoneType* getBoneForName( const QString& name );
////	void addBone( CharBoneType* bone );
//////public:
//////	const QVector3D& eyesPosition() const { return _eyesPosition; }
//////public:
//////	const QList<CharPointType*>& points() const{ return _points; }
//////	void addPoint( CharPointType* point );
//////private:
//////	void createPoints( PhArticulationLinkType* link );
//////public:
//////	const QList<CharSegmentType*>& segments() const{ return _segments; }
//////	void addSegment( CharSegmentType* segment );
//////public:
//////	CharPoseSet& poses() const;
//////public:
//////	CharBrainControlType* brainControl() const{ ASSERT( _brainControl ); return _brainControl; }
//////public:
//////	PhControlPointType* animControlPointType() const{ return _animControlPointType; }
//////	const QList<CharAnimPointType*>& animPoints() const{ return _animPoints; }
//////private:
//////	void createAnimPointTypes();
//};
