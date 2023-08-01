#pragma once

#include "List.h"
#include "Logger.h"

template < typename T >
class GradientTable
{
private:
	struct State {
		float key;
		T value;		
	};
	struct Entry {
		State from;
		State to;
		float length;
	};
private:
	List<Entry*> _entries;
	float _total_length = 0;
public:
	explicit GradientTable(){
	}
	explicit GradientTable( const GradientTable<T>& src ){
		set( src );
	}
	~GradientTable(){
		clear();
	}
public:
	bool isEmpty() const{ return _entries.isEmpty(); }
public:
	void operator=( const GradientTable<T>& src ){
		clear();
		set( src );
	}
	void clear(){
		_entries.deleteAll();
		_total_length = 0;
	}
	void set(  const GradientTable<T>& src ){
		ASSERT( _entries.isEmpty() );
		for( Entry* srcentry : src._entries ){
			Entry* entry = new Entry();
			entry->from.key = srcentry->from.key;
			entry->from.value = srcentry->from.value;
			entry->to.key = srcentry->to.key;
			entry->to.value = srcentry->to.value;
			entry->length = srcentry->length;
			_entries.add( entry );
		}
		_total_length = src._total_length;
	}
public:
	void add( float key, const T& value ){
		if( _entries.size() == 0 ){
			ASSERT( key == 0 );
			Entry* entry = new Entry();
			entry->from.key = 0;
			entry->from.value = value;
			entry->to.key = 0;
			entry->to.value = value;
			_entries.add( entry );
			_total_length = 0;
		} else if( _entries.size() == 1 && _entries.first()->to.key == 0 ){
			ASSERT( 0 < key );
			Entry* entry = _entries.first();
			entry->to.key = key;
			entry->to.value = value;
			entry->length = key;
			_total_length = key;
		} else {
			ASSERT( 0 < key );
			Entry* prev = _entries.last();
			Entry* entry = new Entry();
			entry->from.key = prev->to.key;
			entry->from.value = prev->to.value;
			entry->to.key = entry->from.key + key;
			entry->to.value = value;
			entry->length = key;
			_entries.add( entry );
			_total_length += key;
		}
	}
public:
	T get( float key ) const {
		ASSERT( !_entries.isEmpty() );
		//logDebug( "gt.g", key ); trace();
		if( key < 0 ){
			logError( "GradientTable.get", key );
			return _entries.first()->from.value;
		} else if( _total_length < key ){
			logError( "GradientTable.get", key );
			return _entries.last()->to.value;
		}
		for( Entry* entry : _entries ){
			if( key <= entry->to.key ){
				float f01 = ( key - entry->from.key ) / entry->length;
				//logDebug( "gt.g", key, entry->from.key, entry->to.key, entry->length, f01 );
				ASSERT( 0 <= f01 && f01 <= 1 );
				float f10 = 1.0f - f01;
				return entry->from.value * f10 + entry->to.value * f01;
			}
		}
		logError( "GradientTable.get", key, _total_length );
		trace();
		ASSERT( false );
		return _entries.last()->to.value;
	}
	void trace() const {
		logError( "GradientTable", _total_length );
		for( Entry* entry : _entries ){
			logError( "  ", entry->from.key, "to", entry->to.key, "l", entry->length );
		}
	}
	T getByFactor( float factor ) const {
		ASSERT( 0 <= factor && factor <= 1 );
		return get( factor * _total_length );
	}
};

using Vec2GradientTable = GradientTable<vec2>;
using Vec3GradientTable = GradientTable<vec3>;
using Vec4GradientTable = GradientTable<vec4>;
using QuatGradientTable = GradientTable<glm::quat>;
using ColorGradientTable = Vec4GradientTable;
using FloatGradientTable = GradientTable<float>;


