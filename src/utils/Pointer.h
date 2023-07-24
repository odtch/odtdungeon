#pragma once

#include "stdodt.h"
#include "Debugging.h"

class SharedPointerData {
private:
	DEBUGCOUNTER( SharedPointerData );
	ODTNOCOPY( SharedPointerData );
private:
	int _ref_count = 0;
public:
	explicit SharedPointerData(){}
	virtual ~SharedPointerData(){
		ASSERT( _ref_count == 0 );
	}
public:
	template< typename T > friend class SharedPointer;
};

template < typename Data >
class SharedPointer
{
private:
	Data* _data;
public:
	explicit SharedPointer()
		:_data( null )
	{
	}
	explicit SharedPointer( Data* data )
		:_data( data )
	{
		assert( _data );
		if( _data )_data->_ref_count++;
	}
	explicit SharedPointer( const SharedPointer<Data>& src )
		:_data( src._data )
	{
		if( _data )_data->_ref_count++;
	}
	virtual ~SharedPointer(){
		clearData();
		ASSERT( _data == null );
	}
public:
	bool isNull() const{
		return _data == null;
	}
	Data* data() const{
		assert( _data );
		return _data;
	}
	Data* operator->() const {
		assert( _data );
		return _data;
	}
public:
	void operator=( const SharedPointer<Data>& src ){
		//assert( _data == null );
		clearData();
		_data = src._data;
		if( _data )_data->_ref_count++;
	}
	void operator=( Data* data ){
		clearData();
		if( data ){
			_data = data;
			_data->_ref_count++;
		}
	}
	void clear(){
		clearData();
	}
protected:
	virtual void destroyData( Data* data ){
		// warning, clearing on destructor will not more override
		ASSERT( data );
		odelete( data );
	}
private:
	void clearData(){
		if( _data ){
			Data* data = _data;
			_data = null;
			data->_ref_count--;
			ASSERT( 0 <= data->_ref_count );
			if( data->_ref_count == 0 ){
				destroyData( data );
			}
		}
	}
};
