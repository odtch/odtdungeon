#include "stdodt.h"
#include "CharBrain.h"
#include "CharRagdoll.h"
#include "CharPose.h"
#include "CharCharacterType.h"


uint CharBrain::InputSize(){
	return 1
			+3
			+ CharBoneCount * 3 //4
			;
}
uint CharBrain::OutputSize(){
	return 3 + CharBoneCount * 3 //4
			;
}
float ni( float e ){
	float n = ( M_PI + e ) / ( 2 * M_PI );
	ASSERT( 0 <= n && n <= 1 );
	return n;
}
float no( float n ){
	ASSERT( 0 <= n && n <= 1 );
	float e = n * ( 2 * M_PI ) - M_PI;
	return e;
}
void CharBrain::LoadInput( FloatBuffer& input, float control_move_y, const CharPose& pose ){
	FloatBufferWriter inw( &input );
	inw.add( control_move_y );
	inw.add( pose.translation().x() );
	inw.add( pose.translation().y() );
	inw.add( pose.translation().z() );
	for( int r = 0; r < CharBoneCount; r++ ){
		const glm::quat& q = pose.rotation( r );
//		inw.add( q.x );
//		inw.add( q.y );
//		inw.add( q.z );
//		inw.add( q.w );
		glm::vec3 e = glm::eulerAngles(q);
		inw.add( ni( e.x ) );
		inw.add( ni( e.y ) );
		inw.add( ni( e.z ) );
	}
}
void CharBrain::LoadOutput( FloatBuffer& output, const CharPose& pose ){
	FloatBufferWriter inw( &output );
	inw.add( pose.translation().x() );
	inw.add( pose.translation().y() );
	inw.add( pose.translation().z() );
	for( int r = 0; r < CharBoneCount; r++ ){
		const glm::quat& q = pose.rotation( r );
//		inw.add( q.x );
//		inw.add( q.y );
//		inw.add( q.z );
//		inw.add( q.w );
		glm::vec3 e = glm::eulerAngles(q);
		inw.add( ni( e.x ) );
		inw.add( ni( e.y ) );
		inw.add( ni( e.z ) );
	}
}
void CharBrain::LoadOutput( CharPose& pose, const FloatBuffer& output ){
	FloatBufferReader r( &output );
	float tx = r.get();
	float ty = r.get();
	float tz = r.get();
	pose.setTranslation( Vec3( tx, ty, tz ) );
	for( int ri = 0; ri < CharBoneCount; ri++ ){
		glm::vec3 e;
		e.x = no( r.get() );
		e.y = no( r.get() );
		e.z = no( r.get() );
//		glm::quat q;
//		q.x = r.get();
//		q.y = r.get();
//		q.z = r.get();
//		q.w = r.get();
		glm::quat q = glm::quat(e);
		pose.setRotation( ri, q );
	}
}
