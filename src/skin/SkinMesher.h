#pragma once

#include "SkinMesh.h"
//#include "utils/Job.h"
#include "vulkan/compute/VulkanComputeProgram.h"
#include "vulkan/compute/VulkanComputeExecutor.h"

//class SkinMesherJob : public AsyncJob {
//protected:
//	const Skin* _skin = null;
//	const SkinSkeleton* _skeleton = null;
//protected:
//	const Mesh<SkinVertex>* _source = null;
//	MeshPNT* _target = null;
//protected:
//	bool _skin_matrix_valid[ 100 ];
//	uint _skin_matrix_count = 0;
//	Mat4 _skin_matrices[ 100 ];
//public:
//	explicit SkinMesherJob();
//	virtual ~SkinMesherJob() override;
//public:
//	void initialize( const Skin* skin, const SkinSkeleton* skeleton, const Mesh<SkinVertex>* source, MeshPNT* target );
//protected:
//	void loadSkeleton( const SkinSkeleton& skeleton );
//public:
//	virtual void refreshState() = 0;
//};

//class SkinMesherCpuJob : public SkinMesherJob {
//private:
//public:
//	explicit SkinMesherCpuJob();
//	virtual ~SkinMesherCpuJob() override;
//protected:
//	virtual JobQueue* detect_queue( JobScheduler& scheduler ) override;
//protected:
//	virtual void run() override;
//public:
//	virtual void refreshState() override;
//};

//class SkinMesherGpuJob : public SkinMesherJob {
//private:
//	SkinMesherExecutor* _executor = null;
//public:
//	explicit SkinMesherGpuJob();
//	virtual ~SkinMesherGpuJob() override;
//protected:
//	virtual JobQueue* detect_queue( JobScheduler& scheduler ) override;
//protected:
//	virtual void run() override;
//public:
//	virtual void refreshState() override;
//};


class SkinMesherProgram : public VulkanComputeProgram
{
private:
	static SkinMesherProgram* _skinmesherProgram;
public:
	static SkinMesherProgram* Get();
private:
	VulkanTask* _renderTask = null;
public:
	explicit SkinMesherProgram();
	virtual ~SkinMesherProgram() override;
public:
	void create( VulkanDevice* device, VulkanTask* renderTask );
	virtual void destroy() override;
public:
	VulkanTask* renderTask(){ return _renderTask; }
protected:
	virtual void addBindings( VulkanDescriptorSetLayout& descriptorSetLayout ) override;
};

class SkinMesherExecutor : public VulkanComputeExecutor
{
private:
	SkinMesherProgram* _program = null;
private:
	enum { MaxMatricesCount = 100 };
	struct {
		uint32_t count;
		float filler1[3];
//		uint32_t count2;
//		float a2, a1;
		Mat4 data[ MaxMatricesCount ];
		float a;
		float b;
	} _matrices;
	VulkanBuffer _matricesBuffer;
private:
	Mesh<SkinVertex>* _source_mesh;
	Mesh<VertexPNT>* _target_mesh;
public:
	explicit SkinMesherExecutor();
	virtual ~SkinMesherExecutor() override;
public:
	void create( Mesh<SkinVertex>* source_mesh, Mesh<VertexPNT>* target_mesh );
	virtual void destroy() override;
protected:
	virtual void bind( VulkanDescriptorSet& descriptorSet ) override;
public:
	void start( const Mat4* matrices, uint matrixCount );
public:
	friend class SkinRenderable;
};
