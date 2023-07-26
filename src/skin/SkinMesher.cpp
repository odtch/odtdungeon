#include "stdodt.h"
#include "SkinMesher.h"
//#include "Skin.h"
#include "vulkan/resource/VulkanMesh.h"
#include "vulkan/resource/VulkanMeshBuffer.h"
#include "vulkan/device/VulkanDevice.h"
//#include "window/App.h"

////SkinMesherJob::SkinMesherJob(){
////}
////SkinMesherJob::~SkinMesherJob(){
////}
////void SkinMesherJob::initialize( const Skin* skin, const SkinSkeleton* skeleton, const Mesh<SkinVertex>* source, Mesh<VertexPNT>* target ){
////	assert( skin ); assert( skeleton ); assert( source ); assert( target );
////	assert( _skin == null );
////	_skin = skin;
////	_skeleton = skeleton;
////	_source = source;
////	_target = target;
////	assert( _target->isEmpty() );
////	for( uint v = 0; v < _source->vertexCount(); v++ ){
////		const SkinVertex& vertex = _source->vertex( v );
////		_target->addVertex( VertexPNT( vertex.position, vertex.normal, vertex.texcoord ) );
////	}
////	for( uint i = 0; i < _source->indexCount(); i++ ){
////		_target->addIndex( _source->index( i ) );
////	}
////}
////void SkinMesherJob::loadSkeleton( const SkinSkeleton& skeleton ){
////	_skin_matrix_count = 0;
////	for( int m = 0; m < 100; m++ ){
////		_skin_matrix_valid[m] = false;
////	}
//////	for( SkinJoint* joint : skeleton.joints() ){
//////		if( joint->influencesSkin() ){
//////			uint m = joint->skinMatrixIndex();
//////			if( _skin_matrix_count <= m )_skin_matrix_count = m+1;
//////			_skin_matrices[ m ] = PosOri( tj->absoluteMatrix() ).matrix() * PosOri( sj->absoluteMatrix() ).matrix().inverted();
//////			assert( !_skin_matrix_valid[m] );
//////			_skin_matrix_valid[m] = true;
//////		}
//////	}
////	for( SkinJoint* tj : skeleton.joints() ){
////		//const SkinJoint* sj = _skin->skeleton()->getJointBySkinIndex( m );
////		const SkinJoint* sj = _skin->skeleton()->findJointByName( tj->name() ); int todo_optimize_achtung_skin_inv_verwenden;
////		if( sj ){
////			if( sj->influencesSkin() ){
////				assert( tj->skinMatrixIndex() == sj->skinMatrixIndex() );
////				uint m = sj->skinMatrixIndex();
////				if( _skin_matrix_count <= m )_skin_matrix_count = m+1;
////				_skin_matrices[ m ] = PosOri( tj->absoluteMatrix() ).matrix() * PosOri( sj->absoluteMatrix() ).matrix().inverted();
////				assert( !_skin_matrix_valid[m] );
////				_skin_matrix_valid[m] = true;
////			}
////		}
////	}
////}

////SkinMesherCpuJob::SkinMesherCpuJob(){
////}
////SkinMesherCpuJob::~SkinMesherCpuJob(){
////}
////JobQueue* SkinMesherCpuJob::detect_queue( JobScheduler& scheduler ){
////	return scheduler.intervall_queue( "SkinMesherCpu", 0.3f );
////}
////void SkinMesherCpuJob::run(){
////	//std::cout << "SkinMesherCpuJob::run\n";
////	assert( _skin );
////	loadSkeleton( *_skeleton );
////	for( uint v = 0; v < _source->vertexCount(); v++ ){
////		const SkinVertex& source_vertex = _source->vertex( v );
////		VertexPNT& target_vertex = _target->vertex( v );
////		target_vertex.position = vec3( 0, 0, 0 );
////		target_vertex.normal = vec3( 0, 0, 0 );
////		for( int bi = 0; bi < 3; bi++ ){
////			int bone_index = source_vertex.bone_indexes[ bi ];
////			assert( _skin_matrix_valid[ bone_index ] );
////			const Mat4& mat = _skin_matrices[ bone_index ];
////			target_vertex.position += ( ( mat.map      ( Vec3( source_vertex.position ) ) ) * source_vertex.bone_weights[ bi ] ).asGlm();
////			target_vertex.normal   += ( ( mat.mapNormal( Vec3( source_vertex.normal   ) ) ) * source_vertex.bone_weights[ bi ] ).asGlm();
////		}
////		target_vertex.normal = glm::normalize( target_vertex.normal );
////	}
////	_target->setModified();
////	setCompleted();
////}
////void SkinMesherCpuJob::refreshState(){
////}

////SkinMesherGpuJob::SkinMesherGpuJob(){
////}
////SkinMesherGpuJob::~SkinMesherGpuJob(){
////	if( _executor ){
////		_executor->destroy();
////		delete _executor;
////		_executor = null;
////	}
////}
////JobQueue* SkinMesherGpuJob::detect_queue( JobScheduler& scheduler ){
////	//return scheduler.intervall_queue( "SkinMesherGpu", ( 1.0f / 60 ) / 2 );
////	return scheduler.parallel_queue( "SkinMesherGpu", 3 );
////}
////void SkinMesherGpuJob::run(){
////	//std::cout << "SkinMesherGpuJob::run\n";
////	assert( _skin );
////	loadSkeleton( *_skeleton );
////	assert( _executor == null );
////	_executor = new SkinMesherExecutor();
////	_executor->create( (Mesh<SkinVertex>*) _source, _target );
////	_executor->start( _skin_matrices, _skin_matrix_count );
////}
////void SkinMesherGpuJob::refreshState(){
////	if( _executor ){
////		if( _executor->isCompleted() ){
////			_executor->destroy();
////			delete _executor;
////			_executor = null;
////			_target->setModified( AbstractMesh::ModifiedInGPU );
////			setCompleted();
////		}
////	}
////}


//SkinMesherProgram* SkinMesherProgram::_skinmesherProgram( null );

//SkinMesherProgram* SkinMesherProgram::Get(){
//	assert( _skinmesherProgram );
////	if( _skinmesherProgram == null ){
////		_skinmesherProgram = new SkinMesherProgram();
////		Singleton::Get<VulkanDevice>()->addObjectToDestroy( _skinmesherProgram );
////		_skinmesherProgram->create( Singleton::Get<VulkanDevice>() );
////	}
//	return _skinmesherProgram;
//}

//SkinMesherProgram::SkinMesherProgram(){
//}
//SkinMesherProgram::~SkinMesherProgram(){
//}
//void SkinMesherProgram::create( VulkanDevice* device, VulkanTask* renderTask ){
//	assert( device ); assert( renderTask );
//	assert( _skinmesherProgram == null );
//	_skinmesherProgram = this;
//	_renderTask = renderTask;
//	device->addObjectToDestroy( this );
//	VulkanComputeProgram::create( App::Get()->binpath() + "/media/skinmesher.comp.glsl.spv", 20, device );
//}
//void SkinMesherProgram::destroy(){
//	assert( _skinmesherProgram == this );
//	_skinmesherProgram = null;
//	_renderTask = null;
//	VulkanComputeProgram::destroy();
//}
//void SkinMesherProgram::addBindings( VulkanDescriptorSetLayout& descriptorSetLayout ){
//	descriptorSetLayout.addBinding( 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_COMPUTE_BIT, 0 );
//	descriptorSetLayout.addBinding( 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_COMPUTE_BIT, 0 );
//	descriptorSetLayout.addBinding( 2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_COMPUTE_BIT, 0 );
//}

//SkinMesherExecutor::SkinMesherExecutor()
//	:_source_mesh( null )
//	,_target_mesh( null )
//{
//	memset( &_matrices, 0, sizeof( _matrices ) );
//}
//SkinMesherExecutor::~SkinMesherExecutor(){
//}
//void SkinMesherExecutor::create( Mesh<SkinVertex>* source_mesh, Mesh<VertexPNT>* target_mesh ){
//	assert( source_mesh != null && _source_mesh == null );
//	assert( target_mesh != null && _target_mesh == null );
//	SkinMesherProgram* program = SkinMesherProgram::Get();
//	_program = asserted( program );
//	_source_mesh = source_mesh;
//	_target_mesh = target_mesh;
//	_matricesBuffer.create( sizeof( _matrices ),
//		VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
//		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
//		program->commandPool().device()
//	);
//	if( !_source_mesh->hasVulkanMesh() ){
//		assert( false );
//	//		VulkanMeshBuffer* vulkanMeshBuffer = new VulkanMeshBuffer();
//	//	_source_mesh->setVulkanMesh( );
//	//		vulkanMeshBuffer->load<SkinVertex>( commandPool(), *_source_mesh,
//	//								VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
//	//											VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT
//	//								);
//	//		_source_mesh->setVulkanMeshBuffer( vulkanMeshBuffer );
//	}
//	//	VulkanMeshBuffer* source_vulkanMeshBuffer = _source_mesh->vulkanMeshBuffer();
//	if( !_target_mesh->hasVulkanMesh() ){
//		assert( false );
//	}
//	//		VulkanMeshBuffer* vulkanMeshBuffer = new VulkanMeshBuffer();
//	//		vulkanMeshBuffer->load<VertexPNT>( commandPool(), *_target_mesh,
//	//								VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
//	// VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
//	//								VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT |	VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | 		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT
//	//								);
//	//		_target_mesh->setVulkanMeshBuffer( vulkanMeshBuffer );
//	//	}
//////	vec2 inputData[ 100 ];
//////	_inputBuffer.create( sizeof( inputData ),
//////						 VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
//////						 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
//////						 program->commandPool().device()
//////					 );
//////	_inputBuffer.load( program->commandPool(), inputData, sizeof( inputData ) );
////	vec2 outputData[ 100 ];
////	_outputBuffer.create( sizeof( outputData ),
////						 VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
////						 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
////						 program->commandPool().device()
////					 );
//	VulkanComputeExecutor::create( program, Vec3i( _source_mesh->vertexCount(), 1, 1 ) );
//}
//void SkinMesherExecutor::destroy(){
//	VulkanComputeExecutor::destroy();
//	_matricesBuffer.clear(); // .destroy();
////	if( _source_mesh->hasVulkanMeshBuffer() ){
////		VulkanMeshBuffer* vulkanMeshBuffer = _source_mesh->vulkanMeshBuffer();
////		vulkanMeshBuffer->destroy();
////		delete vulkanMeshBuffer;
////		_source_mesh->setVulkanMeshBuffer( null );
////	}
////	//_inputBuffer.destroy();
////	if( _target_mesh->hasVulkanMeshBuffer() ){
////		auto vmb = _target_mesh->vulkanMeshBuffer();
////		vmb->destroy();
////		delete vmb;
////		_target_mesh->setVulkanMeshBuffer( null );
////	}
////	_outputBuffer.destroy();
//	_source_mesh = null;
//	_target_mesh = null;
//}
//void SkinMesherExecutor::bind( VulkanDescriptorSet& descriptorSet ){
//	descriptorSet.bindBuffer( 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, _matricesBuffer );
//	VulkanMeshDataPointer& c = _source_mesh->vulkanMesh()->current();
//	assert( !c.isNull() );
//	c.data();
//	assert( c.data()->hasVulkanMeshBuffer() );
//	VulkanMeshBuffer* vulkanMeshBuffer = c.data()->vulkanMeshBuffer();
//	VulkanBuffer& source_buffer = vulkanMeshBuffer->vertexBuffer();
//	descriptorSet.bindBuffer( 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, source_buffer );
//	descriptorSet.bindBuffer( 2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, _target_mesh->vulkanMesh()->current().data()->vulkanMeshBuffer()->vertexBuffer() );
//}
//void SkinMesherExecutor::start( const Mat4* matrices, uint matrixCount ){
////	_matrices.count++;
////	if( 10 < _matrices.count )_matrices.count = 0;
//	_matrices.count = matrixCount;
//	assert( matrixCount <= MaxMatricesCount );
//	for( uint m = 0; m < matrixCount; m++ ){
//		_matrices.data[m] = matrices[m];
//	}
//////	_matrices.count2 = 10 - _matrices.count;
////	_matrices.data[0] = Mat4::Translation( Vec3( 1, 10, 100 ) );
////	_matrices.data[1] = Mat4::Translation( Vec3( 2, 20, 200 ) );
////	_matrices.data[2] = Mat4::Translation( Vec3( 3, 30, 300 ) );
////	for( int i = 0; i <= 3; i++ ){
//////		Mat4 m = _matrices.data[i];
//////		std::cout << "  i " << i
//////				  << " " << m.asGlm()[0][0]
//////				<< " " << m.asGlm()[3][0]
//////				<< " " << m.asGlm()[3][1]
//////				<< " " << m.asGlm()[3][2]
//////				<< " " << m.asGlm()[3][3]
//////				<< "\n";
//////		const SkinVertex& v = _source_mesh->vertex( i );
//////		std::cout << "hst " << i
//////				  << " " << v.position.x << " " << v.position.y << " " << v.position.z
//////					 << " " << v.normal.x << " " << v.normal.y << " " << v.normal.z
//////				  << " " << v.bone_indexes[0] << " " << v.bone_indexes[1] << "\n";
////		const VertexPNT& v = _target_mesh->vertex( i );
////		std::cout << "hst " << i
////				  << " " << v.position.x << " " << v.position.y << " " << v.position.z
////					 << " " << v.normal.x << " " << v.normal.y << " " << v.normal.z
////				  << "\n";
////	}
//////	((float&)_matrices.data[0].asGlm()[0][0]) = 5;
//////	((float&)_matrices.data[1].asGlm()[0][0]) = 50;
////	_matrices.a = 123;
////	_matrices.b = 321;
//	_matricesBuffer.load( commandPool(), &_matrices, sizeof( _matrices ) );
//	//_program->renderTask()->addWaitFor( &this->completedSemaphore() );
//	VulkanComputeExecutor::start();
//}



