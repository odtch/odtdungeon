#pragma once

#include "RenderCore.h"

class RenderLayer
{
	DEBUGCOUNTER(RenderLayer);
protected:
	explicit RenderLayer();
	virtual ~RenderLayer();
public:
	virtual void setNextFixedCamera( const Camera& nextCamera ) = 0;
};
