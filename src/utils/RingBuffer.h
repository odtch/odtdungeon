#pragma once

#include "stdodt.h"
#include "Array.h"

template < typename T >
class RingBuffer
{
	ODTNOCOPY( RingBuffer );
private:
	Array<T> _buffer;
	uint _start;
public:
	explicit RingBuffer( uint size )
		:_buffer( size )
		,_start( 0 )
	{
	}
	~RingBuffer(){
	}
public:
	int size() const{
		return _buffer.size();
	}
public:
	const T& operator[]( int i ) const{
		return _buffer[ index( i ) ];
	}
	T& operator[]( int i ){
		return _buffer[ index( i ) ];
	}
public:
	T& next(){
		_start = ( _start + 1 ) % size();
		return _buffer[ _start ];
	}
private:
	int index( int i ) const {
		ASSERT( 0 <= i && i < size() );
		return ( size() + _start - i ) % size();
	}
};

