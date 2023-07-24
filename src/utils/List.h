#pragma once

#include "stdodt.h"

template < typename T >
class List
{
	ODTNOCOPY( List )
private:
	std::list< T > _list;
public:
	explicit List(){
	}
//	List( const List<T>& src ) = 0 ;
//{
	//	addAll( src );
	//}
	~List(){
		ASSERT( isEmpty() );
	}
public:
	int size() const{
		return _list.size();
	}
	bool isEmpty() const{
		return _list.empty();
	}
public:
	// Achtung, dies ist eine O(N)-Funktion!
	T operator[]( int i ) const{
		return get( i );
	}
	T get( int i ) const{
		assert( 0 <= i && i < size() );
		auto it = _list.begin();
		if( 0 < i ){
			std::advance( it, i );
		}
		return *it;
	}
	T getModded( int i ) const {
		assert( !isEmpty() );
		assert( 0 <= i );
		return get( i % size() );
	}
	bool contains( T child ) const {
		for( T c : _list ){
			if( c == child )
				return true;
		}
		return false;
	}
	int indexOf( const T child ) const {
		int i = 0;
		for( T c : _list ){
			if( c == child )
				return i;
			i++;
		}
		return -1;
	}
	T first() const{
		assert( !isEmpty() );
		return _list.front();
	}
	T last() const{
		assert( !isEmpty() );
		return _list.back();
	}
	T single() const{
		assert( size() == 1 );
		return _list.front();
	}
	T findFirst( std::function<bool( T o )> filter ) const {
		for( T o : _list ){
			if( filter( o ) )
				return o;
		}
		return null;
	}
public:
	void add( T child, bool checkNotContains = true ){
		assert( child );
		if( checkNotContains ){
			ASSERT( !contains( child ) );
		}
		_list.push_back( child );
	}
	void addAll( const List<T>& list ){
		for( T c : list ){
			add( c );
		}
	}
	void insert( int index, T child ){
		assert( 0 <= index && index <= size() );
		assert( child );
		ASSERT( !contains( child ) );
		auto it = _list.begin();
		if( 0 < index ){
			std::advance( it, index );
		}
		_list.insert( it, child );
	}
	void remove( T child ){
		int old_size = size();
		_list.remove( child );
		int new_size = size();
		assert( old_size -1 == new_size );
	}
	void removeAt( int i ){
		assert( 0 <= i && i < size() );
		auto it = _list.begin();
		if( 0 < i ){
			std::advance( it, i );
		}
		int old_size = size();
		_list.erase( it );
		int new_size = size();
		assert( old_size -1 == new_size );
	}
	void removeAll(){
		_list.clear();
	}
	T takeFirst(){
		T f = _list.front();
		_list.pop_front();
		return f;
	}
	T takeLast(){
		T last = _list.back();
		_list.pop_back();
		return last;
	}
	void deleteAll(){
		while( !isEmpty() ){
			T last = takeLast();
			delete last;
		}
	}
public:
	void sort( std::function<int( T a, T b )> sorter ){
		_list.sort( [&]( T k, T l ){
			int r = sorter( k, l );
			if( r >= 0 )return false;
			return true;
		} );
		// sorter );
	}
public:
	std::_List_const_iterator<T> begin() const {
		return _list.begin();
	}
	std::_List_const_iterator<T> end() const {
		return _list.end();
	}
};
