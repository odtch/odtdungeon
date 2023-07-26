#include "stdodt.h"
#include "SkinRenderer.h"
#include "SkinMesh.h"
#include "SkinMesher.h"
#include "renderer/Renderer.h"
#include "vulkan/raytracer/VulkanRaytracer.h"

//SkinRenderable::SkinRenderable( Skin* skin )
//	:RenderablePNT( skin->mesh(), skin->material() )
//	,_skin( skin )
//{
//}
//SkinRenderable::~SkinRenderable(){
//	if( _cpu_mesher ){
//		logDebug( "Skin::onRemovedFromScene, cpu_mesher is", _cpu_mesher->stateAsString() );
//		//_cpu_mesher->requestStop();
//		//_cpu_mesher->stop();
//	}
//	if( _gpu_mesher ){
//		while( _gpu_mesher->isRunning() ){
//			logDebug( "Skin::onRemovedFromScene, gpu mesher is still running" );
//		}
//		_gpu_mesher->destroy();
//		odelete( _gpu_mesher );
//	}
//	if( _cpu_mesher ){
//		_cpu_mesher->stop();
//		odelete( _cpu_mesher );
//	}
//	ASSERT( _gpu_mesher == null );
//}
//void SkinRenderable::render( Renderer& renderer ){
//	if( _gpu_mesher == null ){
//	} else if( _gpu_mesher->isRunning() ){
//	} else if( _gpu_mesher->isCompleted() ){
//	}

////	} else if( _gpu_mesher->isCompleted() && _gpu_mesher_waiting == false ){
////		_gpu_mesher_waiting = true;
////		logDebug( "set modi in gpu" );
////		_skin->_mesh.setModified( AbstractMesh::ModifiedInGPU );
////	} else if( _gpu_mesher->isCompleted() && _gpu_mesher_waiting == true ){
////		if( _skin->_mesh.isModified() ){
////			logDebug( "still modi" );
////		} else {
////			if( _skin->_bones_modified ){
////				_gpu_mesher_waiting = false;
////				_bones_modified = false;
////				_gpu_mesher->start( _skin->_bone_matrix, _skin->type()->bones().size() );
////				raytracer.renderTask().addWaitFor( _gpu_mesher->completedSemaphore() );
////			}
////		}
////	} else {
////		logError( "GpuSkinner unexpected state" );
////	}
////	if( _gpu_mesher ){
////		if( _gpu_mesher->isRunning() ){
////		}
////	}
//	if( _skin->isBonesModified() ){
//		skipc--;
//		if( skipc > 0 ){
//			return;
//		}
////		Random rnd;
////		skipc = rnd.geti( 10, 40 );
//		//mapMeshOnCPU();
//		//mapMeshOnCPUasTask();
//		mapMeshOnGPU( *renderer.asVulkanRaytracer() );
//	}
////	if( _cpu_mesher ){
////		while( _cpu_mesher->isRunning() ){
////			//logDebug( "Skin::animate cpu mesher is running" );

////		}
////	}
////	if( renderer.isVulkanRaytracer() ){
////		VulkanRaytracer* raytracer = renderer.asVulkanRaytracer();
////		assert( false );
////	} else {
////		assert( false );
//	//	}
//}
//void SkinRenderable::mapMeshOnCPU(){
//	Vec3 position;
//	Vec3 normal;
//	for( uint v = 0; v < _skin->type()->mesh().vertexCount(); v++ ){
//		const SkinVertex& src = _skin->type()->mesh().vertex( v );
//		VertexPNT& vertex = _skin->mesh()->vertex( v );
//		position.clear();
//		normal.clear();
//		for( int b = 0; b < 3; b++ ){
//			int bi = src.bone_indexes[ b ];
//			if( 0 <= bi ){
//				assert( bi < _skin->type()->bones().size() );
//				position += ( _skin->_bone_matrix[ bi ].map( Vec3( src.position ) ) * src.bone_weights[ b ] );
//				normal += ( _skin->_bone_matrix[ bi ].mapNormal( Vec3( src.normal ) ) * src.bone_weights[ b ] );
//			}
//		}
//		normal.normalize();
//		vertex.position = position.asGlm();
//		vertex.normal = normal.asGlm();
//	}
//	_skin->_bones_modified = false;
//	_skin->_mesh.setModified();
//}
//void SkinRenderable::mapMeshOnCPUasTask(){
//	if( _cpu_mesher == null ){
//		_cpu_mesher = new Task();
//		_cpu_mesher->start( "SkinMesher", [this]( TaskToken& token ){ mapMeshOnCPU(); } );
//	} else if( _cpu_mesher->isRunning() ){
//	} else if( _cpu_mesher->isCompleted() ){
//		_cpu_mesher->start( "SkinMesher", [this]( TaskToken& token ){ mapMeshOnCPU(); } );
//	} else if( _cpu_mesher->isStarting() ){
//	} else {
//		assert( false );
//	}
//}
//void SkinRenderable::mapMeshOnGPU( VulkanRaytracer& raytracer ){
//	if( !_skin->type()->mesh().hasVulkanMesh() ){
//		logDebug( "Skin.source has no vulkan mesh" );
//		return;
//	}
//	if( !_skin->_mesh.hasVulkanMesh() ){
//		logDebug( "Skin.target has no vulkan mesh" );
//		return;
//	}
//	if( _gpu_mesher == null ){
//		_gpu_mesher = new SkinMesherExecutor();
//		_gpu_mesher->create( &_skin->type()->mesh(), &_skin->_mesh );
//		_gpu_mesher_waiting = false;
//		_skin->_bones_modified = false;
//	} else if( _gpu_mesher->isRunning() ){
//		assert( false );
//		return;
//	} else {
//		assert( _gpu_mesher->isCompleted() );
//	}

////	logDebug( "SkinGpu.aw", raytracer.renderTask().isInitializing(), raytracer.renderTask().isRunning(), raytracer.renderTask().isCompleted()
////			  , raytracer.renderTask().completedSemaphore().vkSemaphore() );
//	//_gpu_mesher->commandBuffer().addWait( raytracer.renderTask().completedSemaphore() );

////	logDebug( "SkinGpu.start", raytracer.renderTask().completedSemaphore().vkSemaphore() );
//	_gpu_mesher->start( _skin->_bone_matrix, _skin->type()->bones().size() );
//	//raytracer.buildTLASTask().addWaitFor( _gpu_mesher->completedSemaphore() );
//	//raytracer.loadDataTask().addWaitFor( _gpu_mesher->completedSemaphore() );
//	//raytracer.renderTask().addWaitFor( _gpu_mesher->completedSemaphore() );
//	_skin->_bones_modified = false;
//	_skin->_mesh.setModified( AbstractMesh::ModifiedInGPU );
//}

//SkinRenderer::SkinRenderer( Skin* skin, Object* object )
//	:ObjectRenderableProperty( new SkinRenderable( skin ), object )
//	//asserted( skin )->mesh(), asserted( skin )->material(), object )
//	,_skin( asserted( skin ) )
//{
//}
//SkinRenderer::~SkinRenderer(){
//}

//SkinSkeletonRenderer::SkinSkeletonRenderer( Skeleton* skeleton, Skin* skin, Object* object )
//	:SkinRenderer( skin, object )
//	,_skeleton( asserted( skeleton ) )
//{
//}
//SkinSkeletonRenderer::~SkinSkeletonRenderer(){
//	odelete( _skeleton );
//}
//void SkinSkeletonRenderer::animate( float dt ){
//	SkinRenderer::animate( dt );
//	skin()->loadBones( *_skeleton );
//}

