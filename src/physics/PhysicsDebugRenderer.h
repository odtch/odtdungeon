#pragma once

#include "PhysicsCore.h"
#include "utils/LookupTable.h"
#include "JoltImpl.h"
#include "Physics.h"
#include <Jolt/Core/FixedSizeFreeList.h>

#ifdef JPH_DEBUG_RENDERER
	#include <Jolt/Renderer/DebugRenderer.h>
#else
//	// Hack to still compile DebugRenderer inside the test framework when Jolt is compiled without
//	#define JPH_DEBUG_RENDERER
//	#include <Jolt/Renderer/DebugRenderer.h>
//	#undef JPH_DEBUG_RENDERER
#endif

//#include <Renderer/RenderPrimitive.h>
//#include <Renderer/RenderInstances.h>
//#include <Jolt/Core/Mutex.h>
//#include <unordered_map>

#ifdef JPH_DEBUG_RENDERER

/// Implementation of DebugRenderer
class PhysicsDebugRenderer final : public SceneNode, public JPH::DebugRenderer
{
private:
	Physics* _physics = null;
private:
	SceneArea* _area = null;
	//MeshPNT _mesh;
	List<Material*> _materials;
	LookupTable<uint32_t,Material*> _c_to_m;
	List<RenderInstance*> _renderables;

public:
	/// Constructor
	explicit PhysicsDebugRenderer( Physics* physics );
	virtual ~PhysicsDebugRenderer() override;

protected:
	virtual void onAddedToScene( Scene* scene ) override;
	virtual void onRemovedFromScene( Scene* scene ) override;
	virtual void animate( float dt ) override;
private:
	void createLine( const Vec3& a, const Vec3& b, const vec4& c, float radius );
public:
	/// Implementation of DebugRenderer interface
	virtual void						DrawLine( JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor) override;
	virtual void						DrawTriangle(JPH::Vec3Arg inV1, JPH::Vec3Arg inV2, JPH::Vec3Arg inV3, JPH::ColorArg inColor) override;
	virtual DebugRenderer::Batch						CreateTriangleBatch(const DebugRenderer::Triangle *inTriangles, int inTriangleCount) override;
	virtual DebugRenderer::Batch						CreateTriangleBatch(const DebugRenderer::Vertex *inVertices, int inVertexCount, const JPH::uint32 *inIndices, int inIndexCount) override;
	virtual void						DrawGeometry(JPH::Mat44Arg inModelMatrix, const JPH::AABox &inWorldSpaceBounds, float inLODScaleSq, JPH::ColorArg inModelColor, const DebugRenderer::GeometryRef &inGeometry, DebugRenderer::ECullMode inCullMode, DebugRenderer::ECastShadow inCastShadow, DebugRenderer::EDrawMode inDrawMode) override;
	virtual void						DrawText3D(JPH::Vec3Arg inPosition, const JPH::string_view &inString, JPH::ColorArg inColor, float inHeight )	override;

										/*
	/// Draw all primitives that were added
	void								Draw();

	/// Clear all primitives (to be called after drawing)
	void								Clear();
	
private:
	/// Helper functions to draw sub parts
	void								DrawLines();
	void								DrawTriangles();
	void								DrawTexts();

	/// Helper functions to clear sub parts
	void								ClearLines();
	void								ClearTriangles();
	void								ClearTexts();

	/// Implementation specific batch object
	class BatchImpl : public RefTargetVirtual, public RenderPrimitive
	{
	public:
										BatchImpl(Renderer *inRenderer, D3D_PRIMITIVE_TOPOLOGY inType) : RenderPrimitive(inRenderer, inType) { }

		virtual void					AddRef() override			{ RenderPrimitive::AddRef(); }
		virtual void					Release() override			{ if (--mRefCount == 0) delete this; }
	};

	/// Finalize the current locked primitive and add it to the primitives to draw
	void								FinalizePrimitive();

	/// Ensure that the current locked primitive has space for a primitive consisting inVtxSize vertices
	void								EnsurePrimitiveSpace(int inVtxSize);

	Renderer *							mRenderer;

	/// Shaders for triangles
	unique_ptr<PipelineState>			mTriangleStateBF;
	unique_ptr<PipelineState>			mTriangleStateFF;
	unique_ptr<PipelineState>			mTriangleStateWire;

	/// Shaders for shadow pass for triangles
	unique_ptr<PipelineState>			mShadowStateBF;
	unique_ptr<PipelineState>			mShadowStateFF;
	unique_ptr<PipelineState>			mShadowStateWire;

	/// The shadow buffer (depth buffer rendered from the light)
	Ref<Texture>						mDepthTexture;

	/// Lock that protects the triangle batches from being accessed from multiple threads
	Mutex								mPrimitivesLock;

	Batch								mEmptyBatch;

	/// Properties for a single rendered instance
	struct Instance
	{
		/// Constructor
										Instance(Mat44Arg inModelMatrix, Mat44Arg inModelMatrixInvTrans, ColorArg inModelColor) : mModelMatrix(inModelMatrix), mModelMatrixInvTrans(inModelMatrixInvTrans), mModelColor(inModelColor) { }

		Mat44							mModelMatrix;
		Mat44							mModelMatrixInvTrans;
		Color							mModelColor;
	};

	/// Rendered instance with added information for lodding
	struct InstanceWithLODInfo : public Instance
	{
		/// Constructor
										InstanceWithLODInfo(Mat44Arg inModelMatrix, Mat44Arg inModelMatrixInvTrans, ColorArg inModelColor, const AABox &inWorldSpaceBounds, float inLODScaleSq) : Instance(inModelMatrix, inModelMatrixInvTrans, inModelColor), mWorldSpaceBounds(inWorldSpaceBounds), mLODScaleSq(inLODScaleSq) { }

		/// Bounding box for culling
		AABox							mWorldSpaceBounds;

		/// Square of scale factor for LODding (1 = original, > 1 = lod out further, < 1 = lod out earlier)
		float							mLODScaleSq;
	};
	
	/// Properties for a batch of instances that have the same primitive
	struct Instances
	{
		vector<InstanceWithLODInfo>		mInstances;

		/// Start index in mInstancesBuffer for each of the LOD in the geometry pass. Length is one longer than the number of LODs to indicate how many instances the last lod has.
		vector<int>						mGeometryStartIdx;

		/// Start index in mInstancesBuffer for each of the LOD in the light pass. Length is one longer than the number of LODs to indicate how many instances the last lod has.
		vector<int>						mLightStartIdx;
	};

	using InstanceMap = unordered_map<GeometryRef, Instances>;

	/// Clear map of instances and make it ready for the next frame
	void								ClearMap(InstanceMap &ioInstances);

	/// Helper function to draw instances
	inline void							DrawInstances(const Geometry *inGeometry, const vector<int> &inStartIdx);

	/// List of primitives that are finished and ready for drawing
	InstanceMap							mWireframePrimitives;
	InstanceMap							mPrimitives;
	InstanceMap							mTempPrimitives;
	InstanceMap							mPrimitivesBackFacing;
	int									mNumInstances = 0;
	Ref<RenderInstances>				mInstancesBuffer[Renderer::cFrameCount];

	/// Primitive that is being built + its properties
	Batch								mLockedPrimitive;
	Vertex *							mLockedVerticesStart = nullptr;
	Vertex *							mLockedVertices = nullptr;
	Vertex *							mLockedVerticesEnd = nullptr;
	AABox								mLockedPrimitiveBounds;

	/// A single text string
	struct Text
	{
										Text(Vec3Arg inPosition, const string &inText, ColorArg inColor, float inHeight) : mPosition(inPosition), mText(inText), mColor(inColor), mHeight(inHeight) { }

		Vec3							mPosition;
		string							mText;
		Color							mColor;
		float							mHeight;
	};

	/// All text strings that are to be drawn on screen
	vector<Text>						mTexts;
	Mutex								mTextsLock;

	/// Font with which to draw the texts
	RefConst<Font>						mFont;

	/// A single line segment
	struct Line
	{
										Line(const Float3 &inFrom, const Float3 &inTo, ColorArg inColor) : mFrom(inFrom), mFromColor(inColor), mTo(inTo), mToColor(inColor) { }

		Float3							mFrom;
		Color							mFromColor;
		Float3							mTo;
		Color							mToColor;
	};

	/// The list of line segments
	vector<Line>						mLines;
	Mutex								mLinesLock;

	/// The shaders for the line segments
	unique_ptr<PipelineState>			mLineState;
	*/
};
#endif
