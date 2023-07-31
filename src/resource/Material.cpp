#include "stdodt.h"

#include "Material.h"
#include "Texture.h"
#include "ResourceType.h"
#include "ResourceStorage.h"
//#include "MaterialAlternatives.h"
//#include "vulkan/raytracing/VulkanRaytracerFlags.h"

Material::Material()
	:
//	,_texture_has_transparency( false )
//	,_emissiveTexture( null )
	_color( 1, 1, 1, 1 )
//	,_transparency( 0, 0, 0, 0 )
//	,_reflection( 0, 0, 0, 0 )
{
	static std::atomic<uint32_t> next_materialindex( 0 );
	_materialindex = next_materialindex.fetch_add( 1, std::memory_order_relaxed );
}
Material::~Material(){
}
bool Material::hasFlag( uint flag ) const{
	return _flags & flag;
}
void Material::setFlag( uint flag, bool enabled ){
	if( enabled ){
		_flags |= flag;
	} else {
		_flags &= (~flag);
	}
	setModified();
}
//void Material::setTranslucent( bool translucent ){
//	_translucent = translucent;
//	setModified();
//}
////void Material::setTranslucent(){
////	_type = MaterialType_Translucent;
////}
////void Material::setRenderable( bool renderable ){
////	setFlag( Material_Renderable, renderable );
////}
////void Material::setCreatesShadow( bool creates_shadow ){
////	setFlag( Material_CreatesShadow, creates_shadow );
////}
////void Material::setCreatesLight( bool creates_light ){
////	setFlag( Material_CreatesLight, creates_light );
////}
////void Material::setLightAffected(bool f){
////	setFlag( Material_LightAffected, f );
////}
void Material::setTexture( Texture* texture ){
	assert( texture );
	_texture = texture;
	setModified();
}
////void Material::setTextureHasTransparency( bool f ){
////	setFlag( Material_TextureHasTransparency, f );
////}
////void Material::setEmissiveTexture( Texture* texture ){
////	assert( texture );
////	_emissiveTexture = texture;
////}
void Material::setColor( const Vec4& color ){
    _color = color;
	setModified();
}
void Material::setModified( bool modified ){
	_modified = modified;
}
////void Material::setTransparency( const glm::vec4& transparency ){
////	_transparency = transparency;
////}
//void Material::setReflection(const glm::vec4 &reflection){
//	_reflection = reflection;
//	setModified();
//}
//void Material::setTileCount( const TileCount& tileCount ){
//	_tileCount = tileCount;
//	setModified();
//}
//void Material::setTileCount( uint x, uint y ){
//	_tileCount.x = x;
//	_tileCount.y = y;
//	setModified();
//}
ResourceType* Material::type() const {
	return Singleton::Get<MaterialType>();
}
void Material::load( BinaryFileReader& reader ){
//	Resource::load( reader );
	_flags = reader.read_uint32();
//	_translucent = reader.read_bool();
	assert( _texture == null );
	uint8_t t = reader.read_uint8();
	if( t == 0 ){
	} else if( t == 1 ){
		_texture = asserted( dynamic_cast<Texture*>( asserted( dynamic_cast<ResourceReader*>( &reader ) )->read_reference() ) );
	} else {
		assert( false );
	}
	_color = reader.read_vec4();
//	_reflection = reader.read_vec4();
	_tileCount.x = reader.read_uint32();
	_tileCount.y = reader.read_uint32();
	setModified();
}
void Material::save( BinaryFileWriter& writer ) const {
//	Resource::save( writer );
	writer.write_uint32( _flags );
//	writer.write_bool( _translucent );
	if( _texture == null ){
		writer.write_uint8( 0 );
	} else {
		writer.write_uint8( 1 );
		asserted( dynamic_cast<ResourceWriter*>( &writer ) )->write_reference( _texture );
	}
	writer.write_vec4( _color );
//	writer.write_vec4( _reflection );
	writer.write_uint32( _tileCount.x );
	writer.write_uint32( _tileCount.y );
}
////const List<Material*>& Material::alternatives() const
////{
////	assert( _alternatives );
////	return _alternatives->materials();
////}
////void Material::setViewportIndex( int viewport_index ){
////	_viewport_index = viewport_index;
////}
