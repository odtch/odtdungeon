#include "stdodt.h"

#include "DungeonCollection.h"
#include "resource/CollectionImporter.h"
#include "character/CharImporter.h"
#include "character/CharRagdoll.h"

const char* DungeonCollection::Id = "Dungeon";
DungeonCollection* DungeonCollection::Get(){
	return Singleton::Get<DungeonCollection>();
}
DungeonCollection::DungeonCollection()
	:Collection( Id )
{
	File::DeleteFileIfExists( "/home/rt/odtdungeon/media/Dungeon.obc" );
}
DungeonCollection::~DungeonCollection(){
}
void DungeonCollection::import( CollectionImporter& importer ){
	importer.materialWithTexture( "dt01", "/home/rt/media/Polygon_Dungeon/Textures/Dungeons_Texture_01.png" );
	importer.mesh( "platform05", "/home/rt/media/Polygon_Dungeon/FBX/SM_Env_Rock_Flat_Platform_05.fbx", CollectionImporter::YUp_to_ZUp_Synty1() );
	importer.mesh( "banner01", "/home/rt/media/Polygon_Dungeon/FBX/SM_Wep_Banner_01.fbx", CollectionImporter::YUp_to_ZUp_Synty1() );
	importer.mesh( "banner02", "/home/rt/media/Polygon_Dungeon/FBX/SM_Wep_Banner_02.fbx", CollectionImporter::YUp_to_ZUp_Synty1() );
	importer.mesh( "banner03", "/home/rt/media/Polygon_Dungeon/FBX/SM_Wep_Banner_03.fbx", CollectionImporter::YUp_to_ZUp_Synty1() );
	importer.mesh( "wall01", "/home/rt/media/Polygon_Dungeon/FBX/SM_Env_Wall_01.fbx", CollectionImporter::YUp_to_ZUp_Synty1() );
	importer.mesh( "wall02", "/home/rt/media/Polygon_Dungeon/FBX/SM_Env_Wall_02.fbx", CollectionImporter::YUp_to_ZUp_Synty1() );
	importer.mesh( "wall03", "/home/rt/media/Polygon_Dungeon/FBX/SM_Env_Wall_03.fbx", CollectionImporter::YUp_to_ZUp_Synty1() );
	importer.mesh( "wall04", "/home/rt/media/Polygon_Dungeon/FBX/SM_Env_Wall_04.fbx", CollectionImporter::YUp_to_ZUp_Synty1() );
	importer.mesh( "wall05", "/home/rt/media/Polygon_Dungeon/FBX/SM_Env_Wall_05.fbx", CollectionImporter::YUp_to_ZUp_Synty1() );
	importer.mesh( "wall06", "/home/rt/media/Polygon_Dungeon/FBX/SM_Env_Wall_06.fbx", CollectionImporter::YUp_to_ZUp_Synty1() );
	importer.mesh( "wall07", "/home/rt/media/Polygon_Dungeon/FBX/SM_Env_Wall_07.fbx", CollectionImporter::YUp_to_ZUp_Synty1() );
	importer.mesh( "tile01", "/home/rt/media/Polygon_Dungeon/FBX/SM_Env_Tiles_01.fbx", CollectionImporter::YUp_to_ZUp_Synty1() );
	importer.mesh( "tile02", "/home/rt/media/Polygon_Dungeon/FBX/SM_Env_Tiles_02.fbx", CollectionImporter::YUp_to_ZUp_Synty1() );
	importer.mesh( "tile03", "/home/rt/media/Polygon_Dungeon/FBX/SM_Env_Tiles_03.fbx", CollectionImporter::YUp_to_ZUp_Synty1() );
	importer.materialWithTexture( "fk01", "/home/rt/media/Polygon_Fantasy_Kingdom/Source_Files/Textures/PolygonFantasyKingdom_Texture_01_A.png" );
	importer.resource( "rt", CharImporter::ImportRagdollType( "/home/rt/media/Polygon_Fantasy_Kingdom/Source_Files/Characters/SK_Chr_Soldier_Female_01.fbx", CharImporter::MecanimFormat ) );
					   //"/home/rt/media/Polygon_SciFi_Space/Characters/SK_Chr_Crew_Female_01.fbx"
					   //"/home/rt/media/Polygon_Fantasy_Kingdom/Source_Files/Characters/SK_Chr_Jester_01.fbx"
					   //"/home/rt/media/Polygon_Fantasy_Kingdom/Source_Files/Characters/SK_Chr_Prince_01.fbx"
					   //"/home/rt/media/Polygon_Fantasy_Kingdom/Source_Files/Characters/SK_Chr_Rider_01.fbx"
					   //"/home/rt/media/Polygon_Fantasy_Kingdom/Source_Files/Characters/SK_Chr_Soldier_Male_01.fbx"
}
