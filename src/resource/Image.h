#pragma once

#include "Resource.h"


class Image : public Resource
{
	DEBUGCOUNTER( Image )
private:
	uint _width = 0;
	uint _height = 0;
	unsigned char* _pixels = null;
public:
	explicit Image();
	virtual ~Image() override;
public:
	uint width() const{ return _width; }
	uint height() const{ return _height; }
	unsigned char* pixels(){ ASSERT( _pixels ); return _pixels; }
public:
    glm::vec4 pixel( uint x, uint y ) const;
    void set( uint x, uint y, const glm::vec4& color );
public:
    void fill( const glm::vec4& color );
public:
	virtual ResourceType* type() const override;
public:
	void create( uint width, uint height );
	virtual void load( BinaryFileReader& reader ) override;
	virtual void save( BinaryFileWriter& writer ) const override;
#ifdef ODTDEBUG
	void import( const String& filename );
//	static Array2D<vec4>* Import16( const String& filename );
#endif
	void fixAlphaErrorToBlack();
	void convertGrayToAlpha();
};
