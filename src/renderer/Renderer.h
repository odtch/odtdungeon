#pragma once

#include "utils/Debugging.h"
#include "utils/Logger.h"
#include "utils/Threading.h"
#include "math/PosOri.h"
#include "resource/Material.h"
#include "resource/Mesh.h"

#include "RenderInstance.h"

class Renderer : public Thread
{
	DEBUGCOUNTER(Renderer);
private:
public:
	explicit Renderer();
	virtual ~Renderer();
public:
    virtual Material* createMaterial( const String& name ) = 0;
public:
    virtual MeshPNT* createDynamicMeshPNT( const String& name ) = 0;
public:
    virtual RenderInstancePNT* createInstance( const PosOri& posori, MeshPNT* mesh, Material* material ) = 0;
};
