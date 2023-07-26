#pragma once

#include "SkinCore.h"
//#include "renderer/Renderable.h"
//#include "resource/Mesh.h"

//class SkinRenderer : public Resource
//{
//private:
//	const Skin* _skin = null;
//	const SkinSkeleton* _skeleton = null;
//private:
//	struct MeshEntry {
//		const Mesh<SkinVertex>* source;
//		MeshPNT target;
//		RenderablePNT renderable;
//		SkinMesherJob* mesher = null;
//		bool _skeleton_modified = true;
//	};
//	List<MeshEntry*> _entries;
//private:
//public:
//	explicit SkinRenderer( const Skin* skin, const SkinSkeleton* skeleton, Material* material );
//	virtual ~SkinRenderer() override;
//public:
//	void setSkeletonModified();
//public:
//	void render( const PosOri& posori, Renderer& renderer );
//};





////class Skin_Renderable : public Renderable
////{
////private:
////	Skin* _skin;
////	Material* _material;
//////	SkinSkeleton* _skeleton;
//////	SkinAnimation* _animation;
//////	float _t;
////	List<Mesh<VertexPNT>*> _meshes;
////	List<Renderable*> _meshrenderables;
////////	struct Joint {
////////	public:
////////		Joint* parent;
////////		List<Joint*> childs;
////////	public:
////////		Particle* center;
////////		List<Particle*> axparticles;
////////	public:
////////		~Joint(){
////////			childs.removeAll();
////////			axparticles.removeAll();
////////		}
////////	};
////////	Joint* _root;
////////	List<Joint*> _joints;
////public:
////	explicit Skin_Renderable(
////			Skin* skin,
////			//const std::string& skin_filename, const std::string& anim_filename,
////			Material* material );
////	virtual ~Skin_Renderable() override;
////private:
//////	void loadSkinJoint( SkinJoint* joint, SkinJoint* parent, TestJoint* testparent );
//////	Joint* createJoint( ParticleMatrix* pm, SkinJoint* skin_joint, Joint* parent );
//////protected:
//////	virtual void animate( float dt ) override;
////public:
////	void setSkeleton( const SkinSkeleton& skeleton );
////public:
////	const List<Renderable*>& meshrenderables() const { return _meshrenderables; }
//////	virtual void render( Renderer& renderer ) override;
//////public:
//////	virtual bool isSkin() const override { return true; }
//////	virtual Skin_Renderable* asSkin() override { return this; }
////};
