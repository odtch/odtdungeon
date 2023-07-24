#pragma once

#include "utils/Debugging.h"
#include "utils/Logger.h"
#include "math/PosOri.h"
#include "resource/Material.h"
#include "resource/Mesh.h"

class RenderInstance
{
private:
    PosOri _posori;
protected:
    explicit RenderInstance( const PosOri& posori );
    virtual ~RenderInstance();
public:
    const PosOri& posori() const{ return _posori; }
    void setPosOri( const PosOri& posori );
};

class RenderInstancePNT : public RenderInstance
{
private:
public:
protected:
    explicit RenderInstancePNT( const PosOri& posori, Mesh<VertexPNT>* mesh, Material* material );
    virtual ~RenderInstancePNT() override;
public:
};
