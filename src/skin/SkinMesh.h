#pragma once

#include "SkinCore.h"

struct SkinBoneType {
public:
	int index = -1;
	String name;
	String jointname;
public:
	Mat4 bone2mesh;
};

class SkinType {
private:
	SkinMesh* _mesh = null;
private:
	List<SkinBoneType*> _bones;
public:
	explicit SkinType();
	~SkinType();
public:
	const SkinMesh& mesh() const{ ASSERT( _mesh ); return *_mesh; }
	SkinMesh& mesh(){ ASSERT( _mesh ); return *_mesh; }
public:
	const List<SkinBoneType*>& bones() const{ return _bones; }
public:
	void load( BinaryFileReader& reader );
	void save( BinaryFileWriter& writer ) const;
public:
	friend class SkinImporter;
};
