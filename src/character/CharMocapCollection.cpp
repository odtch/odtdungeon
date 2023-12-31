#include "stdodt.h"
#include "CharMocapCollection.h"
#include "resource/CollectionImporter.h"
#include "CharRagdoll.h"
#include "CharAnimation.h"
//#include "CharSkin.h"
#include "CharImporter.h"
#include "converter/AssImp.h"

const char* CharMocapCollection::Id = "Mocap";
CharMocapCollection* CharMocapCollection::Get(){
	return Singleton::Get<CharMocapCollection>();
}
CharMocapCollection::CharMocapCollection()
	:Collection( Id )
{
//	File::DeleteFileIfExists( "/home/rt/odtdungeon/media/Mocap.obc" );
}
CharMocapCollection::~CharMocapCollection(){
}
void impanim( CollectionImporter& importer, CharImporter& charimporter, const String& id, const String& filename ){
	AssImp a2;
	a2.open( filename, AssImp::YUp_to_ZUp_Synty2() );
	assert( 1 == a2.animationCount() );
	AssImpAnimation* a2anim = a2.loadAnimation();
	importer.resource( id, charimporter.loadAnimation( *a2anim ) );
	odelete( a2anim );
}
void CharMocapCollection::import( CollectionImporter& importer ){
	Material* material = importer.materialWithTexture( "mcg", "/home/rt/media/mocap/FBX_Ninja_v27_Pro/MotusMan_v55/MotusMan_v55.fbm/MCG_diff.jpg" );
	material->setFlag( MaterialFlag_CalcNormalFromTriangle );
	CharImporter charimporter( CharImporter::MocapFormat );
	charimporter.createRagdoll();
	{
		AssImp assimp;
		assimp.open( "/home/rt/media/mocap/MotusMan_v55/MotusMan_v55.fbx", AssImp::YUp_to_ZUp_Synty2() );
		Skeleton* skeleton = assimp.loadSkeleton();
		charimporter.setupRagdollFromSkeleton( *skeleton );
		charimporter.loadSkin( *skeleton, assimp, 0 );
		odelete( skeleton );
		CharRagdollType* ragdolltype = charimporter.ragdolltype();
		importer.resource( "mm", ragdolltype );
		File::Find( "/home/rt/media/mocap/FBX_Ninja_v27_Pro/Animation/Root_Motion", File::FindOptions( ".fbx", false ), [&]( const File::Entry& entry ){
			impanim( importer, charimporter, entry.name, entry.path );
		} );
		File::Find( "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion", File::FindOptions( ".fbx", false ), [&]( const File::Entry& entry ){
			impanim( importer, charimporter, entry.name, entry.path );
		} );
		File::Find( "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/IPC", File::FindOptions( ".fbx", false ), [&]( const File::Entry& entry ){
			impanim( importer, charimporter, entry.name, entry.path );
		} );
		//assert( false );
		//impanim( importer, charimporter, "wf", "/home/rt/media/mocap/FBX_Ninja_v27_Pro/Animation/Root_Motion/NJA_Rlx_Walk_Forward_Loop.fbx" );
	}

//	CharImporter charimporter( CharImporter::MocapFormat );
//	charimporter.createRagdoll();
//	{
//		AssImp assimp;
//		assimp.open( "/home/rt/media/mocap/MotusMan_v55/MotusMan_v55.fbx" );
//		Skeleton* skeleton = assimp.loadSkeleton();
//		charimporter.setupRagdollFromSkeleton( *skeleton );
//		charimporter.loadSkin( *skeleton, assimp, 0 );
//		odelete( skeleton );
//		addResource( "MotusMan", charimporter.ragdolltype() );
//	}{
//		AssImp assimp;
//		assimp.open( "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Walk_F_Loop.fbx" );
//		AssImpAnimation* assimp_animation = assimp.loadAnimation();
//		CharAnimation* animation = charimporter.loadAnimation( *assimp_animation );
//		addResource( "MOB1_Walk_F_Loop", animation );
//	}{
//		AssImp assimp;
//		assimp.open( "/home/rt/media/mocap/FBX_Ninja_v27_Pro/Animation/Root_Motion/NJA_Cbt_Forward_Roll_v3.fbx" );
//		AssImpAnimation* assimp_animation = assimp.loadAnimation();
//		CharAnimation* animation = charimporter.loadAnimation( *assimp_animation );
//		addResource( "NJA_Cbt_Forward_Roll_v3", animation );
//	}{
//		AssImp assimp;
//		assimp.open( "/home/rt/media/mocap/FBX_Ninja_v27_Pro/Animation/Root_Motion/NJA_Rlx_Stand_Idle.fbx" );
//		AssImpAnimation* assimp_animation = assimp.loadAnimation();
//		CharAnimation* animation = charimporter.loadAnimation( *assimp_animation );
//		addResource( "NJA_Rlx_Stand_Idle", animation );
//	}
}
