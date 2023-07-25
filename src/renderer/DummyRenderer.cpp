#include "stdodt.h"

//#include "DummyRenderer.h"

//class DummyRenderInstancePNT : public RenderInstancePNT
//{
//public:
//    explicit DummyRenderInstancePNT( const PosOri& posori, Mesh<VertexPNT>* mesh, Material* material )
//        :RenderInstancePNT( posori, mesh, material ){}
//    virtual ~DummyRenderInstancePNT() override{}
//};



//DummyRenderer::DummyRenderer(){
//}
//DummyRenderer::~DummyRenderer(){
//}
//Material* DummyRenderer::createMaterial( const String& name ){
//    Material* material = new Material( name );
//    return material;
//}

//Mesh<VertexPNT>* DummyRenderer::createDynamicMeshPNT( const String& name ){
//    MeshPNT* mesh = new MeshPNT( name );
//    return mesh;
//}
//RenderInstancePNT* DummyRenderer::createInstance( const PosOri& posori, Mesh<VertexPNT>* mesh, Material* material ){
//    RenderInstancePNT* instance = new DummyRenderInstancePNT( posori, mesh, material );
//    return instance;
//}
//void DummyRenderer::run(){
//    while( !should_stop() ){
//        logDebug( "DummyRenderer::run" );
//        sleep_ms( 1300 );
//    }
//}

