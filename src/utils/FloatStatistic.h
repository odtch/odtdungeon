#pragma once

//#include "MathCore.h"
//#include "core/OdtCore.h"
//#ifdef ODTSERIALIZATION
//#include "base/serialization/Serialization.h"
//#endif

class FloatStatistic {
//	DEBUGCOUNTER( FloatStatistic );
private:
	float _min;
	float _max;
	float _sum;
	int _count;
public:
	explicit FloatStatistic();
	explicit FloatStatistic( const FloatStatistic& values );
	virtual ~FloatStatistic();
public:
	void reset();
	void reset( float avg );
	void set( float min, float max, float sum, int count);
public:
	void add( float value );
	void add( const FloatStatistic& other );
public:
	float min() const;
	float max() const;
	float minToMax() const;
	float sum() const;
	int count() const;
	float avg() const;
public:
	void adjustMinMax( float min, float max );
public:
	bool isInside( float value ) const;
public:
	float toFactor( float value ) const;
public:
	std::string toString() const;
//#ifdef ODTSERIALIZATION
//public:
//	explicit FloatStatistic( DeserializationStream& stream );
//public:
//	void deserialize( DeserializationStream& stream );
//	void serialize( SerializationStream& stream ) const;
//#endif
};

inline float FloatStatistic::min() const{ return _min; }

inline float FloatStatistic::max() const{ return _max; }

inline float FloatStatistic::minToMax() const{ return max() - min(); }

inline float FloatStatistic::sum() const{ return _sum; }

inline int FloatStatistic::count() const{ return _count; }

inline float FloatStatistic::avg() const{ if( _count < 1 )return 0.0f; return _sum / _count; }

