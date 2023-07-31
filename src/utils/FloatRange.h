#pragma once

class FloatRange {
private:
	float _min;
	float _max;
	float _mintomax;
public:
	explicit FloatRange();
	explicit FloatRange( const FloatRange& values );
	explicit FloatRange( float min, float max );
	virtual ~FloatRange();
public:
	void reset();
public:
	float min() const{ return _min; }
	void setMin( float min );
public:
	float max() const{ return _max; }
	void setMax( float max );
public:
	float minToMax() const{ return _mintomax; }
	float valuetofactor( float value ) const;
	float factortovalue( float factor ) const;
public:
	void set( float min, float max );
	void adjust( float value );
	float adjusted( float value ) const;
public:
	bool isInside( float value ) const;
public:
//	QString toString() const;
public:
	void operator=( const FloatRange& range );
private:
	void recalc();
};
