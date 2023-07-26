#include "stdodt.h"
//#include "CharTestCollection.h"
//#include "CharRagdoll.h"
//#include "CharAnimation.h"
//#include "CharSkin.h"
//#include "CharImporter.h"
//#include "resource/CollectionImporter.h"
//#include "converter/AssImp.h"

//const char* CharTestCollection::Id = "CharTestCollection";
//CharTestCollection* CharTestCollection::Get(){
//	return Singleton::Get<CharTestCollection>();
//}
//CharTestCollection::CharTestCollection()
//	:Collection( Id )
//{
//	//File::DeleteFileIfExists( "/home/rt/build-odtspace-Desktop-Debug/media/CharTestCollection.obc" );
//}
//CharTestCollection::~CharTestCollection(){
//}
//void CharTestCollection::load( const String& mediapath ){
//	Collection::load( mediapath );
//}
//void CharTestCollection::import( CollectionImporter& importer ){
//	Material* material = importer.importMaterial( "ssm1a", "/home/rt/media/Polygon_SciFi_Space/Textures/PolygonSciFiSpace_Texture_01_A.png" );
//	//material->setFlag( MaterialFlag_CalcNormalFromTriangle );
//	CharImporter charimporter( CharImporter::MecanimFormat );
//	charimporter.createRagdoll();
//	{
//		AssImp assimp;
//		assimp.open( "/home/rt/media/Polygon_SciFi_Space/Characters/SK_Chr_Crew_Female_01.fbx" );
//		Skeleton* skeleton = assimp.loadSkeleton();
//		charimporter.setupRagdollFromSkeleton( *skeleton );
//		charimporter.loadSkin( *skeleton, assimp, 0 );
//		odelete( skeleton );
//		addResource( "SK_Chr_Crew_Female_01", charimporter.ragdolltype() );
//	}{
////		AssImp assimp;
////		assimp.open( "/home/rt/media/Polygon_SciFi_Worlds/src/FBX/Characters.fbx" );
////		AssImpSkeleton* skeleton = assimp.loadSkeleton();
////		charimporter.setupRagdollFromSkeleton( *skeleton );
////		charimporter.loadSkin( *skeleton, assimp, 0 );
////		odelete( skeleton );
////		addResource( "sw1", charimporter.ragdolltype() );
//	}{
//	}


//	Collection::import( importer );
//}
//void CharTestCollection::build(){
//	Collection::build();
//}
