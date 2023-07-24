#include "stdodt.h"

#include "DungeonScene.h"
#include "resource/MeshBuilder.h"

DungeonScene::DungeonScene( Renderer* renderer )
    :Scene( renderer )
{
}
DungeonScene::~DungeonScene(){
}
void DungeonScene::run(){
    {
        Material* material = renderer().createMaterial( "dungeonboxmaterial" );
        material->setColor( Vec4( 0.5f, 0.5f, 0.5f, 1.0f ) );
        MeshPNT* mesh = renderer().createDynamicMeshPNT( "dungeonboxmesh" );
        MeshBuilder::CreateBox( *mesh, PosOri(), Vec3( 2, 4, 1 ), VertexPNT() );
        renderer().createInstance( PosOri(), mesh, material );
    }
    Scene::run();
}
