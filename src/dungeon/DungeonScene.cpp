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
		Material* material = renderer().createMaterial( "dungeonboxmaterial" );
		material->setColor( Vec4( 0.5f, 0.5f, 0.5f, 1.0f ) );
		Texture* texture_red = renderer().loadTexture( "test_red" );
		{
			MeshPNT* mesh = renderer().createDynamicMeshPNT( "dungeonboxmesh" );
			MeshBuilder::CreateBox( *mesh, PosOri().translated( Vec3( 800, 600, 0 ) ).rotated( 30, Vec3::Right ).rotated( 15, Vec3::Up ).rotated( 15, Vec3::Forward ), Vec3( 600, 400, 1 ), VertexPNT() );
			RenderLayer* uilayer = renderer().createLayer();
			renderer().createInstance( uilayer, PosOri(), mesh, material );
		}
		{
			Camera* camera = new Camera();
			RenderLayer* layer = renderer().createLayer();
			renderer().addLight( layer, RenderLight::CreateAmbient( vec4( 0.8f, 0.5f, 0.5f, 1.0f ) ) );
			renderer().addLight( layer, RenderLight::CreateDirectional( Vec3( 0.1f, 0.1f, -1.0f ).normalized(), vec4( 0.5f, 0.8f, 0.5f, 1.0f ) ) );
			MeshPNT* mesh = renderer().createDynamicMeshPNT( "dungeonboxmesh2" );
			MeshBuilder::CreateBox( *mesh, PosOri(), Vec3( 2, 4, 1 ), VertexPNT() );

			renderer().createInstance( layer, PosOri().translated( Vec3( 0, 0, 0 ) ).rotated( 30, Vec3::Right ).rotated( 15, Vec3::Up ).rotated( 15, Vec3::Forward ), mesh, material );
		}
    }
    Scene::run();
}
