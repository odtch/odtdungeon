#pragma once

#include "CharCore.h"
#include "converter/AssImpCore.h"

class CharImporter
{
public:
	enum Format { MocapFormat, MecanimFormat };
public:
	struct Bone {
		String name;
//		vec4 color;
		Bone* parent_bone = null;
		PosOri absoluteTposori;
		CharJointType* joint = null;
		String skin_root_joint_name;
		Mat4 skin_absoluteTposori;
		Mat4 skin_relativeTposori;
//		RnSkeletonJoint* skin_root_joint = null;
//		List<RnSkeletonJoint*> skin_joints;
//		PosOri joint_global_posori;
//		List<Vec3> global_points;
//		List<Vec3> local_points;
	};
	List<Bone*> _bones;
	Bone* _root_bone = null;
public:
	Bone _head;
	Bone _neck;
	Bone _spine_1;
	Bone _spine_0;
	struct Side {
		String name_prefix;
		Vec3 direction;
		String skin_joint_ext_name;
		Bone upperarm;
		Bone lowerarm;
		Bone hand;
		Bone upperleg;
		Bone lowerleg;
		Bone foot;
	} _l, _r;
	List<Side*> _sides;
private:
	CharRagdollType* _ragdolltype = null;
public:
	MeshPNT* _testmesh = null;
public:
	explicit CharImporter( Format format );
	~CharImporter();
private:
	void setupBones();
	void setupJointNamesMocap();
	void setupJointNamesMecanim1();
	void setupJointNamesMecanim2();
public:
	void createRagdoll();
	void setupRagdollFromSkeleton( const Skeleton& skeleton );
	void loadSkin( const Skeleton& skeleton, AssImp& assimp, uint meshindex );
	void mapSkeletonToRagdoll( const Skeleton& skeleton, CharRagdoll& ragdoll );
private:
	Bone* findBoneForSkeletonJoint( SkeletonJoint* aijoint ) const;
	Bone* getBoneForSkeletonJoint( SkeletonJoint* aijoint ) const;
public:
	CharRagdollType* ragdolltype() const{ return asserted( _ragdolltype ); }
public:
	CharAnimation* loadAnimation( const AssImpAnimation& skinanimation );
};

//public:
//	CharCharacterType* _character_type;
//private:
////public:
////	static void Start( RnCollection* collection, RnWidget* window );
////private:
////	RnSkin* _skin;
////	RnSkeleton* _skinSkeleton;
////private:
////	struct Part {
////		QString name;
////		Part* parent;
////		QList<Part*> childs;
////		QString skinJointName;
////		RnSkeletonJoint* skinJoint;
////		Vec3 jointPosition;
////		PhArticulationLinkType* link;
////		PhArticulationJointType* joint;
////	public:
////		Part()
////			:parent( null )
////			,skinJoint( null )
////			,link( null )
////			,joint( null )
////		{}
////	};
//private:
//	RnLines _lines;
//public:
//	CharCharacterType* create();
//	void calcJoints();
//	void mapSkinJointsToBones();
//	void mapSkinJointToBone( RnSkeletonJoint* skin_joint, Bone* bone );
//	void calcBones();
//	void createCharacterType();
//public:
//	virtual void render( RnRenderer& renderer ) override;
//};

