#include "stdodt.h"

#include "RenderInstance.h"

RenderInstance::RenderInstance( const Mat4& posori )
    :_posori( posori )
	,_color( 1, 1, 1, 1 )
{
}
RenderInstance::~RenderInstance(){
}
void RenderInstance::setPosOri( const Mat4& posori ){
	_posori = posori;
}
void RenderInstance::setColor( const vec4& color ){
	_color = color;
}
void RenderInstance::setTextureTile( float textureTile ){
	_textureTile = textureTile;
}

RenderInstancePNT::RenderInstancePNT( const Mat4& posori, Mesh<VertexPNT>* mesh, Material* material )
    :RenderInstance( posori )
	,_mesh( asserted( mesh ) )
	,_material( asserted( material ) )
{
}
RenderInstancePNT::~RenderInstancePNT(){
}
void RenderInstancePNT::resetMeshMaterial( Mesh<VertexPNT>* mesh, Material* material ){
	_mesh = asserted( mesh );
	_material = asserted( material );
}

RenderSkinInstance::RenderSkinInstance( const Mat4& posori, Skin* skin )
	:RenderInstance( posori )
	,_skin( asserted( skin ) )
{
}
RenderSkinInstance::~RenderSkinInstance(){
}

