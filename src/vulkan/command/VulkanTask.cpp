#include "stdodt.h"

#include "VulkanTask.h"
#include "VulkanCommand.h"
#include "VulkanCommandBuffer.h"
#include "VulkanCommandPool.h"
#include "vulkan/device/VulkanDevice.h"
#include "vulkan/device/VulkanPhysicalDevice.h"
#include "VulkanQueue.h"
#include "VulkanFence.h"
#include "VulkanSemaphore.h"
#include "vulkan/resource/VulkanBuffer.h"
#include "utils/DebugCounterTable.h"

#ifdef ODTDEBUG
DebugCounterTable vtd( "VulkanTaskData" );
#endif
int next_task_id = 0;

VulkanTaskData::VulkanTaskData( const String& name )
	:_name( name )
{
#ifdef ODTDEBUG
vtd.inc( _name );
#endif
	_id = (next_task_id++);
}
VulkanTaskData::~VulkanTaskData(){
#ifdef ODTDEBUG
vtd.decr( _name );
#endif
}

// https://stackoverflow.com/questions/70461613/vkcmdwritetimestamp-writes-the-same-time-before-and-after-the-draw-calls
// https://nikitablack.github.io/post/how_to_use_vulkan_timestamp_queries/
// https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/vkCmdWriteTimestamp.html
// https://stackoverflow.com/questions/67358235/how-to-measure-execution-time-of-vulkan-pipeline

VulkanTask::VulkanTask()
//	:Pointer<VulkanTaskData>( null )
{
}
VulkanTask::VulkanTask( const String& name, RestartType restarttype, uint maxCommandCount, VulkanCommandPool* pool )
//	:Pointer<VulkanTaskData>( null )
{
	create( name, restarttype, maxCommandCount, pool );
}
VulkanTask::VulkanTask( const VulkanTask& task )
	:SharedPointer<VulkanTaskData>( task )
{
//	if( task.data()->_id == 603 ){
//		logDebug( "aaaa" );
//	}
}
VulkanTask::~VulkanTask(){
	if( !isNull() )
//	if( data()->_id == 603 ){
//		logDebug( "aaaabbbbb" );
//	}
	clear();
}
void VulkanTask::create(const String& name, RestartType restarttype, uint maxCommandCount, VulkanCommandPool* pool ){
	assert( isNull() );
	VulkanTaskData* data = new VulkanTaskData( name );
//	logDebug( "VulkanTask::create", data->_id, name );
	SharedPointer<VulkanTaskData>::operator=( data );
	data->_maxCommandCount = maxCommandCount;
	assert( pool );
	assert( 0 < maxCommandCount );
	data->_host_pc = PerformanceCounter::GetOrCreate( String( "GPU." ) + data->_name );
	data->_device = pool->device();
	data->_commands = new SharedPointer<VulkanCommand>[ maxCommandCount ];
	data->_commandbuffer = new VulkanCommandBuffer();
	data->_commandbuffer->begin( ( restarttype == RunOnce ? VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT : 0 ), pool );
#ifdef ODTDEBUG
	data->_performancecounter = data->_device->createPerformanceCounter();
#endif
	data->_completedSemaphore.create( data->_device );
	data->_completedFence.create( name, 0, data->_device );
	data->_state = VulkanTaskData::Initializing;
#ifdef ODTDEBUG
	vkCmdResetQueryPool( data->_commandbuffer->vkCommandBuffer(), data->_performancecounter->vkQueryPool(), 0, 2 );
	vkCmdWriteTimestamp( data->_commandbuffer->vkCommandBuffer(), VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, data->_performancecounter->vkQueryPool(), 0 );
#endif
	pool->queue()->onTaskCreated( *this );
}
void VulkanTask::operator=(const VulkanTask& task){
//	if( task.data()->_id == 603 ){
//		logDebug( "aaaacccc" );
//	}
	SharedPointer<VulkanTaskData>::operator=( task );
}
void VulkanTask::addWaitFor( VulkanTask& predecessor, VkPipelineStageFlags stage ){
	assert( !predecessor.isNull() );
	assert( isInitializing() );
	//logDebug( "VulkanTask", data()->_id, data()->_name, "addWaitFor", predecessor.data()->_id, predecessor.data()->_name );
	if( predecessor.isCompleted() ){
//		logDebug( "VulkanTask", data()->_id, data()->_name, "addWaitFor", predecessor.data()->_id, predecessor.data()->_name );
//		logDebug( "  predecessor already completed" );
		return;
	}
	data()->_commandbuffer->addWait( predecessor.completedSemaphore(), stage );
}
void VulkanTask::addWaitFor( VulkanSemaphore& semaphore, VkPipelineStageFlags stage ){
	assert( isInitializing() );
	//data()->_waiting_semaphores.add( new VulkanSemaphore( semaphore ) );
	data()->_commandbuffer->addWait( semaphore, stage );
}
void VulkanTask::add( VulkanCommand* command ){
	assert( command );
	assert( isInitializing() );
	SharedPointer<VulkanCommand> ptr( command );
	add( ptr );
}
void VulkanTask::add( SharedPointer<VulkanCommand>& command ){
	assert( !command.isNull() );
	assert( isInitializing() );
	if( data()->_maxCommandCount <= data()->_commandCount ){
		logError( "VulkanTask.add too many cmds", data()->_name );
		assert( false );
	}
	data()->_commands[ data()->_commandCount ] = command;
	data()->_commandCount++;
}
void VulkanTask::addLoadBuffer( VulkanBuffer& buffer, const void* data, const size_t& size ){
	add( new VulkanLoadBufferCommand( buffer, data, size ) );
}
void VulkanTask::addOnCompleted( std::function<void ()> onCompleted ){
	assert( isInitializing() );
	data()->_onCompleteds.push_back( onCompleted );
}

void VulkanTask::start(){
	try{
		VulkanTaskData* data = this->data();
		if( data->_state == VulkanTaskData::Initializing ){
			for( int c = 0; c < data->_commandCount; c++ ){
				data->_commands[c]->bind( *data->_commandbuffer );
			}
	#ifdef ODTDEBUG
			vkCmdWriteTimestamp( data->_commandbuffer->vkCommandBuffer(), VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, data->_performancecounter->vkQueryPool(), 1 );
	#endif
			data->_commandbuffer->end();
		} else {
			assert( false );
		}
		data->_state = VulkanTaskData::Running;
		//logDebug( "VulkanTask started", data->_id, data->_name, "cmdCount=", data->_commandCount );
		data->_commandbuffer->submit( &data->_completedSemaphore, &data->_completedFence );
	} catch( const std::exception& ex ){
		logError( "VulkanTask failed", name(), ex.what() );
		throw;
	}
}
bool VulkanTask::isInitializing(){
	if( isNull() )return false;
	return data()->_state == VulkanTaskData::Initializing;
}
bool VulkanTask::isRunning(){
	if( isNull() )return false;
	return data()->_state == VulkanTaskData::Running;
}
bool VulkanTask::isCompleted(){
	if( data()->_state == VulkanTaskData::Running ){
		refresh();
	}
	return data()->_state == VulkanTaskData::Completed;
}
VulkanTask::RefreshReturnType VulkanTask::refresh(){
	VulkanTaskData* data = this->data();
	//logDebug( "VulkanTask", data->_id, data->_name, ".refresh", data->_state, data->_completedFence.isSignaled() );
//	if( data->_id == 603 ){
//		logDebug( "VulkanTask", data->_id, data->_name, ".refresh", data->_state, data->_completedFence.isSignaled() );
//	}
	switch( data->_state ){
	case VulkanTaskData::Uninitialzed:
		logDebug( "VulkanTask refresh on unitialized" );
		return RefreshContinue;
	case VulkanTaskData::Initializing:
		logDebug( "VulkanTask refresh in initializing" );
		return RefreshContinue;
	case VulkanTaskData::Running:
		if( data->_completedFence.isSignaled() ){
//			if( data->_id == 603 ){
//				logDebug( "VulkanTask", data->_id, data->_name, ".refresh completed", data->_state, data->_completedFence.isSignaled() );
//			}
			//data->_waiting_semaphores.deleteAll();
			for( int c = 0; c < data->_commandCount; c++ ){
				data->_commands[c]->onCompleted();
			}
			for( std::function<void()> onCompleted : data->_onCompleteds ){
				onCompleted();
			}
#ifdef ODTDEBUG
			if( data->_performancecounter ){
				uint64_t buffer[2];
				VkResult result = vkGetQueryPoolResults( data->_device->vkDevice(), data->_performancecounter->vkQueryPool(), 0, 2, sizeof(uint64_t) * 2, buffer, sizeof(uint64_t), VK_QUERY_RESULT_64_BIT );
				if( result == VK_NOT_READY ){
					logDebug( "VulkanTask qp not ready" );
				} else if( result == VK_SUCCESS ){
					//logDebug( "VulkanTask time", ( buffer[1] - buffer[0] ) );
				} else {
					logDebug( "VulkanTask qp failed" );
				}
				if( data->_host_pc ){
					data->_host_pc->add_s( data->_performancecounter->duration_to_ms( buffer[1] - buffer[0] ) );
				}
				if( data->_performancecounter ){
					data->_device->destroyPerformanceCounter( data->_performancecounter );
					data->_performancecounter = null;
				}
			}
#endif
			data->_state = VulkanTaskData::Completed;
			//logDebug( "VulkanTask completed", data->_id, data->_name, "cmdCount=", data->_commandCount );
			return RefreshCompleted;
		}
		return RefreshContinue;
	case VulkanTaskData::Completed:
		return RefreshCompleted;
	default:
		assert( false );
		return RefreshContinue;
	}
}
void VulkanTask::destroyData( VulkanTaskData* data ){
	assert( data );
	//logDebug( "Task::destroy", data->_name );
	if( data->_state == VulkanTaskData::Completed ){
	} else {
		logDebug( "~VulkanTask not completed" );
	}
//	if( data->_id == 603 ){
//		logDebug( "aaaabbbbbxxxxxx" );
//	}
	//data->_waiting_semaphores.deleteAll();
	data->_onCompleteds.clear();
	if( data->_commands ){
		for( uint c = 0; c < data->_commandCount; c++ ){
			data->_commands[c].clear();
		}
		delete[] data->_commands;
		data->_commands = null;
	}
	if( data->_commandbuffer ){
		data->_commandbuffer->destroy();
		odelete( data->_commandbuffer );
	}
//	_completedSemaphore->destroy();
//	_completedFence.destroy();
#ifdef ODTDEBUG
	if( data->_performancecounter ){
		data->_device->destroyPerformanceCounter( data->_performancecounter );
		data->_performancecounter = null;
	}
#endif
	data->_device = null;
//	logDebug( "VulkanTask.destroy", data->_id, data->_name );
	delete data;
}

