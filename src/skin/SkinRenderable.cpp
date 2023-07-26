#include "stdodt.h"
#include "SkinRenderable.h"
//#include "Skin.h"
//#include "SkinMesh.h"
//#include "SkinMesher.h"

//SkinRenderer::SkinRenderer( const Skin* skin, const SkinSkeleton* skeleton, Material* material )
//	:_skin( skin )
//	,_skeleton( skeleton )
//{
//	assert( _skin );
//	assert( _skeleton );
//	assert( material );
//	for( Mesh<SkinVertex>* source : _skin->meshes() ){
//		MeshEntry* entry = new MeshEntry();
//		entry->source = source;
//		entry->renderable.setMesh( &entry->target );
//		entry->renderable.setMaterial( material );
//		//entry->mesher = new SkinMesherCpuJob();
//		entry->mesher = new SkinMesherGpuJob();
//		entry->mesher->initialize( _skin, _skeleton, entry->source, &entry->target );
//		_entries.add( entry );
//	}
//}
//SkinRenderer::~SkinRenderer(){
//	_entries.deleteAll();
//}
//void SkinRenderer::setSkeletonModified(){
//	for( MeshEntry* entry : _entries ){
//		entry->_skeleton_modified = true;
//	}
//}
//void SkinRenderer::render( const PosOri& posori, Renderer& renderer ){
//	for( MeshEntry* entry : _entries ){
//		if( entry->_skeleton_modified ){
//			if( entry->mesher->canStart() ){
//				entry->mesher->start();
//				entry->_skeleton_modified = false;
//			}
//		}
//	}
//	for( MeshEntry* entry : _entries ){
//		entry->mesher->refreshState();
//		renderer.add( posori, &entry->renderable );
//	}
//}


////Skin_Renderable::Skin_Renderable(Skin* skin, Material* material)
////	:_skin( skin )
////	,_material( material )
////{
////	for( auto mesh : _skin->meshes() ){
////		_meshes.add( SkinMesh::Convert( *mesh ) );
////	}
////	for( auto mesh : _meshes ){
////		_meshrenderables.add( new RenderablePNT( mesh, _material ) );
////	}
////}
////TestSkeleton::TestSkeleton( const std::string& skin_filename, const std::string& anim_filename, Material* material )
////	:_material( material )
//////	,_root( null )
////	,_t( 0 )
////{
////	Mat4 transform = Mat4::Scale( 0.01f );
////	{
////		AssImp assimp;
////		assimp.open( skin_filename );
////		//assimp.traceFile();
////		assimp._transform = transform;
////		_skin = assimp.loadSkin();
////		_skeleton = _skin->skeleton()->clone();//assimp.loadSkinSkeleton();
////		_skeleton->root()->recalcByRelativeMatrix();
////		Mat4 t = Mat4::RotationAtAxis( -90, Vec3::Right );// * Mat4::Scale( 0.01f );
////		_skeleton->root()->setAbsoluteMatrix( _skeleton->root()->absoluteMatrix() * t );
////	}
////	{
////		AssImp assimp;
////		assimp.open( anim_filename );
////		//assimp.traceFile();
////		assimp._transform = transform;
////		assert( assimp.animationCount() == 1 );
////		SkinSkeleton* _skeleton = assimp.loadSkin()->skeleton()->clone();//Skeleton();
////		_skeleton->root()->recalcByRelativeMatrix();
////		Mat4 t = Mat4::RotationAtAxis( -90, Vec3::Right );// * Mat4::Scale( 0.01f );
////		_skeleton->root()->setAbsoluteMatrix( _skeleton->root()->absoluteMatrix() * t );

////		_animation = assimp.loadAnimation( 0, *_skeleton );
////		{
////		}
////	}
////	std::cout << "\n\nskin: " << skin_filename << "\n";
////	for( auto j : _skin->skeleton()->joints() ){
////		if( j->influencesSkin() ){
////			std::cout << "  " << j->skinMatrixIndex() << " " << j->name() << "\n";
////		}
////	}
////}
////Skin_Renderable::~Skin_Renderable(){
////	_meshrenderables.deleteAll();
////	_meshes.deleteAll();
////	_skin = null;
////	_material = null;
////}
////void Skin_Renderable::setSkeleton( const SkinSkeleton& skeleton ){
////	static Mat4 skin_matrices[ 100 ];
////	static bool skin_matrix_valid[ 100 ];
////	for( int m = 0; m < 100; m++ ){
////		skin_matrix_valid[m] = false;
////	}
////	for( SkinJoint* tj : skeleton.joints() ){
////		//const SkinJoint* sj = _skin->skeleton()->getJointBySkinIndex( m );
////		const SkinJoint* sj = _skin->skeleton()->findJointByName( tj->name() );
////		if( sj ){
////			if( sj->influencesSkin() ){
////				int m = sj->skinMatrixIndex();
////				skin_matrices[ m ] = PosOri( tj->absoluteMatrix() ).matrix() * PosOri( sj->absoluteMatrix() ).matrix().inverted();
////				assert( !skin_matrix_valid[m] );
////				skin_matrix_valid[m] = true;
////			}
////		}
////	}
////	for( int mesh_index = 0; mesh_index < _skin->meshes().size(); mesh_index++ ){
////		const Mesh<SkinVertex>* source_mesh = _skin->meshes().get( mesh_index );
////		Mesh<VertexPNT>* target_mesh = _meshes.get( mesh_index );
////		SkinMesh::Update( *source_mesh, skin_matrices, *target_mesh );
////	}
////}
//////TestSkeleton::~TestSkeleton(){
//////	delete _animation;
//////	delete _skeleton;
//////	delete _skin;
//////	//	_joints.deleteAll();
//////}
//////void TestSkeleton::animate( float dt ){
//////	SceneObject::animate( dt );
//////	_t += dt;
//////	if( _animation->duration() < _t )_t = 0;
//////}
//////void TestSkeleton::render( Renderer& renderer ){
//////	if( !false ){
//////		const SkinSkeleton* skeleton = _animation->test( _t );
//////		for( auto renderable : _meshrenderables ){
//////			renderer.add( posori().matrix().asGlm(), renderable );
//////		}
//////	}
//////}



