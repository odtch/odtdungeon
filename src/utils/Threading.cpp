#include "stdodt.h"

#include "Threading.h"
#include "Logger.h"

#include <thread>
#include <atomic>
#include <chrono>

struct ThreadData {
	std::atomic<bool> _started = false;
	std::atomic<bool> _running = false;
	std::thread _thread;
	std::atomic<bool> _should_stop = false;
	std::atomic<bool> _joined = false;
};


ThreadStoppedException::ThreadStoppedException()
	:std::runtime_error( "thread stopped" )
{
}

Thread::Thread(){
	_data = new ThreadData();
}
Thread::~Thread(){
	if( _data ){
		ASSERT( !_data->_running );
		if( !_data->_joined ){
			logDebug( "Thread deleting not joined" );
			stop();
		}
		delete _data;
	}
}
void Thread::start(){
	//logDebug( "Thread::start" );
	assert( _data->_started == false );
	assert( _data->_running == false );
	_data->_running = true;
	_data->_thread = std::thread( &Thread::internal_run, this );
}
void Thread::requestStop(){
	_data->_should_stop = true;
}
void Thread::stop(){
	//logDebug( "Thread::stop" );
	requestStop();
	if( _data->_thread.joinable() ){
		//logDebug( "Thread::stop joining" );
		_data->_thread.join();
		//logDebug( "Thread::stop joined" );
	}
	if( _data->_running ){
		logDebug( "Thread::stop running not false" );
	}
	_data->_joined = true;
}
bool Thread::should_stop() const {
	return _data->_should_stop;
}
void Thread::check_stop(){
	if( should_stop() )
		throw ThreadStoppedException();
}
void Thread::sleep_ms( uint ms ){
	//logDebug( "Thread::sleep", ms );
	std::this_thread::sleep_for( std::chrono::milliseconds( ms ) );
}
void Thread::internal_run(){
	try{
		//logDebug( "Thread::internal_run" );
		run();
		//logDebug( "Thread::internal_run done" );
	} catch( const ThreadStoppedException& ex ){
		//logError( "Thread::internal_run stopped", ex.what() );
	} catch( const std::exception& ex ){
		logError( "Thread::internal_run failed", ex.what() );
	} catch( ... ){
		logError( "Thread::internal_run failed unexpected" );
	}
	_data->_running = false;
}

MutexLocker::MutexLocker(){
}
MutexLocker::~MutexLocker(){
	if( _locked_mutex )
		unlock();
}
void MutexLocker::lock( std::timed_mutex& mutex ){
	ASSERT( _locked_mutex == null );
	while( !mutex.try_lock_for( std::chrono::milliseconds( 1000 ) ) ){
		logWarning( "MutexLocker lock failed" );
	}
	_locked_mutex = &mutex;
}
void MutexLocker::unlock(){
	ASSERT( _locked_mutex );
	_locked_mutex->unlock();
	_locked_mutex = null;
}

