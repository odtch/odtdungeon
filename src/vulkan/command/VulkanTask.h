#pragma once

#include "vulkan/VulkanCore.h"
#include "VulkanSemaphore.h"
#include "VulkanFence.h"

class VulkanTaskData : public SharedPointerData
{
	DEBUGCOUNTER(VulkanTaskData)
	ODTNOCOPY(VulkanTaskData)
private:
	enum State {
		Uninitialzed,
		Initializing,
		Running,
		Completed
	};
	State _state = Uninitialzed;
private:
	int _id;
	String _name;
	PerformanceCounter* _host_pc = null;
private:
	VulkanDevice* _device = null;
	VulkanCommandBuffer* _commandbuffer = null;
private:
	uint _maxCommandCount = 0;
	uint _commandCount = 0;
	SharedPointer<VulkanCommand>* _commands = null;
#ifdef ODTDEBUG
	VulkanQueryPool* _performancecounter = null;
#endif
private:
	//List<VulkanSemaphore*> _waiting_semaphores_in_next_run;
	//List<VulkanSemaphore*> _waiting_semaphores;
private:
	VulkanSemaphore _completedSemaphore;
	VulkanFence _completedFence;
private:
	std::list<std::function<void()>> _onCompleteds;
public:
	explicit VulkanTaskData( const String& name );
	virtual ~VulkanTaskData() override;
public:
	friend class VulkanTask;
};

class VulkanTask : private SharedPointer<VulkanTaskData>
{
	DEBUGCOUNTER(VulkanTask)
public:
	enum RestartType {
		RunOnce, RunMultipleTimes
	};
public:
	explicit VulkanTask();
	explicit VulkanTask( const String& name, RestartType restarttype, uint maxCommandCount, VulkanCommandPool* pool );
	explicit VulkanTask( const VulkanTask& task );
	virtual ~VulkanTask() override;
public:
	VulkanDevice* device(){ return data()->_device; }
public:
	bool isNull() const{ return SharedPointer<VulkanTaskData>::isNull(); }
	const String& name() const{ return data()->_name; }
	void create( const String& name, RestartType restarttype, uint maxCommandCount, VulkanCommandPool* pool );
	void clear(){ SharedPointer<VulkanTaskData>::clear(); }
	void operator=( const VulkanTask& task );
public:
	bool hasPlaceForNCommands( uint commandCount ) const{
		return data()->_commandCount + commandCount <= data()->_maxCommandCount;
	}
public:
	void addWaitFor( VulkanTask& predecessor, VkPipelineStageFlags stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT );
	void addWaitFor( VulkanSemaphore& semaphore, VkPipelineStageFlags stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT );
	//void addWaitForInNextRun( VulkanSemaphore* semaphore );
public:
	void add( VulkanCommand* command );
	void add( SharedPointer<VulkanCommand>& command );
public:
	void addLoadBuffer( VulkanBuffer& buffer, const void* data, const size_t& size );
public:
	VulkanCommandBuffer& commandBuffer(){ assert( data()->_commandbuffer ); return *data()->_commandbuffer; }
public:
	VulkanSemaphore& completedSemaphore(){ return data()->_completedSemaphore; }
public:
	void addOnCompleted( std::function<void()> onCompleted );
public:
	void start();
	bool isInitializing();
	bool isRunning();
	bool isCompleted();
public:
	enum RefreshReturnType { RefreshCompleted, RefreshContinue };
	RefreshReturnType refresh();
protected:
	virtual void destroyData( VulkanTaskData* data ) override;
};
