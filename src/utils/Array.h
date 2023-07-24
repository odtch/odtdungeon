#pragma once

#include "stdodt.h"

template < typename T >
class Array
{
	ODTNOCOPY( Array );
private:
	uint _size;
	T* _array = null;
//	std::vector< T > _list;
public:
	explicit Array( uint size )
		:_size( size )
	{
		assert( 0 < _size );
		_array = new T[ _size ];
	}
	~Array(){
		if( _array != null ){
			delete[] _array;
			_array = null;
		}
	}
public:
	int size() const{
		return _size;
	}
public:
	const T& operator[]( int i ) const{
		assert( 0 <= i && i < _size );
		return _array[ i ];
	}
	T& operator[]( int i ){
		assert( 0 <= i && i < _size );
		return _array[ i ];
	}
	const T& get( int i ) const{
		assert( 0 <= i && i < _size );
		return _array[ i ];
	}
	void set( int i, T value ){
		assert( 0 <= i && i < _size );
		_array[ i ] = value;
	}
public:
	const T& first() const{
		return _array[0];
	}
	T& first(){
		return _array[0];
	}
public:
	void fill( const T& value ){
		T* p = _array;
		for( int i = 0; i < _size; i++ ){
			(*p) = value;
			p++;
		}
	}
//	const T& first() const{
//		return _list.front();
//	}
//	const T& operator[]( int i ) const{
//		return get( i );
//	}
//	T& operator[]( int i ){
//		return get( i );
//	}
//	const T& get( int i ) const{
//		assert( 0 <= i && i < size() );
//		return _list.at( i );
//	}
//	T& get( int i ){
//		assert( 0 <= i && i < size() );
//		return _list.at( i );
//	}
//	bool contains( T child ) const {
//		for( T c : _list ){
//			if( c == child )
//				return true;
//		}
//		return false;
//	}
//public:
//	void fill( T value ){
//		for( int i = 0; i < size(); i++ ){
//			get( i ) = value;
//		}
//	}
//	void add( T child ){
//		assert( child );
//		_list.push_back( child );
//	}
//	void remove( T child ){
//		int old_size = size();
//		_list.remove( child );
//		int new_size = size();
//		assert( old_size -1 == new_size );
//	}
//	void removeAll(){
//		_list.clear();
//	}
//	T takeLast(){
//		T last = _list.back();
//		_list.pop_back();
//		return last;
//	}
//	void deleteAll(){
//		while( !isEmpty() ){
//			T last = takeLast();
//			delete last;
//		}
//	}
public:
//	std::_List_const_iterator<T> begin() const {
//		return _list.begin();
//	}
//	std::_List_const_iterator<T> end() const {
//		return _list.end();
//	}
};
