#include "stdodt.h"
#include "CharSkin.h"
#include "CharJoint.h"
#include "CharRagdoll.h"
#include "CharResource.h"

CharSkinPartType::CharSkinPartType( uint32_t jointindex, const String& jointname, Mesh<VertexPNT>*mesh)
	:_jointindex( jointindex )
	,_jointname( jointname )
	,_mesh( asserted( mesh ) )
{
}
CharSkinPartType::~CharSkinPartType(){
}

CharSkinType::CharSkinType(const String &id)
	:Resource( id )
{
}
CharSkinType::~CharSkinType(){
	for( CharSkinPartType* part : _parts ){
		odelete( part );
	}
	_parts.removeAll();
}
//ResourceType*CharSkinType::type() const {
//	return Singleton::Get<CharSkinResourceType>();
//}
void CharSkinType::load( BinaryFileReader& reader ){
	Resource::load( reader );
	uint partcount = reader.read_uint32();
	while( _parts.size() < partcount ){
		uint32_t jointindex = reader.read_uint32();
		String jointname;
		reader.read_string( jointname );
		MeshPNT* mesh = new MeshPNT( "CharSkinType" );
		mesh->load( reader );
		CharSkinPartType* part = new CharSkinPartType( jointindex, jointname, mesh );
		_parts.add( part );
	}
	reader.read_magicnumber( 's' );
}
void CharSkinType::save( BinaryFileWriter& writer ) const {
	Resource::save( writer );
	writer.write_uint32( _parts.size() );
	for( CharSkinPartType* part : _parts ){
		writer.write_uint32( part->jointindex() );
		writer.write_string( part->jointname() );
		part->mesh()->save( writer );
	}
	writer.write_uint32( 's' );
}

CharSkinPart::CharSkinPart( CharSkinPartType* type, Material* material, CharJoint* joint, Object* object )
	:/*ObjectRenderableProperty( asserted( type )->mesh(), asserted( material ), object )
//	,*/_joint( asserted( joint ) )
{
}
CharSkinPart::~CharSkinPart(){
}
void CharSkinPart::animate( float dt ){
	assert( false ); //setPosOri( _joint->posori() );
	ObjectRenderableProperty::animate( dt );
}



CharSkin::CharSkin( CharSkinType* type, Material* material, CharRagdoll* skeleton, Object* object )
	:/*ObjectProperty( object )
	,*/_type( asserted( type ) )
	,_material( asserted( material ) )
	,_skeleton( asserted( skeleton ) )
{
	for( CharSkinPartType* parttype : _type->parts() ){
		CharJoint* joint = skeleton->joints().get( parttype->jointindex() );
#ifdef ODTDEBUG
		if( joint->type()->name() != parttype->jointname() ){
			logError( "CharSkin Skeleton not map: ", parttype->jointindex(), parttype->jointname(), joint->type()->index(), joint->type()->name() );
			ASSERT( false );
		}
#endif
		CharSkinPart* part = new CharSkinPart( parttype, _material, joint, object );
		_parts.add( part );
	}
}
CharSkin::~CharSkin(){
	_type = null;
	_parts.removeAll();
}
