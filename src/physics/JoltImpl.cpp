#include "stdodt.h"
#include "Physics.h"
#include "OdtPhysics.h"
#include "JoltImpl.h"

void setPosOriFromJPH( PosOri& posori, JPH::Mat44 mat44 ){
	posori.set( mat44 );
}

void setMat4FromJPH( Mat4& t, JPH::Mat44 mat44 ){
	//float & d = ((JPH::Mat44)mat44)(0,0);
	float & d = mat44(0,0);
	float& td = t(0,0);
	const float* dp = &d;
	float* tp = &td;
	for( int i = 0; i < 4 * 4; i++ ){
		(*tp) = (*dp);
		//logDebug( i, (*tp) );
		tp++; dp++;
	}
}

//void addToIgnoreMeAndConnectedFilter( JPH::IgnoreMultipleBodiesFilter& filter, Object* object ){
//	ASSERT( object );
//	for( ObjectProperty* property : object->properties() ){
//		PhysicsBody* body = dynamic_cast<PhysicsBody*>( property );
//		if( body ){
//			filter.IgnoreBody( body->body()->GetID() );
//		}
//		ObjectConnector* connector = dynamic_cast<ObjectConnector*>( property );
//		if( connector ){
//			for( ObjectConnection* connection : connector->connections() ){
//				if( connection->parentConnector() == connector ){
//					addToIgnoreMeAndConnectedFilter( filter, connection->childObject() );
//				}
//			}
//		}
//	}
//}
