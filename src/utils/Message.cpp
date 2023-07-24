#include "stdodt.h"

#include "Message.h"
#include "LookupTable.h"
#include "Debugging.h"
#include "Logger.h"

MessageQueue::MessageQueue(){
}
MessageQueue::~MessageQueue(){
	_mutex.lock();
	_next_read = 0;
	_next_write = 0;
}
void MessageQueue::post( const Message& message ){
	post( message.type, message.p1, message.p2, message.sender );
}
void MessageQueue::post( MessageType type, void* p1, void* p2, void* sender ){
	_mutex.lock();
	if( _next_read <= _next_write && _next_write +1 < MaxMessages ){
		_next_write++;
		if( MaxMessages <= _next_write ){
			logError( "MessageQueue::post over end of array" );
			_next_write = 0;
		}
	} else {
		logError( "MessageQueue::post unhandled state", _next_read, _next_write );
	}
	_mutex.unlock();
}
bool MessageQueue::get( Message& message ){
	_mutex.lock();
	bool result;
	if( _next_read == _next_write ){
		_mutex.unlock();
		return false;
	} else {
		_mutex.unlock();
		return true;
	}
}
void MessageQueue::receive( MessageType type, void* p1, void* p2, void* sender ){
	post( type, p1, p2, sender );
}

MessageListeners::MessageListeners( uint max_listener_count )
	:_listeners( max_listener_count )
{
	for( int l = 0; l < _listeners.size(); l++ ){
		_listeners[ l ] = null;
	}
}
MessageListeners::~MessageListeners(){
	for( int l = 0; l < _listeners.size(); l++ ){
		_listeners[ l ] = null;
	}
}
void MessageListeners::add( MessageListener* listener ){
	assert( listener );
	for( int l = 0; l < _listeners.size(); l++ ){
		if( _listeners[ l ] == null ){
			_listeners[ l ] = listener;
			return;
		}
	}
	logError( "MessageListeners::add too many listeners" );
	ASSERT( false );
}
void MessageListeners::remove( MessageListener* listener ){
	assert( listener );
	for( int l = 0; l < _listeners.size(); l++ ){
		if( _listeners[ l ] == listener ){
			_listeners[ l ] = null;
			return;
		}
	}
	logError( "MessageListeners::remove listener not found" );
	ASSERT( false );
}
void MessageListeners::send( MessageType type, void* p1, void* p2, void* sender ){
	for( int l = 0; l < _listeners.size(); l++ ){
		if( _listeners[ l ] ){
			_listeners[ l ]->receive( type, p1, p2, sender );
		}
	}
}
