#include "stdodt.h"
#include "Particles.h"
#include "scene/Scene.h"
#include "scene/SceneArea.h"
#include "resource/MeshBuilder.h"
//#include "vulkan/resource/VulkanMesh.h"

Particle::Particle()
{

}

void Particle::set( const Particle& src ){
	mesh = src.mesh;
	material = src.material;
	f01 = src.f01;
	time = src.time;
	duration = src.duration;
	posori = src.posori;
	positionOverLifetime = src.positionOverLifetime;
	velocity = src.velocity;
	acceleration = src.acceleration;
	directed_velocity = src.directed_velocity;
	directed_xrot = src.directed_xrot;
	scale = src.scale;
	scaleOverLifetime = src.scaleOverLifetime;
	color = src.color;
	colorOverLifetime = src.colorOverLifetime;
}

void Particle::animate( float f01, float dt ){
	ASSERT( 0 <= f01 && f01 <= 1 );
	//this->f01 = f01;
	velocity += acceleration * dt;
	if( !positionOverLifetime.isEmpty() ){
		posori.setPosition( Vec3( positionOverLifetime.getByFactor( f01 ) ) );
		ASSERT( directed_velocity == 0 );
	} else if( directed_velocity != 0 ){
		posori.translate( posori.direction() * directed_velocity * dt + velocity * dt );
	} else {
		posori.translate( velocity * dt );
	}
	if( directed_xrot != 0 ){
		posori.rotate( directed_xrot * dt, posori.right() );
	}
	if( colorOverLifetime ){
		color = colorOverLifetime->getByFactor( f01 );
	}
	if( scaleOverLifetime ){
		scale = scaleOverLifetime->getByFactor( f01 );
	}
}

//ParticleRenderable::ParticleRenderable(Mesh<VertexPNT>* mesh, Material* material)
//	:RenderablePNT( mesh, material )
//{
//}
//ParticleRenderable::~ParticleRenderable(){
//}
//void ParticleRenderable::reset( const Particle& particle ){
//	_material = particle.material;
//	ASSERT( _mesh );
//	ASSERT( _material );
//	_lifetime = 0;
//	_particle = particle;
//}



ParticleSystem* ParticleSystem::Get( SceneArea* area ){
	assert( area );
	for( SceneNode* child : area->childs() ){
		ParticleSystem* system = dynamic_cast<ParticleSystem*>( child );
		if( system )
			return system;
	}
	assert( false );
}
ParticleSystem::ParticleSystem( uint particleCount, RenderLayer* layer  ) // , SceneArea* area
//	:_area( asserted( area ) )
	:_layer( asserted( layer ) )
{
//	_layer = asserted( _area->layer() );
	_unusedposition = Vec3::Down * 5;
	MeshBuilder::CreateParticleCross( _defaultmesh, Vec3::Null, 1.0f );
	//_defaultmesh.setGeometryFlags( 0 );
	_defaultmaterial.setColor( Vec4( 1, 0, 0, 1 ) );
//	if( true ){
//		_defaultmaterial.setTranslucent( true );
//		_layer = _layer->translucentLayer();
//	}
////	_particles.add( new Particle( &_crossmesh, &_defaultmaterial ) );
////	_particles.add( new Particle( &_directedmesh, &_defaultmaterial ) );
//	while( _renderables.size() < particleCount ){
//		_renderables.add( new ParticleRenderable( &_defaultmesh, &_defaultmaterial ) );
//	}
//	for( ParticleRenderable* particle : _renderables ){
//		_unused_renderables.push( particle );
//		particle->setPosOri( Mat4::Translation( Vec3::Up * _renderables.size() * 1.1f ) );
//	}
////	{
////		Particle* particle = tryCreateParticle();
////		assert( particle );
////		particle->_mesh = &_directedmesh;
////		particle->_duration01 = 0.1f;
////		particle->_lifetime = 0;
////	}
//	_area->addChild( this );
	while( _particles.size() < particleCount ){
		Particle* particle = new Particle();
		_particles.add( particle );
		ASSERT( particle->_living == false );
		_unused_particles.push( particle );
	}
}
ParticleSystem::~ParticleSystem(){
	_particles.deleteAll();
}
void ParticleSystem::emitParticle( const Particle& aparticle ){
	ASSERT( 0 == aparticle.duration || 0 < aparticle.duration );
	ASSERT( _layer ); ASSERT( _renderer );
	if( _unused_particles.empty() ){
		static uint errorcount = 10;
		if( 0 < errorcount ){
			logError( "no unused particles" );
			errorcount--;
		}
		return;
	}
	Particle* particle = _unused_particles.front();
	_unused_particles.pop();
	assert( particle->_living == false );
	assert( particle->_instance );
	particle->set( aparticle );
	particle->_living = true;
	ASSERT( particle->_instance );
	particle->_instance->resetMeshMaterial( particle->mesh, particle->material );
	//particle->_instance = _renderer->createInstance( _layer, particle->posori, particle->mesh, particle->material );
	particle->f01 = 0;
	updateParticle( *particle );
}
//ParticleRenderable* ParticleSystem::tryCreateRenderable(){
//	ParticleRenderable* particle = _unused_renderables.front();
//	ASSERT( particle );
//	ASSERT( particle->_unused );
//	_unused_renderables.pop();
//	if( !_unused_renderables.empty() ){
//		assert( particle != _unused_renderables.front() );
//	}
//	particle->_unused = false;
//	particle->_lifetime = 0;
//	return particle;
//}
//void ParticleSystem::destroyRenderable( ParticleRenderable* particle ){
//	ASSERT( particle );
//	ASSERT( particle->_unused == false );
//	particle->_unused = true;
//	particle->_lifetime = 0;
//	_unused_renderables.push( particle );
//	particle->_material = &_defaultmaterial;
//	if( particle->_mesh == &this->_defaultmesh ){
//	} else {
//		//logDebug( "rel ", particle->_mesh->id() );
//		particle->_mesh->vulkanMesh()->decInstanceCount();
//		particle->_mesh = &this->_defaultmesh;
//	}
//	particle->setPosOri( Mat4::Translation( Vec3::Up * _unused_renderables.size() * 1.1f ));
//}
void ParticleSystem::onAddedToScene( Scene* scene ){
////	_layer->renderer()->loadMeshToGPU( &_crossmesh );
////	_layer->renderer()->loadMeshToGPU( &_directedmesh );
	SceneNode::onAddedToScene( scene );
	ASSERT( _layer );
	//ASSERT( _layer == null );
	//_layer = area()->layer();
	_renderer = &scene->renderer();
	PosOri posori;
	for( Particle* particle : _particles ){
		ASSERT( particle->_instance == null );
		particle->_instance = _renderer->createInstance( _layer, posori.matrix(), &_defaultmesh, &_defaultmaterial );
		posori.translate( Vec3::Down );
	}
}
void ParticleSystem::onRemovedFromScene(Scene* scene ){
////	logDebug( "ParticleSystem::onRemovedFromScene" );
////	logInfo( "Particle used", ( _particles.size() - _unuseds.size() ), "/", _particles.size(), " Free:", _unuseds.size() );
//	for( ParticleRenderable* particle : _renderables ){
//		if( !particle->_unused ){
//			destroyRenderable( particle );
//		}
////	}
////	logInfo( "Particle used", ( _particles.size() - _unuseds.size() ), "/", _particles.size(), " Free:", _unuseds.size() );
////	for( Particle* particle : _particles ){
//		_layer->removeRenderable( particle );
//	}
////	logInfo( "Particle used", ( _particles.size() - _unuseds.size() ), "/", _particles.size(), " Free:", _unuseds.size() );
	_layer = null;
	_renderer = null;
	SceneNode::onRemovedFromScene( scene );
}
void ParticleSystem::animate( float dt ){
	_unusedposition = Vec3::Down * 5;
	for( Particle* particle : _particles ){
		if( particle->_living ){
			ASSERT( particle->_instance );
			particle->time += dt;
			if( particle->duration <= 0 ){
				particle->animate( 0, dt );
			} else if( particle->time <= particle->duration ){
				float f01 = particle->time / particle->duration;
				particle->f01 = f01;
				particle->animate( f01, dt );
			} else {
				particle->_living = false;
				_unused_particles.push( particle );
			}
		}
		updateParticle( *particle );
	}
	SceneNode::animate( dt );
////#ifdef ODTDEBUG
//	static float time_to_next_log = 5.5f;
//	time_to_next_log -= dt;
//	if( time_to_next_log < 0 ){
//		logInfo( "Particle used", ( _renderables.size() - _unused_renderables.size() ), "/", _renderables.size(), " Free:", _unused_renderables.size() );
//		time_to_next_log += 5;
//	}
////#endif
}
void ParticleSystem::updateParticle( const Particle& particle ){
	if( particle._living ){
		particle._instance->setPosOri( particle.posori.matrix() * Mat4::Scale( particle.scale ) );
		particle._instance->setColor( particle.color );
		assert( 0 <= particle.f01 && particle.f01 <= 1 );
		particle._instance->setTextureTile( particle.f01 );
	} else {
		particle._instance->setPosOri( Mat4::Translation( _unusedposition ) );
		_unusedposition += Vec3::Down;
	}
}
