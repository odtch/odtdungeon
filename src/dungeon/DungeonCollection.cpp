#include "stdodt.h"

#include "DungeonCollection.h"
#include "resource/CollectionImporter.h"

const char* DungeonCollection::Id = "Dungeon";
DungeonCollection* DungeonCollection::Get(){
	return Singleton::Get<DungeonCollection>();
}
DungeonCollection::DungeonCollection()
	:Collection( Id )
{
//	File::DeleteFileIfExists( "/home/rt/build-odtspace-Desktop-Debug/media/DungeonCollection.obc" );
}
DungeonCollection::~DungeonCollection(){
}
void DungeonCollection::import( CollectionImporter& importer ){
	importer.materialWithTexture( "dt01", "/home/rt/media/Polygon_Dungeon/Textures/Dungeons_Texture_01.png" );
	importer.mesh( "platform5", "/home/rt/media/Polygon_Dungeon/FBX/SM_Env_Rock_Flat_Platform_05.fbx", CollectionImporter::YUp_to_ZUp_Synty1() );
	importer.mesh( "banner02", "/home/rt/media/Polygon_Dungeon/FBX/SM_Wep_Banner_02.fbx", CollectionImporter::YUp_to_ZUp_Synty1() );
	importer.materialWithTexture( "fk01", "/home/rt/media/Polygon_Fantasy_Kingdom/Source_Files/Textures/PolygonFantasyKingdom_Texture_01_A.png" );
}
