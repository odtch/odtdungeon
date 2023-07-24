#pragma once

#include "MathCore.h"
#include "Orientation.h"
#include "Mat4.h"

struct PosOri {
private:
	Vec3 _position;
	Orientation _orientation;
private:
	mutable bool _matrix_valid;
	mutable Mat4 _matrix;
public:
	explicit PosOri();
	explicit PosOri( const Vec3& position, const Orientation& orientation );
	explicit PosOri( const Vec3& position, const Vec3& direction, const Vec3& up );
	PosOri( const PosOri& src );
	explicit PosOri( const Mat4& src );
public:
	const Vec3& position() const { return _position; }
	void setPosition( const Vec3& position );
public:
	const Orientation& orientation() const{ return _orientation; }
	void setOrientation( const Orientation& orientation );
public:
	const Vec3& direction() const { matrix(); return _orientation.direction(); }
	const Vec3& right() const { matrix(); return _orientation.right(); }
	const Vec3& up() const { matrix(); return _orientation.up(); }
public:
	const Mat4& matrix() const;
public:
	void set( const Vec3& position, const Vec3& direction, const Vec3& up );
	void set( const Vec3& position, const Orientation& orientation );
	void set( const Mat4& matrix );
public:
	void operator=( const PosOri& posori );
public:
	void translate( const Vec3& offset );
	PosOri translated( const Vec3& offset ) const;
	void translateLocal( const Vec3& offset );
	PosOri translatedLocal( const Vec3& offset ) const;
	void rotate( float angle, const Vec3& axis );
	PosOri rotated( float angle, const Vec3& axis ) const;
	void rotate( const glm::quat& rotation );
	PosOri rotated( const glm::quat& rotation ) const;
public:
	void setZto0();
//public:
//	void save( BinaryFileWriter& writer ) const;
//	void load( BinaryFileReader& reader );
public:
	static bool Equals( const PosOri& a, const PosOri& b, float max_pos_dist = 0.001f, float max_ori_dist = 0.001f );
public:
	static PosOri Interpolated( const PosOri& a, const PosOri& b, float f );
	static PosOri AbsoluteToRelative( const PosOri& parentposori, const PosOri& absoluteposori );
	static PosOri AbsoluteToRelative( const Mat4& parentmatrix, const Mat4& absolutematrix );

#ifdef ODTJOLT
public:
	void set( JPH::Mat44 mat44 );
#endif
public:
	std::string toString() const;
	std::string toCode() const;
};

