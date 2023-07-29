#pragma once

#include "utils/Debugging.h"
#include "utils/String.h"
#include "math/Mat4.h"
//#include "resource/OdtResource.h"

#ifdef ODTDEBUG

class Converter {
public:
	static void Run();
private:
	String _sourcepath;
	String _targetpath;
	/*const String& srcmediapath, const String& binmediapath );
	String _srcmediapath;
	//LookupTable<String,String*> debug_materials;
	String debug_models;
	*/
public:
	explicit Converter();
	~Converter();
public:
	void run();
private:
	bool shouldConvert( const String& srcpath, const String& trgpath );
	void compileShader( const String& vulkanLevel, const String& fn );
	void convertImage( const String& trgpath, const String& srcpath );
	void convertMesh( const String& name, const String& srcpath, const Mat4& transform );
	/*
	void copy( const String& srcpath, const String& trgpath );
	void convertCollection( const String& id, const String& srcpath );
	void buildCollectionTemplate( const String& trgpath, const String& id, const String& srcpath, const String& bincollpath );
	void convertSrcMedia();
	void convertSrcCollection( DictionaryVariant& json );

//
//	void convertMeshFile( const String& id, const String& filename );
protected:
//	String filename( const String& id ) const;
//	bool isModified( const String& id, const String& filename );
*/
};

#endif
