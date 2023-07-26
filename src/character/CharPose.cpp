#include "stdodt.h"
#include "CharPose.h"

CharPose::CharPose(){
}
CharPose::CharPose(const CharPose& src){
	set( src );
}
CharPose::~CharPose(){
}
void CharPose::setTranslation( const Vec3& rootPosition ){
	_translation = rootPosition;
}
void CharPose::setRotation(int bone_index, const glm::quat& rotation){
	assert( 0 <= bone_index && bone_index < CharBoneCount );
	_rotations[ bone_index ] = rotation;
}
CharPose* CharPose::clone() const {
	return new CharPose( *this );
}
void CharPose::operator=(const CharPose& pose){
	set( pose );
}
void CharPose::set(const CharPose& pose){
	_translation = pose._translation;
	for( int l = 0; l < CharBoneCount; l++ ){
		_rotations[ l ] = pose._rotations[ l ];
	}
}
void CharPose::interpolate( const CharPose& from, float factor_01, const CharPose& to ){
	assert( 0 <= factor_01 && factor_01 <= 1.0f );
	float factor_10 = 1.0f - factor_01;
	_translation = from._translation * factor_01 + to._translation * factor_10;
	for( int l = 0; l < CharBoneCount; l++ ){
		_rotations[ l ] = glm::slerp( from._rotations[ l ], to._rotations[ l ], factor_10 );
	}
}
void CharPose::load( BinaryFileReader& reader ){
	_translation = reader.read_vec3();
	for( int r = 0; r < CharBoneCount; r++ ){
		glm::quat& rotation = _rotations[ r ];
		rotation.w = reader.read_float();
		rotation.x = reader.read_float();
		rotation.y = reader.read_float();
		rotation.z = reader.read_float();
	}
}
void CharPose::save( BinaryFileWriter& writer ) const {
	writer.write_vec3( _translation );
	for( int r = 0; r < CharBoneCount; r++ ){
		const glm::quat& rotation = _rotations[ r ];
		writer.write_float( rotation.w );
		writer.write_float( rotation.x );
		writer.write_float( rotation.y );
		writer.write_float( rotation.z );
	}
}
//void CharPose::serialize( SerializationStream &stream ){
//	stream.writeQString( _name );
//	stream.writeArraySize( _values.size() );
//	_values.serializeData( stream );
//}
//CharPose *CharPose::Deserialize( DeserializationStream& stream ){
//	QString name = stream.loadQString();
//	int valueCount = stream.loadArraySize();
//	CharPose* pose = new CharPose( name, valueCount );
//	pose->_values.deserializeData( stream );
//	return pose;
//}

