#pragma once

#include "Renderer.h"

class DummyRenderer : public Renderer
{
private:
public:
    explicit DummyRenderer();
    virtual ~DummyRenderer() override;
public:
protected:
    virtual void run() override;
};
