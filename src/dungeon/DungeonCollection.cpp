#include "stdodt.h"

#include "DungeonCollection.h"
#include "resource/CollectionImporter.h"

const char* DungeonCollection::Id = "DungeonCollection";
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
	importer.materialWithTexture( "fk01", "/home/rt/media/Polygon_Fantasy_Kingdom/Source_Files/Textures/PolygonFantasyKingdom_Texture_01_A.png" );
}
