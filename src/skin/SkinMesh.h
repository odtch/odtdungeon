#pragma once

#include "SkinCore.h"

struct SkinVertex {
public:
	vec3 position; float filler00;
	vec3 normal; float filler10;
	vec2 texcoord; float filler20; float filler30;
	float bone_weights[3]; float filler40;
	int bone_indexes[3]; int filler50;
public:
	SkinVertex();
public:
	void addBoneWeight( int boneindex, float weight );
	float totalBoneWeight() const;
	void adjustBoneWeightsTo1();
public:
//	static ResourceType* GetResourceType();
};

#define SkinMesh Mesh<SkinVertex>

//class SkinMesh : public Mesh<SkinVertex>
//{
////public:
////	static Mesh<VertexPNT>* Convert( const Mesh<SkinVertex>& source );
////	static void Update( const Mesh<SkinVertex>& source, Mat4* matrices, Mesh<VertexPNT>& target );
//};


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
