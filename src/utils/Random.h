#pragma once

#include "Debugging.h"
#include "List.h"
//#include "math/Vec3.h"

class Random
{
	DEBUGCOUNTER( Random );
private:
	int _gen1;
	int _gen2;
	int _max;
	int _seed;
public:
	explicit Random();
	explicit Random( int aGen1, int aMax );
	~Random();
public:
	void init( int aGen1, int aMax );
	void reset();
public:
	int geti();
	int geti( int min, int max );
public:
	float getf(){ return getf( 0, 1 ); }
	float getf( float min, float max );
public:
	float getf_ring( float innerradius, float outerradius ); // returns in range -out to -in or in to out
public:
	bool getb();
public:
	glm::vec3 getvec3(); // returns in range -1 to 1 with length 0 to ~1.7
public:
	template<class T>
	T* oneof( const List<T*>& list ){
		if( list.isEmpty() ){
			assert( false );
			geti();
			return null;
		} else if( list.size() == 1 ){
			geti();
			return list.get( 0 );
		} else {
			return list.get( geti( 0, list.size()-1 ) );
		}
	}	
//public:
//	Vec3 vec3();  // -1 to 1, len = 1
//public:
//	QVector2D vector2D( qreal maxDist );

//#ifdef ODTQT3D
//public:
//	QVector3D vector3D( float maxDist );
//	QVector3D direction();
//#endif

};
