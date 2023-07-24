#include "stdodt.h"

#include "RenderInstance.h"

RenderInstance::RenderInstance( const PosOri& posori )
    :_posori( posori )
{
}
RenderInstance::~RenderInstance(){
}

RenderInstancePNT::RenderInstancePNT( const PosOri& posori, Mesh<VertexPNT>* mesh, Material* material )
    :RenderInstance( posori )
{
}
RenderInstancePNT::~RenderInstancePNT(){
}

