#pragma once

#include "utils/Debugging.h"
#include "utils/Logger.h"

class RenderInstance
{
private:
public:
protected:
    explicit RenderInstance();
    virtual ~RenderInstance();
public:
};

class RenderInstancePNT : public RenderInstance
{
private:
public:
protected:
    explicit RenderInstancePNT();
    virtual ~RenderInstancePNT() override;
public:
};
