#pragma once

#include "SceneNode.h"

class SceneObject : public SceneNode
{
private:
	PosOri _posori;
public:
	explicit SceneObject();
	virtual ~SceneObject() override;
public:
	const PosOri& posori() const{ return _posori; }
	void setPosOri( const PosOri& posori );
};
