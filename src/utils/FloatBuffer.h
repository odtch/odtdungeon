#pragma once

#include "Debugging.h"
#include "FloatRange.h"
class Vec3;

class FloatBuffer
{
	DEBUGCOUNTER( FloatBuffer );
private:
	float* _datas;
	int _size;
	float* _last;
public:
	explicit FloatBuffer( int size );
	FloatBuffer( const FloatBuffer& tocopy );
	~FloatBuffer();
private:
	void allocate( int size );
public:
	int size() const{ return _size; }
public:
	float* data() const{ return _datas; }
	float* first() const{ return _datas; }
	float* last() const{ return _last; }
public:
	float operator[]( int i ) const { ASSERT( i >= 0 && i < _size ); return _datas[i]; }
	float& operator[]( int i ){ ASSERT( i >= 0 && i < _size ); return _datas[i]; }
	float get( int i ) const { ASSERT( i >= 0 && i < _size ); return _datas[i]; }
	float* getPtr( int i ) const { ASSERT( i >= 0 && i < _size ); return &_datas[i]; }
public:
	void clear();
	void operator=( const FloatBuffer& tocopy );
	void copyFrom( const float* others );
	void copyFrom( const FloatBuffer& other );
//	void copyFrom( const QList<float>& values );
	void copyFrom( int at, const FloatBuffer& other );
	void setAll( float value );
	void set( int index, float value );
//	void add( float value );
//public:
//	void setFromDoubles( const double* source );
//	void copyToDoubles( double* target ) const;
//public:
////	void exportCsv( QTextStream& stream )const;
////	QString toString() const;
//public:
//	static float DiffSquared( const FloatBuffer& a, const FloatBuffer& b );
//	float equals( const FloatBuffer& other, float maxdiff ) const;
//	bool equals2( const FloatBuffer& other, float maxdiff = 0.01f ) const;
//	float equalsSquared( const FloatBuffer& other, float maxdiff ) const;
//public:
//	float min() const;
//	float max() const;
//	int maxindex() const;
//	float sum() const;
//public:
//	void setMaxTo1RestTo0( int start, int count );
//public:
//	void softmaximize();
//	int getIndexOfSoftmax( float value ) const;
//	int getIndexOfMax() const;
//	int getIndexOfMax( int s, int e ) const;
//public:
////	void serialize( SerializationStream& stream ) const;
////	void serializeData( SerializationStream& stream ) const;
////	void deserializeData( DeserializationStream& stream );
////	static FloatBuffer Deserialize( DeserializationStream& stream );
};

class FloatBufferReader {
	DEBUGCOUNTER( FloatBufferReader );
private:
	const FloatBuffer* _buffer;
	float* _index;
	float* _last;
public:
	FloatBufferReader( const FloatBuffer* buffer );
	~FloatBufferReader();
public:
	bool atEnd() const{ return _index > _last; }
	float get();
};

class FloatBufferWriter {
	DEBUGCOUNTER( FloatBufferWriter );
private:
	FloatBuffer* _buffer;
	float* _index;
	float* _last;
public:
	FloatBufferWriter( FloatBuffer* buffer );
	~FloatBufferWriter();
public:
	bool atEnd() const{ return _index > _last; }
	void add( float v );
	void add( const Vec3& v );
//	void add( float value, const FloatRange& valueRange, int rangeCount );
//	void add2( float value, const FloatRange& valueRange, int rangeCount );
////	void add( const QVector2D& vector );
////	void add( const QVector3D& vector );
//	void add( const Vec3& data );
//	void add( const FloatBuffer& data );
};

////class FloatBufferWithHistory {
////	DEBUGCOUNTER( FloatBufferWithHistory );
////private:
////	QList<FloatBuffer*> _history;
////	FloatBuffer* _act;
////public:
////	explicit FloatBufferWithHistory( int size, int historySize );
////	~FloatBufferWithHistory();
////public:
////	qreal get( int index );
////	void set( int index, qreal value );
////public:
////	FloatBuffer* act(){ return _act; }
////	void pushActToHistory();
////};

//class FloatBufferNormalizer
//{
//private:
//	enum { Max = 100 };
//	int _size;
//	FloatRange _ranges[ Max ];
//public:
//	explicit FloatBufferNormalizer( int size );
//	~FloatBufferNormalizer();
//public:
//	void adjust( const FloatBuffer& values );
//	void normalize( const FloatBuffer& values, FloatBuffer& factors );
//	void denormalize( const FloatBuffer& factors, FloatBuffer& values ) const;

//};

//private:
//	Array* _data = null;
//public:
//	explicit FloatBuffer();
//	explicit FloatBuffer( const FloatBuffer& values );
//	virtual ~FloatBuffer();
//public:
//	void reset();
//	void reset( float avg );
//	void set( float min, float max, float sum, int count);
//public:
//	void add( float value );
//	void add( const FloatBuffer& other );
//public:
//	float min() const;
//	float max() const;
//	float minToMax() const;
//	float sum() const;
//	int count() const;
//	float avg() const;
//public:
//	void adjustMinMax( float min, float max );
//public:
//	bool isInside( float value ) const;
//public:
//	float toFactor( float value ) const;
//public:
//	std::string toString() const;
////#ifdef ODTSERIALIZATION
////public:
////	explicit FloatBuffer( DeserializationStream& stream );
////public:
////	void deserialize( DeserializationStream& stream );
////	void serialize( SerializationStream& stream ) const;
////#endif
//};

//inline float FloatBuffer::min() const{ return _min; }

//inline float FloatBuffer::max() const{ return _max; }

//inline float FloatBuffer::minToMax() const{ return max() - min(); }

//inline float FloatBuffer::sum() const{ return _sum; }

//inline int FloatBuffer::count() const{ return _count; }

//inline float FloatBuffer::avg() const{ if( _count < 1 )return 0.0f; return _sum / _count; }

