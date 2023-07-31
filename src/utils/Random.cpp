#include "stdodt.h"
#include "Random.h"

Random::Random(){
	_gen1	= -1;
	_gen2	= 1;
	_max	= 337357757;
	_seed	= 0;
}
Random::Random( int aGen1, int aMax ){
	init( aGen1, aMax );
}
Random::~Random(){
}
void Random::init( int aGen1, int aMax ){
	ASSERT( aGen1 > 1000 );
	ASSERT( aMax  > 1000 );
	_gen1	= aGen1;
	_gen2	= aGen1 * 2;
	_max	= aMax;
	_seed	= 0;
}
void Random::reset(){
	_seed	= 0;
}
int Random::geti(){
	if( _gen1 < 0 ){
		//return qrand() % _max;
		int z = rand();
		return (int)( z % _max ); // - QRandomGenerator::min();
	}
	_seed = ( ( _gen1 * _seed ) + _gen2 ) % _max;
	if( _seed < 0 )return -_seed;
	return _seed;
}
int Random::geti( int min, int max ){
	assert( min < max );
	int i = geti();
	if( max <= min )return min;
	i = min + ( i % ( max - min +1 ) );
	ASSERT( min <= i );
	ASSERT( i <= max );
	return i;
}
//float Random::nextfloat(){
//	float r = ((float) ( next() % _max ) ) / ( (float) _max );
//	ASSERT( 0.0f <= r );
//	ASSERT( r <= 1.0f );
//	return r;
//}
float Random::getf( float min, float max ){
	assert( min < max );
	assert( 0.0f < max );
	assert( 0 < _max );
	int i = geti();
	float r = min + ( ( (float)( i ) * ( max - min ) ) / _max );
	ASSERT( min <= r );
	ASSERT( r <= max );
	return r;
}
float Random::getf_ring( float innerradius, float outerradius ){
	float intoout = outerradius - innerradius;
	assert( 0 < intoout );
	float v = getf( -intoout, intoout );
	if( v < 0 ){
		v -= innerradius;
	} else {
		v += innerradius;
	}
//	if( v < 0 ){
//		assert( innerradius <= -v && -v <= outerradius );
//	} else {
//		assert( innerradius <= v && v <= outerradius );
//	}
	return v;
}
glm::vec3 Random::getvec3(){
	return glm::vec3( getf( -1, 1 ), getf( -1, 1 ), getf( -1, 1 ) );
}

//bool Random::getbool()
//{
//	int i = getint( -1000, 1000 -1 );
//	if( i < 0 )return false;
//	return true;
//}
//Vec3 Random::vec3(){
//	Vec3 value;
//	while( true ){
//		value.setX( getfloat( -1, 1 ) );
//		value.setY( getfloat( -1, 1 ) );
//		value.setZ( getfloat( -1, 1 ) );
//		float l = value.length();
//		if( 0.1f < l ){
//			value /= l;
//			return value;
//		}
//	}
//}
///*
//QVector2D Random::vector2D(qreal maxDist)
//{
//	return QVector2D( getqreal( -maxDist, maxDist ), getqreal( -maxDist, maxDist ) );
//}

//#ifdef ODTQT3D
//QVector3D Random::vector3D( float maxDist )
//{
//	return QVector3D( getqreal( -maxDist, maxDist ), getqreal( -maxDist, maxDist ), getqreal( -maxDist, maxDist ) );
//}

//QVector3D Random::direction()
//{
//	while( true ){
//		QVector3D dir = vector3D( 10.0f );
//		float len = dir.length();
//		if( 0.5f < len ){
//			return dir / len;
//		}
//	}
//}
//#endif
//*/
