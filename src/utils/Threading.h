#pragma once

#include "Debugging.h"

struct ThreadData;

class Thread {
	DEBUGCOUNTER(Thread);
private:
	ThreadData* _data = null;
public:
	explicit Thread();
	virtual ~Thread();
public:
	void start();
	void requestStop();
	void stop();
protected:
	bool should_stop() const;
	void check_stop();
protected:
	virtual void run() = 0;
protected:
	void sleep_ms( uint ms );
private:
	void internal_run();
};

class MutexLocker {
	ODTNOCOPY(MutexLocker);
private:
	//std::lock_guard<std::mutex>* _locker;
	std::timed_mutex* _locked_mutex = null;
public:
	explicit MutexLocker();
	//explicit MutexLocker( std::mutex* mutex );
	~MutexLocker();
public:
	void lock( std::timed_mutex& mutex );
	void unlock();
};

class ThreadStoppedException : public std::runtime_error
{
public:
	explicit ThreadStoppedException();
};

#include "List.h"

template < typename T >
class SyncedQueue
{
private:
	std::timed_mutex _mutex;
private:
	List<T> _queue;
public:
	explicit SyncedQueue(){
	}
	~SyncedQueue(){
		ASSERT( _queue.isEmpty() );
	}
public:
	T get(){
		MutexLocker locker;
		locker.lock( _mutex );
		if( _queue.isEmpty() )
			return null;
		return _queue.takeFirst();
	}
	void add( T value ){
		MutexLocker locker;
		locker.lock( _mutex );
		_queue.add( value );
	}
};
