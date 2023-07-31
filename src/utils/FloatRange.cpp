#include "stdodt.h"
#include "FloatRange.h"

FloatRange::FloatRange()
	:_min( 0 )
	,_max( 0 )
	,_mintomax( 0 )
{
}
FloatRange::FloatRange(const FloatRange& values)
	:_min( values._min )
	,_max( values._max )
	,_mintomax( values._mintomax )
{
}
FloatRange::FloatRange(float min, float max){
	set( min, max );
}
FloatRange::~FloatRange(){
}
void FloatRange::reset(){
	_min = 0;
	_max = 0;
	_mintomax = 0;
}
void FloatRange::setMin( float min ){
	_min = min;
	recalc();
}
void FloatRange::setMax( float max ){
	_max = max;
	recalc();
}
float FloatRange::valuetofactor( float value ) const {
	if( 0 < _mintomax ){
		return ( value - _min ) / _mintomax;
	} else {
		if( value == _min )
			return 0;
		assert( 0 < _mintomax );
		return 0;
	}
}
float FloatRange::factortovalue(float factor) const {
	return _min + factor * _mintomax;
}
void FloatRange::set(float min, float max){
	_min = min;
	_max = max;
	recalc();
}
void FloatRange::adjust(float value){
	if( value < _min )_min = value;
	if( _max < value )_max = value;
	recalc();
}
float FloatRange::adjusted( float value ) const{
	if( value < _min )return _min;
	if( _max < value )return _max;
	return value;
}
bool FloatRange::isInside(float value) const{
	if( value < _min )return false;
	if( _max < value )return false;
	return true;
}
//QString FloatRange::toString() const {
//	return QString( QObject::tr( "%1-%2" ) )
//			.arg( min() )
//			.arg( max() );
//}
void FloatRange::operator=( const FloatRange& range ){
	_min = range._min;
	_max = range._max;
	_mintomax = range._mintomax;
}
void FloatRange::recalc(){
	ASSERT( _min <= _max );
	_mintomax = _max - _min;
}

