#include "stdodt.h"
#include "FloatStatistic.h"

FloatStatistic::FloatStatistic()
	:_min( 0 )
	,_max( 0 )
	,_sum( 0 )
	,_count( 0 )
{
}
FloatStatistic::FloatStatistic(const FloatStatistic& values)
	:_min( values._min )
	,_max( values._max )
	,_sum( values._sum )
	,_count( values._count )
{
}
FloatStatistic::~FloatStatistic(){
}
void FloatStatistic::reset(){
	_min = 0;
	_max = 0;
	_sum = 0;
	_count = 0;
}
void FloatStatistic::reset( float avg ){
	_min = avg;
	_max = avg;
	_sum = avg;
	_count = 1;
}
void FloatStatistic::set(float min, float max, float sum, int count){
	ASSERT( min <= max );
	ASSERT( 0 <= count );
	_min = min;
	_max = max;
	_sum = sum;
	_count = count;
}
void FloatStatistic::add( float value ){
	if( _count == 0 ){
		_min = value;
		_max = value;
		_sum = value;
		_count = 1;
	} else {
		_min = std::min( _min, value );
		_max = std::max( _max, value );
		_sum += value;
		_count++;
	}
}
void FloatStatistic::add( const FloatStatistic& other ){
	adjustMinMax( other.min(), other.max() );
	_sum += other._sum;
	_count += other.count();
}
void FloatStatistic::adjustMinMax( float min, float max ){
	if( min < _min )_min = min;
	if( _max < max )_max = max;
}
bool FloatStatistic::isInside( float value ) const {
	if( value < _min )return false;
	if( _max < value )return false;
	return true;
}
float FloatStatistic::toFactor(float value) const {
	float mtm = minToMax();
	if( mtm <= 0.0f )
		return 0;
	return ( value - _min ) / mtm;
}
std::string FloatStatistic::toString() const{
	return std::string( "min: " ) + std::to_string( min() )
			+ std::string( ", max: " ) + std::to_string( max() )
			+ std::string( ", avg: " ) + std::to_string( _count == 0 ? 0 : avg() )
			+ std::string( ", count: " ) + std::to_string( count() ) ;
}

#ifdef ODTSERIALIZATION
FloatStatistic::FloatStatistic( DeserializationStream& stream )
	:FloatStatistic()
{
	deserialize( stream );
}
void FloatStatistic::deserialize( DeserializationStream &stream ){
	_min = stream.loadQReal();
	_max = stream.loadQReal();
	_sum = stream.loadQReal();
	_count = stream.loadArraySize();
	ASSERT( _min <= _max );
	ASSERT( 0 <= _count );
}
void FloatStatistic::serialize( SerializationStream& stream ) const{
	stream.writeQReal( _min );
	stream.writeQReal( _max );
	stream.writeQReal( _sum );
	stream.writeArraySize( _count );
}
#endif
