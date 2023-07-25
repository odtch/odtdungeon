#include "stdodt.h"

#include "DungeonScene.h"
#include "math/Camera.h"
#include "resource/MeshBuilder.h"

DungeonScene::DungeonScene( Renderer* renderer )
    :Scene( renderer )
{
}
DungeonScene::~DungeonScene(){
}
void DungeonScene::run(){
    {
		Camera* camera = new Camera();
		RenderLayer* layer = renderer().createLayer();
        Material* material = renderer().createMaterial( "dungeonboxmaterial" );
        material->setColor( Vec4( 0.5f, 0.5f, 0.5f, 1.0f ) );
		Texture* texture_red = renderer().loadTexture( "test_red" );
        MeshPNT* mesh = renderer().createDynamicMeshPNT( "dungeonboxmesh" );
        MeshBuilder::CreateBox( *mesh, PosOri(), Vec3( 2, 4, 1 ), VertexPNT() );
		renderer().createInstance( layer, PosOri(), mesh, material );
    }
    Scene::run();
}
