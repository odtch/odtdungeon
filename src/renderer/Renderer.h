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
	virtual RenderLayer* createRootLayer() = 0;
	virtual RenderLayer* createNextLayer( RenderLayer* prev ) = 0;
	virtual RenderLayer* createTranslucentLayer( RenderLayer* prev ) = 0;
public:
	virtual RenderInstancePNT* createInstance( RenderLayer* layer, const Mat4& posori, MeshPNT* mesh, Material* material ) = 0;
	virtual RenderSkinInstance* createSkinInstance( RenderLayer* layer, const Mat4& posori, Skin* skin ) = 0;
public:
	virtual void addLight( RenderLayer* layer, RenderLight* light ) = 0;
public:
	std::timed_mutex& writeMutex(){ return _writeMutex; }
};
