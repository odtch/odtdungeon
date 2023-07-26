#include "stdodt.h"
//#include "CharMocapCollection.h"
//#include "CharRagdoll.h"
//#include "CharAnimation.h"
//#include "CharSkin.h"
//#include "CharImporter.h"
//#include "resource/CollectionImporter.h"
//#include "converter/AssImp.h"

//const char* CharMocapCollection::Id = "CharMocapCollection";
//CharMocapCollection* CharMocapCollection::Get(){
//	return Singleton::Get<CharMocapCollection>();
//}
//CharMocapCollection::CharMocapCollection()
//	:Collection( Id )
//{
////	File::DeleteFileIfExists( "/home/rt/build-odtspace-Desktop-Debug/media/CharMocapCollection.obc" );
//}
//CharMocapCollection::~CharMocapCollection(){
//}
//void CharMocapCollection::load( const String& mediapath ){
//	Collection::load( mediapath );
//}
//void CharMocapCollection::import( CollectionImporter& importer ){
//	Material* material = importer.importMaterial( "Material", "/home/rt/media/mocap/FBX_Ninja_v27_Pro/MotusMan_v55/MotusMan_v55.fbm/MCG_diff.jpg" );
//	material->setFlag( MaterialFlag_CalcNormalFromTriangle );
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


//	Collection::import( importer );
//}
//void CharMocapCollection::build(){
//	Collection::build();
//}
