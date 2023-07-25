#pragma once

#include "RenderCore.h"
#include "utils/Threading.h"
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
	virtual Texture* loadTexture( const String& name ) = 0;
public:
	virtual RenderLayer* createLayer() = 0;
    virtual MeshPNT* createDynamicMeshPNT( const String& name ) = 0;
public:
	virtual RenderInstancePNT* createInstance( RenderLayer* layer, const PosOri& posori, MeshPNT* mesh, Material* material ) = 0;
};
