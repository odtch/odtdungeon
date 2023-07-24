#pragma once

#include "utils/Debugging.h"
#include "utils/Logger.h"
#include "utils/Threading.h"

class Renderer : public Thread
{
	DEBUGCOUNTER(Renderer);
private:
public:
	explicit Renderer();
	virtual ~Renderer();
public:
};
