#pragma once

#include "Resource.h"
#include "Image.h"

//class TextureType : public ResourceType
//{
//public:
//	static const char* Id;
//public:
//	explicit TextureType();
//	virtual ~TextureType() override;
//public:
//	virtual Resource* newInstance() override;
//};

class Texture : public Resource
{
	DEBUGCOUNTER(Texture)
private:
//	String _filename;
	Image* _image = null;
private:
//	int _tileCountX = 1;
//	int _tileCountY = 1;
//public:
//	int _vulkanIndex = -1;
public:
    explicit Texture( const String& name );
//	explicit Texture( const String& filename );
//	explicit Texture( Image* data );
	virtual ~Texture();
//public:
//	virtual ResourceType* type() const override { return Singleton::Get<TextureType>(); }
public:
	Image* image() const{ assert( _image ); return _image; }
	void setImage( Image* image );
public:
	virtual void load( BinaryFileReader& reader ) override;
	virtual void save( BinaryFileWriter& writer ) const override;
//	bool isFile() const{ return !_filename.isEmpty(); }
//	const String& filename() const{ ASSERT( isFile() ); return _filename; }
//public:
//	void load( Image& data );
//	bool isLoaded() const{ return _data != null; }
//	Image* data(){ ASSERT( _data ); return _data; }
//public:
//	void setTileCount( int x, int y );
//	int tileCountX() const{ return _tileCountX; }
//	int tileCountY() const{ return _tileCountY; }
};

