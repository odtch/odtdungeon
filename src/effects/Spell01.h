#pragma once

#include "EffectsCore.h"
#include "Particles.h"

class Spell01 : public SceneNode
{
private:
	ParticleSystem* _system = null;
	Random _rnd;
private:
	Material _material;
	MeshPNT _mesh;
	Particle _particletype;
	FloatGradientTable _scaleOverLifetime;
private:
	PosOri _fireposori;
	PosOri _attachementPosOri;
private:
	float _firetime = -1;
	float _timetonext = 1;
	float _uprot = 0;
private:
	RenderLight* _light1 = null;
	RenderLight* _light2 = null;
public:
	explicit Spell01();
	virtual ~Spell01() override;
public:
	void fire();
	void setAttachementPosOri( const PosOri& attachementPosOri );
protected:
	virtual void onAddedToScene( Scene* scene ) override;
	virtual void onRemovedFromScene( Scene* scene ) override;
public:
	virtual void animate( float dt ) override;
};
