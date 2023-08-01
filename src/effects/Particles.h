#pragma once

#include "EffectsCore.h"

struct Particle {
	ODTNOCOPY(Particle)
public:
	explicit Particle();
public:
	MeshPNT* mesh = null;
	Material* material = null;
public:
	float time = 0; // 0 to duration
	float duration = 0; // <=0 endless,
public:
	PosOri posori;
public:
	Vec3GradientTable positionOverLifetime;
public:
	Vec3 velocity;
	Vec3 acceleration;
	float directed_velocity	= 0;
	float directed_xrot = 0;
public:
	float scale = 1;
	const FloatGradientTable* scaleOverLifetime = null;
public:
	vec4 color = vec4( 1, 1, 1, 1 );
	const ColorGradientTable* colorOverLifetime = null;
public:
	void set( const Particle& src );
public:
	void animate( float f01, float dt );
private:
	bool _living = false;
	RenderInstancePNT* _instance = null;
public:
	friend class ParticleSystem;
};

//class ParticleRenderable : public RenderablePNT
//{
//private:
//	bool _unused = true;
//private:
//	float _lifetime = 0;
//private:
//	Particle _particle;
//public:
//	explicit ParticleRenderable( Mesh<VertexPNT>* mesh, Material* material );
//	virtual ~ParticleRenderable() override;
//private:
//	void reset( const Particle& particle );
//	void update( float f01, float dt );
//private:
//	friend class ParticleSystem;
//};

class ParticleSystem : public SceneNode
{
public:
	static ParticleSystem* Get( SceneArea* area );
//private:
//	SceneArea* _area = null;
	Renderer* _renderer = null;
	RenderLayer* _layer = null;
private:
	MeshPNT _defaultmesh;
	Material _defaultmaterial;
//private:
//	List<ParticleRenderable*> _renderables;
//	std::queue<ParticleRenderable*> _unused_renderables;
private:
	List<Particle*> _particles;
	std::queue<Particle*> _unused_particles;
public:	
	explicit ParticleSystem( uint particleCount ); // , SceneArea* area );
	virtual ~ParticleSystem() override;
public:
//	uint freeParticleCount() const{ return _unused_renderables.size(); }
	void emitParticle( const Particle& particle );
//private:
//	ParticleRenderable* tryCreateRenderable();
//	void destroyRenderable( ParticleRenderable* particle );
protected:
	virtual void onAddedToScene( Scene* scene ) override;
	virtual void onRemovedFromScene( Scene* scene ) override;
public:
	virtual void animate( float dt ) override;
	void updateParticle( const Particle& particle );
};
