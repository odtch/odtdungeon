#include "stdodt.h"
#include "Physics.h"
#include "OdtPhysics.h"
#include "window/Window.h"
#include "JoltImpl.h"
#include "PhysicsDebugRenderer.h"
//#include "renderer/Renderable.h"

#ifndef JPH_DEBUG_RENDERER
	// Hack to still compile DebugRenderer inside the test framework when Jolt is compiled without
	#define JPH_DEBUG_RENDERER
	#include <Jolt/Renderer/DebugRenderer.cpp>
	#undef JPH_DEBUG_RENDERER
#endif // !JPH_DEBUG_RENDERER

#ifdef JPH_DEBUG_RENDERER

static int lod_best = 0;

class PhysicsDebugRendererMesh : public JPH::RefTargetVirtual {
private:
	int _refcount = 0;
private:
	MeshPNT* _mesh = null;
public:
	explicit PhysicsDebugRendererMesh( MeshPNT* mesh );
	virtual ~PhysicsDebugRendererMesh() override;
public:
	MeshPNT* mesh() const { assert( _mesh ); return _mesh; }
public:
	virtual void AddRef() override;
	virtual void Release() override;
};
PhysicsDebugRendererMesh::PhysicsDebugRendererMesh( MeshPNT* mesh )
	:_mesh( asserted( mesh ) )
{
	//logDebug( "pdrm", this );
}
PhysicsDebugRendererMesh::~PhysicsDebugRendererMesh(){
	ASSERT( _mesh == null );
	//logDebug( "pdrm~", this );
}
void PhysicsDebugRendererMesh::AddRef(){
	_refcount++;
	//logDebug( "pdrm::ar", _refcount );
}
void PhysicsDebugRendererMesh::Release(){
	//logDebug( "pdrm::r", _refcount );
	_refcount--;
	if( _refcount <= 0 ){
		//logDebug( "PhysicsDebugRendererMesh::Release", _refcount, (void*) this );
		ASSERT( _mesh );
		odelete( _mesh );
		ASSERT( _mesh == null );
		delete this;
	}
}




//PhysicsDebugRenderer::PhysicsDebugRenderer( Physics* physics )
//	:_physics( physics )
//{
//	ASSERT( _physics );
//	Material* material = new Material();
//	//material->setFlag( MaterialFlag_CalcNormalFromTriangle );
//	//material->setFlag( MaterialFlag_UseNormalAsColor );
//	material->setFlag( MaterialFlag_NoLightAffected );
//	material->setFlag( MaterialFlag_NoShadowEmit );
//	//material->setColor( vec4( 0.5f, 0.0f, 0, 0.5f ) );
//	//material->setColor( vec4( 1, 0.0f, 0, 1 ) );
//	_materials.add( material );
////	_mesh.clear();
////	createLine( Vec3::Null, Vec3::Up * 100, vec4( 1, 1, 1, 1 ), 0.1f );
//	// Initialize base class
////	_renderables.add( new RenderablePNT( &_mesh, material ) );
//	DebugRenderer::Initialize();
//}

//PhysicsDebugRenderer::~PhysicsDebugRenderer()
//{
//	ASSERT( _area == null );
////	assert( _ren)
////_renderables.removeAll();;
////	_renderables.deleteAll();
//	for( auto material : _materials ){
//		if( material->hasTexture() )
//			delete material->texture();
//	}
//	_materials.deleteAll();
//}
//void PhysicsDebugRenderer::onAddedToScene( Scene* scene ){
//	SceneNode::onAddedToScene( scene );
//	assert( _area == null );
//	_area = area();
//}
//void PhysicsDebugRenderer::onRemovedFromScene( Scene* scene ){
//	assert( _area );
//	_area->removeRenderables( _renderables );
//	_renderables.deleteAll();
//	_area = null;
//	SceneNode::onRemovedFromScene( scene );
//}

//void PhysicsDebugRenderer::animate(float dt)
//{
//	static float time_to_next_lod = 0;
//	time_to_next_lod -= dt;
//	if( time_to_next_lod < 0 ){
//		time_to_next_lod = 0.1f;
//		lod_best++;
//		if( lod_best > 5 )
//			lod_best = 0;
//	}

//	static float time_to_next_update = 0 + 1;
//	time_to_next_update -= dt;
//	if( time_to_next_update < 0 ){
//		time_to_next_update = 0.9f + 0 * 0.2f;
//		//area()->removeRenderables( _renderables );
//		//_renderables.deleteAll();
//	} else if( time_to_next_update < 0.1f ){
//		area()->removeRenderables( _renderables );
//		_renderables.deleteAll();
//		JPH::BodyManager::DrawSettings ins;
//		//ins.mDrawShapeColor = JPH::BodyManager::EShapeColor::MotionTypeColor;
//		ins.mDrawShapeColor = JPH::BodyManager::EShapeColor::SleepColor;
//		//_mesh.clear();
//		_physics->_physics_system->DrawBodies( ins, this );
////		if( _mesh.isEmpty() )
////			createLine( Vec3::Null, Vec3::Up * 100, vec4( 1, 1, 1, 1 ), 0.1f );
//		ASSERT( _area );
//		ASSERT( area() == _area );
//		_area->addRenderables( _renderables );
////		int k = 0;
////		for( Renderable* r : _renderables ){
////			logDebug( "a", k , r->asPNT()->mesh()->calcBoundingBox().toString()
////					  , r->asPNT()->material()->color()
////					  );
////			k++;
////		}
//		time_to_next_update = 1;
//	}

//}
///*
//void PhysicsDebugRenderer::createLine(const Vec3& a, const Vec3& b, const glm::vec4& color, float radius )
//{
//	Vec3 o = b - a;
//	Vec3 d = o.normalized();
//	Vec3 c( d.z(), d.x(), d.y() );
//	Vec3 r = Vec3::Cross( d, c );
//	Vec3 u = Vec3::Cross( d, r );
//	Vec3 cv( color.x, color.y, color.z );
//	Vec2 t( 0, 0 );
//	uint au = _mesh.addVertex( VertexPNT( a + u * radius, cv, t ) );
//	uint ad = _mesh.addVertex( VertexPNT( a - u * radius, cv, t ) );
//	uint bu = _mesh.addVertex( VertexPNT( b + u * radius, cv, t ) );
//	uint bd = _mesh.addVertex( VertexPNT( b - u * radius, cv, t ) );
//	_mesh.addIndex( au, bu, bd, ad );
//	uint ar = _mesh.addVertex( VertexPNT( a + r * radius, cv, t ) );
//	uint al = _mesh.addVertex( VertexPNT( a - r * radius, cv, t ) );
//	uint br = _mesh.addVertex( VertexPNT( b + r * radius, cv, t ) );
//	uint bl = _mesh.addVertex( VertexPNT( b - r * radius, cv, t ) );
//	_mesh.addIndex( ar, br, bl, al );
//}
//*/
//void PhysicsDebugRenderer::DrawLine( JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor)
//{
//logDebug( "pdr::l" );
//}

//void PhysicsDebugRenderer::DrawTriangle(JPH::Vec3Arg inV1, JPH::Vec3Arg inV2, JPH::Vec3Arg inV3, JPH::ColorArg inColor)
//{
//	logDebug( "pdr::t" );

//}

//JPH::DebugRenderer::Batch PhysicsDebugRenderer::CreateTriangleBatch(const Triangle* inTriangles, int inTriangleCount)
//{
//	logDebug( "pdr::tb" );

//	MeshPNT* mesh = new MeshPNT();
//	PhysicsDebugRendererMesh* impl = new PhysicsDebugRendererMesh( mesh );
//	for( int i = 0; i < inTriangleCount; i++ ){
//		const Triangle& t = inTriangles[i];
//		for( int v = 0; v < 3; v++ ){
//			auto ve = t.mV[v];
//			uint vi = mesh->addVertex( VertexPNT(
//								 Vec3( ve.mPosition.x, ve.mPosition.y, ve.mPosition.z ),
//								 //Vec3( ve.mColor.r, ve.mColor.g, ve.mColor.b ) / 255,
//								 Vec3( ve.mNormal.x, ve.mNormal.y, ve.mNormal.z ),
//								 Vec2( 0, 0 )
//								 ) );
//			mesh->addIndex( vi );
//		}
//	}
//	return JPH::DebugRenderer::Batch( impl );
//}


//JPH::DebugRenderer::Batch PhysicsDebugRenderer::CreateTriangleBatch( const Vertex* inVertices, int inVertexCount, const JPH::uint32* inIndices, int inIndexCount ){
//	//logDebug( "pdr::tb2" );
//	//assert( false );
//	assert( inIndexCount % 3 == 0 );
//	MeshPNT* mesh = new MeshPNT();
//	PhysicsDebugRendererMesh* impl = new PhysicsDebugRendererMesh( mesh );
//	//= new RenderablePNT( mesh, _materials.first() );
//	for( int v = 0; v < inVertexCount; v++ ){
//		const Vertex& ve = inVertices[v];
//		//logDebug( "c", ve.mColor.r, ve.mColor.g, ve.mColor.b );
//		mesh->addVertex( VertexPNT(
//							 Vec3( ve.mPosition.x, ve.mPosition.y, ve.mPosition.z ) + Vec3::Right * 0.5f,
//							 //Vec3( ve.mColor.r, ve.mColor.g, ve.mColor.b ) / 255,
//							 Vec3( ve.mNormal.x, ve.mNormal.y, ve.mNormal.z ),
//							 Vec2( 0, 0 )
//							 ) );

//	}
//	for( int i = 0; i < inIndexCount; i++ ){
//		mesh->addIndex( inIndices[ i ] );
//	}
//	return JPH::DebugRenderer::Batch( impl );

//}
//void PhysicsDebugRenderer::DrawGeometry( JPH::Mat44Arg inModelMatrix, const JPH::AABox& inWorldSpaceBounds, float inLODScaleSq, JPH::ColorArg inModelColor, const GeometryRef& inGeometry, ECullMode inCullMode, ECastShadow inCastShadow, EDrawMode inDrawMode ){
//	//logDebug( "pdr::g", inGeometry->mLODs.size(), inLODScaleSq );
//	//static Material* material = null;
//	Material* material = _c_to_m.find( inModelColor.GetUInt32() );
//	if( material == null ){
//		material = new Material();
//		material->setFlag( MaterialFlag_CalcNormalFromTriangle );
//		//material->setFlag( MaterialFlag_UseNormalAsColor );
//		material->setFlag( MaterialFlag_NoLightAffected );
//		material->setFlag( MaterialFlag_NoShadowEmit );
//		_c_to_m.add( inModelColor.GetUInt32(), material );
//		_materials.add( material );
//		vec4 c( inModelColor.r, inModelColor.g, inModelColor.b, inModelColor.a );
//		c /= 255;
////		c[0] = 0;
////		c[1] = 0;
////		c[2] = 1;
//		//if( c[3] > 0.02f )
//			//c[3] = 0.02f;
//		material->setColor( c );
//	}
//	//for( int l = 0; l < inGeometry->mLODs.size(); l++ )
//	int l = lod_best;
//	if( l >= inGeometry->mLODs.size() )l = inGeometry->mLODs.size()-1;
//	{
//		JPH::DebugRenderer::LOD& lod = inGeometry->mLODs[ l ];
//		//logDebug( "pdr::g", l, lod.mDistance );
//		// Batch							mTriangleBatch;
//		// float							mDistance;
//		PhysicsDebugRendererMesh* rm = dynamic_cast<PhysicsDebugRendererMesh*>( lod.mTriangleBatch.GetPtr() );
//		assert( rm );
//		if( rm ){
//			auto r = new RenderablePNT( rm->mesh(), material );
//			/*PosOri po;
//			setPosOriFromJPH( po,
//							  //inModelMatrix.GetDirectionPreservingMatrix()*
//							  inModelMatrix
//							   );
//			r->setPosOri( po.matrix() );
//			*/
//			Mat4 m;
//			setMat4FromJPH( m, inModelMatrix );

//			r->setPosOri( Mat4::Translation( Vec3::Up * 0.01f ) * m );
//			//* Mat4::Scale( 0.5f ) );

//			_renderables.add( r );
//			return;
//		}

//	}

//	/*
//	JPH::Mat44Arg inModelMatrix,
//			const JPH::AABox& inWorldSpaceBounds,
//			float inLODScaleSq,
//			JPH::ColorArg inModelColor,
//			const GeometryRef& inGeometry,
//			ECullMode inCullMode, ECastShadow inCastShadow, EDrawMode inDrawMode)
//			*/

//}

//void PhysicsDebugRenderer::DrawText3D(JPH::Vec3Arg inPosition, const std::string_view& inString, JPH::ColorArg inColor, float inHeight)
//{
//	logDebug( "pdr::t" );

//}

///*
//void PhysicsDebugRenderer::DrawLine(const Float3 &inFrom, const Float3 &inTo, ColorArg inColor)
//{
//	lock_guard lock(mLinesLock);
//	mLines.push_back(Line(inFrom, inTo, inColor));
//}

//DebugRenderer::Batch PhysicsDebugRenderer::CreateTriangleBatch(const Triangle *inTriangles, int inTriangleCount)
//{
//	if (inTriangles == nullptr || inTriangleCount == 0)
//		return mEmptyBatch;

//	BatchImpl *primitive = new BatchImpl(mRenderer, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	primitive->CreateVertexBuffer(3 * inTriangleCount, sizeof(Vertex), inTriangles);

//	return primitive;
//}

//DebugRenderer::Batch PhysicsDebugRenderer::CreateTriangleBatch(const Vertex *inVertices, int inVertexCount, const uint32 *inIndices, int inIndexCount)
//{
//	if (inVertices == nullptr || inVertexCount == 0 || inIndices == nullptr || inIndexCount == 0)
//		return mEmptyBatch;

//	BatchImpl *primitive = new BatchImpl(mRenderer, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	primitive->CreateVertexBuffer(inVertexCount, sizeof(Vertex), inVertices);
//	primitive->CreateIndexBuffer(inIndexCount, inIndices);

//	return primitive;
//}

//void PhysicsDebugRenderer::DrawGeometry(Mat44Arg inModelMatrix, const AABox &inWorldSpaceBounds, float inLODScaleSq, ColorArg inModelColor, const GeometryRef &inGeometry, ECullMode inCullMode, ECastShadow inCastShadow, EDrawMode inDrawMode)
//{
//	lock_guard lock(mPrimitivesLock);
	   
//	// Our pixel shader uses alpha only to turn on/off shadows
//	Color color = inCastShadow == ECastShadow::On? Color(inModelColor, 255) : Color(inModelColor, 0);

//	if (inDrawMode == EDrawMode::Wireframe)
//	{
//		mWireframePrimitives[inGeometry].mInstances.push_back({ inModelMatrix, inModelMatrix.GetDirectionPreservingMatrix(), color, inWorldSpaceBounds, inLODScaleSq });
//		++mNumInstances;
//	}
//	else
//	{
//		if (inCullMode != ECullMode::CullFrontFace)
//		{
//			mPrimitives[inGeometry].mInstances.push_back({ inModelMatrix, inModelMatrix.GetDirectionPreservingMatrix(), color, inWorldSpaceBounds, inLODScaleSq });
//			++mNumInstances;
//		}

//		if (inCullMode != ECullMode::CullBackFace)
//		{
//			mPrimitivesBackFacing[inGeometry].mInstances.push_back({ inModelMatrix, inModelMatrix.GetDirectionPreservingMatrix(), color, inWorldSpaceBounds, inLODScaleSq });
//			++mNumInstances;
//		}
//	}
//}

//void PhysicsDebugRenderer::FinalizePrimitive()
//{
//	JPH_PROFILE_FUNCTION();

//	if (mLockedPrimitive != nullptr)
//	{
//		BatchImpl *primitive = static_cast<BatchImpl *>(mLockedPrimitive.GetPtr());

//		// Unlock the primitive
//		primitive->UnlockVertexBuffer();

//		// Set number of indices to draw
//		primitive->SetNumVtxToDraw(int(mLockedVertices - mLockedVerticesStart));

//		// Add to draw list
//		mTempPrimitives[new Geometry(mLockedPrimitive, mLockedPrimitiveBounds)].mInstances.push_back({ Mat44::sIdentity(), Mat44::sIdentity(), Color::sWhite, mLockedPrimitiveBounds, 1.0f });
//		++mNumInstances;

//		// Clear pointers
//		mLockedPrimitive = nullptr;
//		mLockedVerticesStart = nullptr;
//		mLockedVertices = nullptr;
//		mLockedVerticesEnd = nullptr;
//		mLockedPrimitiveBounds = AABox();
//	}
//}

//void PhysicsDebugRenderer::EnsurePrimitiveSpace(int inVtxSize)
//{
//	const int cVertexBufferSize = 10240;

//	if (mLockedPrimitive == nullptr
//		|| mLockedVerticesEnd - mLockedVertices < inVtxSize)
//	{
//		FinalizePrimitive();

//		// Create new
//		BatchImpl *primitive = new BatchImpl(mRenderer, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//		primitive->CreateVertexBuffer(cVertexBufferSize, sizeof(Vertex));
//		mLockedPrimitive = primitive;

//		// Lock buffers
//		mLockedVerticesStart = mLockedVertices = (Vertex *)primitive->LockVertexBuffer();
//		mLockedVerticesEnd = mLockedVertices + cVertexBufferSize;
//	}
//}

//void PhysicsDebugRenderer::DrawTriangle(Vec3Arg inV1, Vec3Arg inV2, Vec3Arg inV3, ColorArg inColor)
//{
//	lock_guard lock(mPrimitivesLock);

//	EnsurePrimitiveSpace(3);

//	// Set alpha to zero to tell our pixel shader to not cast shadows for this triangle
//	// this is because our algorithm only renders shadows for backfacing triangles and this
//	// triangle doesn't have one
//	Color color(inColor, 0);

//	// Construct triangle
//	new ((Triangle *)mLockedVertices) Triangle(inV1, inV2, inV3, color);
//	mLockedVertices += 3;

//	// Update bounding box
//	mLockedPrimitiveBounds.Encapsulate(inV1);
//	mLockedPrimitiveBounds.Encapsulate(inV2);
//	mLockedPrimitiveBounds.Encapsulate(inV3);
//}

//void PhysicsDebugRenderer::DrawInstances(const Geometry *inGeometry, const vector<int> &inStartIdx)
//{
//	RenderInstances *instances_buffer = mInstancesBuffer[mRenderer->GetCurrentFrameIndex()];

//	if (!inStartIdx.empty())
//	{
//		// Get LODs
//		const vector<LOD> &geometry_lods = inGeometry->mLODs;

//		// Write instances for all LODS
//		int next_start_idx = inStartIdx.front();
//		for (size_t lod = 0; lod < geometry_lods.size(); ++lod)
//		{
//			int start_idx = next_start_idx;
//			next_start_idx = inStartIdx[lod + 1];
//			int num_instances = next_start_idx - start_idx;
//			instances_buffer->Draw(static_cast<BatchImpl *>(geometry_lods[lod].mTriangleBatch.GetPtr()), start_idx, num_instances);
//		}
//	}
//}

//void PhysicsDebugRenderer::DrawText3D(Vec3Arg inPosition, const string &inString, ColorArg inColor, float inHeight)
//{
//	lock_guard lock(mTextsLock);
//	mTexts.emplace_back(inPosition, inString, inColor, inHeight);
//}

//void PhysicsDebugRenderer::DrawLines()
//{
//	JPH_PROFILE_FUNCTION();

//	lock_guard lock(mLinesLock);

//	// Draw the lines
//	if (!mLines.empty())
//	{
//		RenderPrimitive primitive(mRenderer, D3D_PRIMITIVE_TOPOLOGY_LINELIST);
//		primitive.CreateVertexBuffer((int)mLines.size() * 2, sizeof(Line) / 2);
//		void *data = primitive.LockVertexBuffer();
//		memcpy(data, &mLines[0], mLines.size() * sizeof(Line));
//		primitive.UnlockVertexBuffer();
//		mLineState->Activate();
//		primitive.Draw();
//	}
//}

//void PhysicsDebugRenderer::DrawTriangles()
//{
//	JPH_PROFILE_FUNCTION();

//	lock_guard lock(mPrimitivesLock);

//	// Finish the last primitive
//	FinalizePrimitive();

//	// Render to shadow map texture first
//	mRenderer->SetRenderTarget(mDepthTexture);

//	// Clear the shadow map texture to max depth
//	mDepthTexture->ClearRenderTarget();

//	// Get the camera and light frustum for culling
//	Vec3 camera_pos = mRenderer->GetCameraState().mPos;
//	const Frustum &camera_frustum = mRenderer->GetCameraFrustum();
//	const Frustum &light_frustum = mRenderer->GetLightFrustum();

//	// Resize instances buffer and copy all visible instance data into it
//	if (mNumInstances > 0)
//	{
//		// Create instances buffer
//		RenderInstances *instances_buffer = mInstancesBuffer[mRenderer->GetCurrentFrameIndex()];
//		instances_buffer->CreateBuffer(2 * mNumInstances, sizeof(Instance));
//		Instance *dst_instance = reinterpret_cast<Instance *>(instances_buffer->Lock());

//		// Next write index
//		int dst_index = 0;

//		// This keeps track of which instances use which lod, first array: 0 = light pass, 1 = geometry pass
//		vector<vector<int>> lod_indices[2];

//		for (InstanceMap *primitive_map : { &mPrimitives, &mTempPrimitives, &mPrimitivesBackFacing, &mWireframePrimitives })
//			for (InstanceMap::value_type &v : *primitive_map)
//			{
//				// Get LODs
//				const vector<LOD> &geometry_lods = v.first->mLODs;
//				size_t num_lods = geometry_lods.size();
//				JPH_ASSERT(num_lods > 0);

//				// Ensure that our lod index array is big enough (to avoid reallocating memory too often)
//				if (lod_indices[0].size() < num_lods)
//					lod_indices[0].resize(num_lods);
//				if (lod_indices[1].size() < num_lods)
//					lod_indices[1].resize(num_lods);

//				// Iterate over all instances
//				const vector<InstanceWithLODInfo> &instances = v.second.mInstances;
//				for (size_t i = 0; i < instances.size(); ++i)
//				{
//					const InstanceWithLODInfo &src_instance = instances[i];

//					// Check if it overlaps with the light or camera frustum
//					bool light_overlaps = light_frustum.Overlaps(src_instance.mWorldSpaceBounds);
//					bool camera_overlaps = camera_frustum.Overlaps(src_instance.mWorldSpaceBounds);
//					if (light_overlaps || camera_overlaps)
//					{
//						// Figure out which LOD to use
//						float dist_sq = src_instance.mWorldSpaceBounds.GetSqDistanceTo(camera_pos);
//						for (size_t lod = 0; lod < num_lods; ++lod)
//							if (dist_sq <= src_instance.mLODScaleSq * Square(geometry_lods[lod].mDistance))
//							{
//								// Store which index goes in which LOD
//								if (light_overlaps)
//									lod_indices[0][lod].push_back((int)i);
//								if (camera_overlaps)
//									lod_indices[1][lod].push_back((int)i);
//								break;
//							}
//					}
//				}

//				// Loop over both passes: 0 = light, 1 = geometry
//				vector<int> *start_idx[] = { &v.second.mLightStartIdx, &v.second.mGeometryStartIdx };
//				for (int type = 0; type < 2; ++type)
//				{
//					// Reserve space for instance indices
//					vector<int> &type_start_idx = *start_idx[type];
//					type_start_idx.resize(num_lods + 1);

//					// Write out geometry pass instances
//					for (size_t lod = 0; lod < num_lods; ++lod)
//					{
//						// Write start index for this LOD
//						type_start_idx[lod] = dst_index;

//						// Copy instances
//						vector<int> &this_lod_indices = lod_indices[type][lod];
//						for (int i : this_lod_indices)
//						{
//							const Instance &src_instance = instances[i];
//							dst_instance[dst_index++] = src_instance;
//						}

//						// Prepare for next iteration (will preserve memory)
//						this_lod_indices.clear();
//					}

//					// Write out end of last LOD
//					type_start_idx.back() = dst_index;
//				}
//			}

//		instances_buffer->Unlock();
//	}

//	if (!mPrimitives.empty() || !mTempPrimitives.empty())
//	{
//		// Front face culling, we want to render the back side of the geometry for casting shadows
//		mShadowStateFF->Activate();

//		// Draw all primitives as seen from the light
//		if (mNumInstances > 0)
//			for (InstanceMap::value_type &v : mPrimitives)
//				DrawInstances(v.first, v.second.mLightStartIdx);
//		for (InstanceMap::value_type &v : mTempPrimitives)
//			DrawInstances(v.first, v.second.mLightStartIdx);
//	}

//	if (!mPrimitivesBackFacing.empty())
//	{
//		// Back face culling, we want to render the front side of back facing geometry
//		mShadowStateBF->Activate();

//		// Draw all primitives as seen from the light
//		for (InstanceMap::value_type &v : mPrimitivesBackFacing)
//			DrawInstances(v.first, v.second.mLightStartIdx);
//	}

//	if (!mWireframePrimitives.empty())
//	{
//		// Switch to wireframe mode
//		mShadowStateWire->Activate();

//		// Draw all wireframe primitives as seen from the light
//		for (InstanceMap::value_type &v : mWireframePrimitives)
//			DrawInstances(v.first, v.second.mLightStartIdx);
//	}

//	// Switch to the main render target
//	mRenderer->SetRenderTarget(nullptr);

//	// Bind the shadow map texture
//	mDepthTexture->Bind(2);

//	if (!mPrimitives.empty() || !mTempPrimitives.empty())
//	{
//		// Bind the normal shader, back face culling
//		mTriangleStateBF->Activate();

//		// Draw all primitives
//		if (mNumInstances > 0)
//			for (InstanceMap::value_type &v : mPrimitives)
//				DrawInstances(v.first, v.second.mGeometryStartIdx);
//		for (InstanceMap::value_type &v : mTempPrimitives)
//			DrawInstances(v.first, v.second.mGeometryStartIdx);
//	}

//	if (!mPrimitivesBackFacing.empty())
//	{
//		// Front face culling, the next batch needs to render inside out
//		mTriangleStateFF->Activate();

//		// Draw all back primitives
//		for (InstanceMap::value_type &v : mPrimitivesBackFacing)
//			DrawInstances(v.first, v.second.mGeometryStartIdx);
//	}

//	if (!mWireframePrimitives.empty())
//	{
//		// Wire frame mode
//		mTriangleStateWire->Activate();

//		// Draw all wireframe primitives
//		for (InstanceMap::value_type &v : mWireframePrimitives)
//			DrawInstances(v.first, v.second.mGeometryStartIdx);
//	}
//}

//void PhysicsDebugRenderer::DrawTexts()
//{
//	lock_guard lock(mTextsLock);

//	JPH_PROFILE_FUNCTION();

//	const CameraState &camera_state = mRenderer->GetCameraState();

//	for (const Text &t : mTexts)
//	{
//		Vec3 forward = camera_state.mForward;
//		Vec3 right = forward.Cross(camera_state.mUp).Normalized();
//		Vec3 up = right.Cross(forward).Normalized();
//		Mat44 transform(Vec4(right, 0), Vec4(up, 0), Vec4(forward, 0), Vec4(t.mPosition, 1));

//		mFont->DrawText3D(transform * Mat44::sScale(t.mHeight), t.mText, t.mColor);
//	}
//}

//void PhysicsDebugRenderer::Draw()
//{
//	DrawLines();
//	DrawTriangles();
//	DrawTexts();
//}

//void PhysicsDebugRenderer::ClearLines()
//{
//	lock_guard lock(mLinesLock);
//	mLines.clear();
//}

//void PhysicsDebugRenderer::ClearMap(InstanceMap &ioInstances)
//{
//	vector<GeometryRef> to_delete;
	
//	for (InstanceMap::value_type &kv : ioInstances)
//	{
//		if (kv.second.mInstances.empty())
//			to_delete.push_back(kv.first);
//		else
//			kv.second.mInstances.clear();
//	}

//	for (GeometryRef &b : to_delete)
//		ioInstances.erase(b);
//}

//void PhysicsDebugRenderer::ClearTriangles()
//{
//	lock_guard lock(mPrimitivesLock);

//	// Close any primitive that's being built
//	FinalizePrimitive();
	
//	// Move primitives to draw back to the free list
//	ClearMap(mWireframePrimitives);
//	ClearMap(mPrimitives);
//	mTempPrimitives.clear(); // These are created by FinalizePrimitive() and need to be cleared every frame
//	ClearMap(mPrimitivesBackFacing);
//	mNumInstances = 0;
//}

//void PhysicsDebugRenderer::ClearTexts()
//{
//	lock_guard lock(mTextsLock);
//	mTexts.clear();
//}

//void PhysicsDebugRenderer::Clear()
//{
//	ClearLines();
//	ClearTriangles();
//	ClearTexts();
//}
//*/

#endif
