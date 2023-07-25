#include "stdodt.h"

#include "DungeonScene.h"
#include "math/Camera.h"
#include "renderer/RenderLight.h"
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
		renderer().addLight( layer, RenderLight::CreateAmbient( vec4( 0.8f, 0.5f, 0.5f, 1.0f ) ) );
		renderer().addLight( layer, RenderLight::CreateDirectional( Vec3( 0.1f, 0.1f, -1.0f ).normalized(), vec4( 0.5f, 0.8f, 0.5f, 1.0f ) ) );

        Material* material = renderer().createMaterial( "dungeonboxmaterial" );
        material->setColor( Vec4( 0.5f, 0.5f, 0.5f, 1.0f ) );
		Texture* texture_red = renderer().loadTexture( "test_red" );
        MeshPNT* mesh = renderer().createDynamicMeshPNT( "dungeonboxmesh" );
		MeshBuilder::CreateBox( *mesh, PosOri().translated( Vec3( 800, 600, 0 ) ).rotated( 30, Vec3::Right ).rotated( 15, Vec3::Up ).rotated( 15, Vec3::Forward ), Vec3( 600, 400, 1 ), VertexPNT() );
		renderer().createInstance( layer, PosOri(), mesh, material );
    }
    Scene::run();
}
