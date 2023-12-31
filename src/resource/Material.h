#pragma once

#include "Resource.h"
#include "MaterialFlags.h"
//#include "Texture.h"


class Material : public Resource
{
	DEBUGCOUNTER( Material )
private:
	uint32_t _materialindex = UINT32_MAX;
private:
	uint32_t _flags = 0;
//private:
//	bool _translucent = false;
private:
	Texture* _texture = null;
////	bool _texture_has_transparency;
////	Texture* _emissiveTexture;
private:
////	//int _type;
    Vec4 _color;
////	vec4 _transparency;
//	vec4 _reflection;
private:
	struct TileCount { uint x = 0; uint y = 0; } _tileCount;
//private:
//	int _viewport_index = -1;
//public:
//	int _raytracer_index = -1;
public:
	bool _modified = true;
//private:
//	MaterialAlternatives* _alternatives = null;
public:
	explicit Material();
	virtual ~Material() override;
public:
	uint32_t materialindex() const{ return _materialindex; }
public:
	uint32_t flags() const{ return _flags; }
	bool hasFlag( uint flag ) const;
	void setFlag( const uint flag, bool enabled = true );
//public:
//	bool isTranslucent() const{ return _translucent; }
//	void setTranslucent( bool translucent = true );
public:
	bool hasTexture() const{ return _texture != null; }
	Texture* texture() const{ assert( hasTexture() ); return _texture; }
	void setTexture( Texture* texture );
////	void setTextureHasTransparency( bool f );
////public:
////	bool hasEmissiveTexture() const{ return _emissiveTexture != null; }
////	Texture* emissiveTexture() const{ assert( hasEmissiveTexture() ); return _emissiveTexture; }
////	void setEmissiveTexture( Texture* emissivetexture );
public:
    const Vec4& color() const{ return _color; }
    void setColor( const Vec4& color );
////public:
////	const vec4& transparency() const{ return _transparency; }
////	void setTransparency( const vec4& transparency );
//public:
//	const vec4& reflection() const{ return _reflection; }
//	void setReflection( const vec4& reflection );
public:
	bool hasTileCount() const{ return ( 0 < _tileCount.x ) || ( 0 < _tileCount.y ); }
	const TileCount& tileCount() const{ ASSERT( hasTileCount() ); return _tileCount; }
	uint tileCountX() const{ return _tileCount.x; }
	uint tileCountY() const{ return _tileCount.y; }
//	void setTileCount( const TileCount& tileCount );
	void setTileCount( uint x, uint y );
////public:
////	int viewportIndex() const{ return _viewport_index; }
////	void setViewportIndex( int viewport_index );
////public:
////	bool hasAlternatives() const{ return _alternatives != null; }
////	const List<Material*>& alternatives() const;
public:
	virtual ResourceType* type() const override;
public:
	virtual void load( BinaryFileReader& reader ) override;
	virtual void save( BinaryFileWriter& writer ) const override;
public:
	bool isModified() const{ return _modified; }
	void setModified( bool modified = true );
//public:
//	friend class Collection;
};


//public:
//	enum MaterialType {
//		MaterialType_Solid = 1,
//		MaterialType_Translucent = 2
//	};
//private:
//	MaterialType _type = MaterialType_Solid;
