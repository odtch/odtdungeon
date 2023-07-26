#pragma once

#include "RenderCore.h"
#include "utils/Threading.h"
#include "RenderInstance.h"

class Renderer : public Thread
{
	DEBUGCOUNTER(Renderer);
private:
	std::timed_mutex _writeMutex;
public:
	explicit Renderer();
	virtual ~Renderer();
public:
    virtual Material* createMaterial( const String& name ) = 0;
	virtual Texture* loadTexture( const String& name ) = 0;
public:
    virtual MeshPNT* createDynamicMeshPNT( const String& name ) = 0;
public:
	virtual RenderLayer* createRootLayer() = 0;
	virtual RenderLayer* createNextLayer( RenderLayer* prev ) = 0;
public:
	virtual RenderInstancePNT* createInstance( RenderLayer* layer, const PosOri& posori, MeshPNT* mesh, Material* material ) = 0;
	virtual void addLight( RenderLayer* layer, RenderLight* light ) = 0;
public:
	std::timed_mutex& writeMutex(){ return _writeMutex; }
};
