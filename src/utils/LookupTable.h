#pragma once

template < typename K, typename T >
class LookupTable
{
private:
	std::unordered_map<K,T> _map;
public:
	explicit LookupTable(){
	}
	LookupTable( const LookupTable<K,T>& src ) = delete;
	~LookupTable(){
	}
public:
	bool contains( K key ) const {
		return _map.find( key ) != _map.end();
	}
	T find( K key ) const {
		auto it = _map.find( key );
		if( it == _map.end() )
			return null;
		return it->second;
	}
	T get( K key ) const {
		auto it = _map.find( key );
		if( it == _map.end() ){
			logError( "lookup-key not found", key );
			assert( false );
		}
		return it->second;
	}
public:
	void add( K key, T value ){
		assert( _map[ key ] == null );
		_map[ key ] = value;
	}
public:
	void clear(){
		_map.clear();
	}
public:
	void foreachValue( std::function<void( T value )> func ) const {
		for( auto it : _map ){
			func( it.second );
		}
	}
	void foreachKeyValue( std::function<void( K key, T value )> func ) const {
		for( auto it : _map ){
			func( it.first, it.second );
		}
	}
};
