#include "stdodt.h"

#include "RenderInstance.h"

RenderInstance::RenderInstance( const PosOri& posori )
    :_posori( posori )
	,_color( 1, 1, 1, 1 )
{
}
RenderInstance::~RenderInstance(){
}

RenderInstancePNT::RenderInstancePNT( const PosOri& posori, Mesh<VertexPNT>* mesh, Material* material )
    :RenderInstance( posori )
	,_mesh( asserted( mesh ) )
	,_material( asserted( material ) )
{
}
RenderInstancePNT::~RenderInstancePNT(){
}

