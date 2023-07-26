#pragma once

#include "CharCore.h"

static const int CharBoneCount = 15;

class CharPose
{
	DEBUGCOUNTER( CharPose );
private:
	Vec3 _translation;
private:
	glm::quat _rotations[ CharBoneCount ];
public:
	explicit CharPose();
	explicit CharPose( const CharPose& src );
	~CharPose();
public:
	const Vec3& translation() const{ return _translation; }
	void setTranslation( const Vec3& rootPosition );
public:
	const glm::quat& rotation( int bone_index ) const{
		assert( 0 <= bone_index && bone_index < CharBoneCount );
		return _rotations[ bone_index ];
	}
	void setRotation( int bone_index, const glm::quat& rotation );
public:
	CharPose* clone() const;
	void operator=( const CharPose& pose );
	void set( const CharPose& pose );
	void interpolate( const CharPose& from, float factor_01, const CharPose& to );
public:
	void load( BinaryFileReader& reader );
	void save( BinaryFileWriter& writer ) const;
public:
	friend class CharImporter;
};

