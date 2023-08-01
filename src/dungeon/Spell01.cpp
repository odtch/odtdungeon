#include "stdodt.h"
#include "Spell01.h"
#include "scene/Scene.h"
#include "scene/SceneArea.h"
#include "resource/MeshBuilder.h"
#include "renderer/RenderLight.h"
#include "DungeonCollection.h"

Spell01::Spell01(){
//	_material.setColor( Vec4( 0, 0, 1, 1 ) );
//	_material.setFlag( MaterialFlag_NoShadowEmit );
//	_material.setFlag( MaterialFlag_NoLightAffected );
//	"spell01a"
	float r = 0.041f;
	MeshBuilder::CreateParticleDirected( _mesh, Vec3( 0, 0, 0 ), Vec2( r, r ) ); //5, 1 ) * r );// Box( _mesh, PosOri(), Vec3( r, r, r ), VertexPNT() );
	_mesh.setGeometryFlags( 0 );
	//_mesh.transform( Mat4::RotationAtAxis( 90, Vec3::Backward ) );
	_particletype.material = DungeonCollection::Get()->getMaterial( "spell01a" );
	_particletype.mesh = &_mesh;
	_particletype.duration = 2;	
	_particletype.scaleOverLifetime = &_scaleOverLifetime;
	_scaleOverLifetime.add( 0, 0.0f );
	_scaleOverLifetime.add( 0.05f, 1.0f );
	_scaleOverLifetime.add( 1, 0.0f );
}
Spell01::~Spell01(){
	ASSERT( _system == null );
}
void Spell01::fire(){
	//return;
	if( _firetime < -0.8f ){
		_firetime = 1;
		_fireposori = _attachementPosOri;
		_light2->_range = 441;
		_light2->_color = vec4( 0.1f, 0.1f, 0.8f, 1.0f ) * 0.1f;
	}
}
void Spell01::setAttachementPosOri( const PosOri& attachementPosOri ){
	_attachementPosOri = attachementPosOri;
	_attachementPosOri.rotate( -90, _attachementPosOri.right() );
	//_attachementPosOri = PosOri( Vec3( 2, 2, 1 ), Vec3::Forward, Vec3::Up );
}
void Spell01::onAddedToScene( Scene* scene ){
	_light1 = RenderLight::CreatePoint( Vec3( 0, 0, -5 ), 0.71f, vec4( 0.1f, 0.1f, 0.8f, 1.0f ) *3 );
	scene->renderer().addLight( area()->layer(), _light1 );
	_light2 = RenderLight::CreatePoint( Vec3( 0, 0, -5 ), 400, vec4( 0.1f, 0.1f, 0.8f, 1.0f ) );
	scene->renderer().addLight( area()->layer(), _light2 );
	ASSERT( _system == null );
	_system = ParticleSystem::Get( area() );
}
void Spell01::onRemovedFromScene( Scene* scene ){
	_system = null;
}
void Spell01::animate( float dt ){
	int sc = 2;
	dt /= sc;
	for( int s = 0; s < sc; s++ ){
	_timetonext -= dt;
	_uprot += dt * 1312;
	if( _uprot > 360 )_uprot -= 360;
	_firetime -= dt;
	_fireposori.translate( _fireposori.direction() * dt * 8 );
	if( _timetonext < 0 ){
		if( _firetime < 0 ){
			_particletype.posori = _attachementPosOri;
			_particletype.posori.translate( _particletype.posori.up() * -0.125f );
			_particletype.posori.translate( _particletype.posori.direction() * 0.2f );
			_particletype.posori.rotate( _uprot, _particletype.posori.direction() );
			_particletype.posori.translate( _particletype.posori.up() * 0.05f );
			//_particletype.posori.translate( _particletype.posori.right() * 0.2f );
			_particletype.velocity = _particletype.posori.up() * _rnd.getf( 0.007f, 0.009f );
			_particletype.duration = _rnd.getf( 0.5f, 0.6f );
			_light1->setPosition( _particletype.posori.position() + Vec3( 0.1f, 0.25f, 0.0f ) );//+ _particletype.posori.direction() * 0.0035f );
		} else {
			_fireposori.rotate( 11.0f, _fireposori.direction() );
			_particletype.posori = _fireposori;
			_particletype.velocity = _particletype.posori.up() * _rnd.getf( 0.09f, 0.199f )
									 + _fireposori.direction() * _rnd.getf( 0.4f, 0.6f );
			_particletype.duration = _rnd.getf( 2.0f, 3.0f );
		}
//		_particletype.posori = PosOri( Vec3( 2, 2, 1 ), Vec3::Left, Vec3::Up );
//		_particletype.velocity = Vec3::Null + Vec3::Forward * 0.3f;
		//		_particletype.duration = 6;
		//		_timetonext += 8;
		//_particletype.duration += 0.1f;
		//_timetonext += 0.1f;
		_particletype.posori.rotate( 90, Vec3::Right ); //_particletype.posori.right() );
		_particletype.posori.rotate( _rnd.getf( 0, 360 ), Vec3::Forward ); //_particletype.posori.right() );

		_system->emitParticle( _particletype );
		_particletype.velocity *= _rnd.getf( 0.8f, 0.9f );
		_particletype.velocity += Vec3( _rnd.getvec3() ) * 0.1f;
		_system->emitParticle( _particletype );
		//_timetonext += 0.70f / 60;
	}
	if( _firetime < 0 ){
		if( _light2->_range > 0 ){
			//_light2->_range -= 1.5f * dt;
			_light2->_color *= 0.97f;
			//_light2->_update_counter++;
			_light2->setPosition( _light2->position() );
		} else {
			_light2->setPosition( Vec3( 0, 0, -4 ) );
		}
	} else {
		_light1->setPosition( Vec3( 0, 0, -5 ) );
		_light2->setPosition( _fireposori.position() + Vec3::Down * 0.15f );
		//_light2->_range += 4.5f * dt;
		if( _light2->_color[3] < 1.53f )
			_light2->_color *= 1.02598f;
	}
	}
}
