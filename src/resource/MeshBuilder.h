#pragma once

#include "ResourceCore.h"
#include "Mesh.h"

namespace MeshBuilder
{
	template< typename V>
	uint32_t getOrCreateVertex( Mesh<V>& mesh, const V& vertexTemplate ){
		for( uint32_t i = 0; i < mesh.vertexCount(); i++ ){
			const V& original = mesh.vertex( i );
			if( original == vertexTemplate )
				return i;
		}
		return mesh.addVertex( vertexTemplate );
	}

	template< typename V>
	void CreateSpherePart( Mesh<V>& mesh, const Vec3& center, const Vec3& n0, const Vec3& n1, const Vec3& n2, float r, int depth, const V& vertexTemplate ){
		if( 0 < depth ){
			depth--;
			Vec3 n01 = ( ( n0 + n1 ) / 2 ).normalized();
			Vec3 n12 = ( ( n1 + n2 ) / 2 ).normalized();
			Vec3 n20 = ( ( n2 + n0 ) / 2 ).normalized();
			CreateSpherePart( mesh, center, n0, n01, n20, r, depth, vertexTemplate );
			CreateSpherePart( mesh, center, n20, n01, n12, r, depth, vertexTemplate );
			CreateSpherePart( mesh, center, n20, n12, n2, r, depth, vertexTemplate );
			CreateSpherePart( mesh, center, n01, n1, n12, r, depth, vertexTemplate );
			return;
		}
		V v0 = vertexTemplate; v0.position = center + n0 * r; v0.normal = n0;
		uint32_t i0 = getOrCreateVertex( mesh, v0 );
		V v1 = vertexTemplate; v1.position = center + n1 * r; v1.normal = n1;
		uint32_t i1 = getOrCreateVertex( mesh, v1 );
		V v2 = vertexTemplate; v2.position = center + n2 * r; v2.normal = n2;
		uint32_t i2 = getOrCreateVertex( mesh, v2 );
		mesh.addIndex( i0, i1, i2 );
	}
	template< typename V>
	void CreateSphere( Mesh<V>& mesh, const Vec3& position, float radius, int depth, const V& vertexTemplate ){
		Vec3 t( 0.0, 1.0, 0.0 );
		Vec3 d( 0.0,-1.0, 0.0 );
		Vec3 l(-1.0, 0.0, 0.0 );
		Vec3 r( 1.0, 0.0, 0.0 );
		Vec3 f( 0.0, 0.0,-1.0 );
		Vec3 b( 0.0, 0.0, 1.0 );
		CreateSpherePart( mesh, position, t, r, f, radius, depth, vertexTemplate );
		CreateSpherePart( mesh, position, t, b, r, radius, depth, vertexTemplate );
		CreateSpherePart( mesh, position, t, l, b, radius, depth, vertexTemplate );
		CreateSpherePart( mesh, position, t, f, l, radius, depth, vertexTemplate );
		CreateSpherePart( mesh, position, d, f, r, radius, depth, vertexTemplate );
		CreateSpherePart( mesh, position, d, r, b, radius, depth, vertexTemplate );
		CreateSpherePart( mesh, position, d, b, l, radius, depth, vertexTemplate );
		CreateSpherePart( mesh, position, d, l, f, radius, depth, vertexTemplate );
	}
	template< typename V>
	uint CreateVertex( Mesh<V>& mesh, const Vec3& position, const Vec3& normal, const Vec2& text, const V& vertexTemplate ){
		V vertex = vertexTemplate;
		vertex.position = position;
		vertex.normal = normal;
		vertex.texCoord = text;
		return mesh.addVertex( vertex );
	}
	template< typename V>
	void CreateRect( Mesh<V>& mesh, uint i00, uint i10, uint i11, uint i01 ){
		mesh.addIndex( i00, i10, i11 );
		mesh.addIndex( i00, i11, i01 );
	}
//	template< typename V>
//	void CreateTriangle( Mesh<V>& mesh, const Vec3& p0, const Vec3& p1, const Vec3& p2, const Vec3& n, const V& vertexTemplate ){
//		uint i0 = CreateVertex( mesh, p0, n, vertexTemplate );
//		uint i1 = CreateVertex( mesh, p1, n, vertexTemplate );
//		uint i2 = CreateVertex( mesh, p2, n, vertexTemplate );
//		mesh.addIndex( i0, i1, i2 );
//	}
//	template< typename V>
//	void CreateTriangle( Mesh<V>& mesh, const Vec3& p0, const Vec3& p1, const Vec3& p2, const V& vertexTemplate ){
//		Vec3 n = Vec3::Cross( ( p1 - p0 ).normalized(), ( p2 - p0 ).normalized() );
//		CreateTriangle( mesh, p0, p1, p2, n, vertexTemplate );
//	}
	template< typename V>
	void CreateRect( Mesh<V>& mesh, const Vec3& p00, const Vec3& p10, const Vec3& p11, const Vec3& p01, const Vec3& n, const V& vertexTemplate ){
		uint i00 = CreateVertex( mesh, p00, n, Vec2( 0, 1 ), vertexTemplate );
		uint i10 = CreateVertex( mesh, p10, n, Vec2( 1, 1 ), vertexTemplate );
		uint i11 = CreateVertex( mesh, p11, n, Vec2( 1, 0 ), vertexTemplate );
		uint i01 = CreateVertex( mesh, p01, n, Vec2( 0, 0 ), vertexTemplate );
		CreateRect( mesh, i00, i10, i11, i01 );
	}
    template< typename V>
    void CreateRectPNT( Mesh<V>& mesh, const Vec3& p00, const Vec3& p10, const Vec3& p11, const Vec3& p01, const Vec3& n, const glm::vec2& t ){
		uint i00 = mesh.addVertex( VertexPNT( p00, n, t ) );
		uint i10 = mesh.addVertex( VertexPNT( p10, n, t ) );
		uint i11 = mesh.addVertex( VertexPNT( p11, n, t ) );
		uint i01 = mesh.addVertex( VertexPNT( p01, n, t ) );
		CreateRect( mesh, i00, i10, i11, i01 );
	}
	template< typename V>
	void CreateBox( Mesh<V>& mesh, const PosOri& posori, const Vec3& radius, const V& vertexTemplate ){
		CreateRect( mesh,
					posori.position() - posori.right() * radius.x() - posori.direction() * radius.y() - posori.up() * radius.z(),
					posori.position() + posori.right() * radius.x() - posori.direction() * radius.y() - posori.up() * radius.z(),
					posori.position() + posori.right() * radius.x() - posori.direction() * radius.y() + posori.up() * radius.z(),
					posori.position() - posori.right() * radius.x() - posori.direction() * radius.y() + posori.up() * radius.z(),
					posori.orientation().direction() * -1,
					vertexTemplate );
		CreateRect( mesh,
					posori.position() + posori.right() * radius.x() + posori.direction() * radius.y() - posori.up() * radius.z(),
					posori.position() - posori.right() * radius.x() + posori.direction() * radius.y() - posori.up() * radius.z(),
					posori.position() - posori.right() * radius.x() + posori.direction() * radius.y() + posori.up() * radius.z(),
					posori.position() + posori.right() * radius.x() + posori.direction() * radius.y() + posori.up() * radius.z(),
					posori.orientation().direction() * 1,
					vertexTemplate );
		CreateRect( mesh,
					posori.position() + posori.right() * radius.x() - posori.direction() * radius.y() - posori.up() * radius.z(),
					posori.position() - posori.right() * radius.x() - posori.direction() * radius.y() - posori.up() * radius.z(),
					posori.position() - posori.right() * radius.x() + posori.direction() * radius.y() - posori.up() * radius.z(),
					posori.position() + posori.right() * radius.x() + posori.direction() * radius.y() - posori.up() * radius.z(),
					posori.orientation().up() * -1,
					vertexTemplate );
		CreateRect( mesh,
					posori.position() - posori.right() * radius.x() - posori.direction() * radius.y() + posori.up() * radius.z(),
					posori.position() + posori.right() * radius.x() - posori.direction() * radius.y() + posori.up() * radius.z(),
					posori.position() + posori.right() * radius.x() + posori.direction() * radius.y() + posori.up() * radius.z(),
					posori.position() - posori.right() * radius.x() + posori.direction() * radius.y() + posori.up() * radius.z(),
					posori.orientation().up() * 1,
					vertexTemplate );
		CreateRect( mesh,
					posori.position() - posori.right() * radius.x() + posori.direction() * radius.y() - posori.up() * radius.z(),
					posori.position() - posori.right() * radius.x() - posori.direction() * radius.y() - posori.up() * radius.z(),
					posori.position() - posori.right() * radius.x() - posori.direction() * radius.y() + posori.up() * radius.z(),
					posori.position() - posori.right() * radius.x() + posori.direction() * radius.y() + posori.up() * radius.z(),
					posori.orientation().right() * -1,
					vertexTemplate );
		CreateRect( mesh,
					posori.position() + posori.right() * radius.x() - posori.direction() * radius.y() - posori.up() * radius.z(),
					posori.position() + posori.right() * radius.x() + posori.direction() * radius.y() - posori.up() * radius.z(),
					posori.position() + posori.right() * radius.x() + posori.direction() * radius.y() + posori.up() * radius.z(),
					posori.position() + posori.right() * radius.x() - posori.direction() * radius.y() + posori.up() * radius.z(),
					posori.orientation().right() * 1,
					vertexTemplate );
	}
	template< typename V>
	void CreatePlane( Mesh<V>& mesh, const PosOri& posori, uint resolution_x, uint resolution_y, float unit, const V& vertexTemplate ){
		uint i = mesh.indexCount();
		V v = vertexTemplate;
		v.normal = posori.up();
		Vec3 lt = posori.position() - posori.right() * ( resolution_x -1 ) * 0.5f * unit - posori.direction() * ( resolution_y -1 ) * 0.5f * unit;
		for( uint x = 0; x < resolution_x; x++ ){
			for( uint y = 0; y < resolution_y; y++ ){
				v.position = lt + posori.right() * x * unit + posori.direction() * y * unit;
				mesh.addVertex( v );
			}
		}
		for( uint x = 0; x+1 < resolution_x; x++ ){
			for( uint y = 0; y+1 < resolution_y; y++ ){
				uint i00 = ( x + 0 ) * resolution_y + ( y + 0 );
				uint i10 = ( x + 1 ) * resolution_y + ( y + 0 );
				uint i01 = ( x + 0 ) * resolution_y + ( y + 1 );
				uint i11 = ( x + 1 ) * resolution_y + ( y + 1 );
				if(
				   ( ( x + 0 ) % 3 == 2 && ( y + 0 ) % 3 == 0 ) ||
				   ( ( x + 0 ) % 3 == 0 && ( y + 0 ) % 3 == 2 )
					){ // das ist fuer TerrainMapSector
					mesh.addIndex( i01, i00, i10 );
					mesh.addIndex( i01, i10, i11 );
				} else if(
					  ( ( x + 0 ) % 3 == 0 && ( y + 0 ) % 3 == 0 )||
					  ( ( x + 0 ) % 3 == 2 && ( y + 0 ) % 3 == 2 )
				){
					mesh.addIndex( i00, i10, i11 );
					mesh.addIndex( i00, i11, i01 );
				} else {
					mesh.addIndex( i00, i10, i11, i01 );
				}
			}
		}
	}
	template< typename V>
	void CreateParticleCross( Mesh<V>& mesh, const Vec3& center, const float radius ){
		{
			uint i00 = mesh.addVertex( VertexPNT( center + Vec3( -radius,  0,  radius ), Vec3( 0, 1, 0 ), Vec2( 0, 0 ) ) );
			uint i10 = mesh.addVertex( VertexPNT( center + Vec3(  radius,  0,  radius ), Vec3( 0, 1, 0 ), Vec2( 1, 0 ) ) );
			uint i01 = mesh.addVertex( VertexPNT( center + Vec3( -radius,  0, -radius ), Vec3( 0, 1, 0 ), Vec2( 0, 1 ) ) );
			uint i11 = mesh.addVertex( VertexPNT( center + Vec3(  radius,  0, -radius ), Vec3( 0, 1, 0 ), Vec2( 1, 1 ) ) );
			mesh.addIndex( i00, i10, i11, i01 );
		}
		{
			uint i00 = mesh.addVertex( VertexPNT( center + Vec3(  0, -radius,  radius ), Vec3( 1, 0, 0 ), Vec2( 0, 0 ) ) );
			uint i10 = mesh.addVertex( VertexPNT( center + Vec3(  0,  radius,  radius ), Vec3( 1, 0, 0 ), Vec2( 1, 0 ) ) );
			uint i01 = mesh.addVertex( VertexPNT( center + Vec3(  0, -radius, -radius ), Vec3( 1, 0, 0 ), Vec2( 0, 1 ) ) );
			uint i11 = mesh.addVertex( VertexPNT( center + Vec3(  0,  radius, -radius ), Vec3( 1, 0, 0 ), Vec2( 1, 1 ) ) );
			mesh.addIndex( i00, i10, i11, i01 );
		}
		{
			uint i00 = mesh.addVertex( VertexPNT( center + Vec3( -radius,  radius,  0 ), Vec3( 0, 0, 1 ), Vec2( 0, 0 ) ) );
			uint i10 = mesh.addVertex( VertexPNT( center + Vec3(  radius,  radius,  0 ), Vec3( 0, 0, 1 ), Vec2( 1, 0 ) ) );
			uint i01 = mesh.addVertex( VertexPNT( center + Vec3( -radius, -radius,  0 ), Vec3( 0, 0, 1 ), Vec2( 0, 1 ) ) );
			uint i11 = mesh.addVertex( VertexPNT( center + Vec3(  radius, -radius,  0 ), Vec3( 0, 0, 1 ), Vec2( 1, 1 ) ) );
			mesh.addIndex( i00, i10, i11, i01 );
		}
	}
	template< typename V>
	void CreateParticleDirected( Mesh<V>& mesh, const Vec3& center, const Vec2& radius ){
		{
			uint i00 = mesh.addVertex( VertexPNT( center + Vec3( -radius.x(),  0,  radius.y() ), Vec3( 0, 1, 0 ), Vec2( 0, 0 ) ) );
			uint i10 = mesh.addVertex( VertexPNT( center + Vec3(  radius.x(),  0,  radius.y() ), Vec3( 0, 1, 0 ), Vec2( 1, 0 ) ) );
			uint i01 = mesh.addVertex( VertexPNT( center + Vec3( -radius.x(),  0, -radius.y() ), Vec3( 0, 1, 0 ), Vec2( 0, 1 ) ) );
			uint i11 = mesh.addVertex( VertexPNT( center + Vec3(  radius.x(),  0, -radius.y() ), Vec3( 0, 1, 0 ), Vec2( 1, 1 ) ) );
			mesh.addIndex( i00, i10, i11, i01 );
		}
		{
			uint i00 = mesh.addVertex( VertexPNT( center + Vec3( -radius.x(),  radius.y(),  0 ), Vec3( 0, 0, 1 ), Vec2( 0, 0 ) ) );
			uint i10 = mesh.addVertex( VertexPNT( center + Vec3(  radius.x(),  radius.y(),  0 ), Vec3( 0, 0, 1 ), Vec2( 1, 0 ) ) );
			uint i01 = mesh.addVertex( VertexPNT( center + Vec3( -radius.x(), -radius.y(),  0 ), Vec3( 0, 0, 1 ), Vec2( 0, 1 ) ) );
			uint i11 = mesh.addVertex( VertexPNT( center + Vec3(  radius.x(), -radius.y(),  0 ), Vec3( 0, 0, 1 ), Vec2( 1, 1 ) ) );
			mesh.addIndex( i00, i10, i11, i01 );
		}
	}
	template< typename V>
	void CreatePipeSegment( Mesh<V>& mesh, const PosOri& posori, float radius, uint32_t sideCount, bool first, bool setNormal, const V& vertexTemplate ){
		uint32_t i0 = mesh.vertexCount();
		for( uint32_t s = 0; s < sideCount; s++ ){
			Mat4 m;
			m.setRotationAtAxis( ( 360.0f * s ) / sideCount, posori.direction() );
			V v = vertexTemplate;
			Vec3 n = m.mapNormal( posori.up() );
			v.position = posori.position() + n * radius;
			if( setNormal ){
				v.normal = n;
			}
			uint32_t s0 = mesh.addVertex( v );
			//logDebug( "  addVertex ", s0 );
			if( !first ){
				uint32_t s1 = i0 + ( s + 1 ) % sideCount;
				uint32_t p0 = s0 - sideCount;
				uint32_t p1 = s1 - sideCount;

				mesh.addIndex( p0, s0, s1, p1 );
				//logDebug( "   addIndex ", p0, s0 );
				// logDebug( "            ", p1, s1 );
				// 0 6 +---+ 3
				//     |  /|
				//     | / |
				// 1 7 +---+ 4
				//     |  /|
				//     | / |
				// 2 8 +---+ 5
				//     |  /|
				//     | / |
				// 0 6 +---+ 3
			}
		}
	}
	template< typename V>
	void CreatePipeSegment( Mesh<V>& mesh, const PosOri& posori, float radius, uint32_t sideCount, bool first, const V& vertexTemplate ){
		CreatePipeSegment<V>( mesh, posori, radius, sideCount, first, true, vertexTemplate );
	}
	template< typename V>
	void CreateLine( Mesh<V>& mesh, const Vec3& pos0, const Vec3& pos1, float radius, uint32_t sideCount, bool setNormal, const V& vertexTemplate ){
		//ASSERT( ( pos1 - pos0 ).length2() > 0.001f );
		Vec3 offset = ( pos1 - pos0 );
		if( offset.length2() <= 0.0001f ){
			logError( "MeshBuilder::CreateLine offset = 0" );
			return;
		}
		Vec3 dir = offset.normalized();
		Vec3 rig = Vec3::Cross( dir, Vec3( dir.z(), dir.x(), dir.y() ) ).normalized();
		Vec3 up = Vec3::Cross( rig, dir ).normalized();
		PosOri posori( pos0, dir, up );
		CreatePipeSegment<V>( mesh, posori, radius, sideCount, true, setNormal, vertexTemplate );
		posori.setPosition( pos1 );
		CreatePipeSegment<V>( mesh, posori, radius, sideCount, false, setNormal, vertexTemplate );
	}
	template< typename V>
	void CreatePosOri( Mesh<V>& mesh, const PosOri& posori, float length, float radius, uint32_t sideCount, bool setNormal, const V& avertexTemplate ){
		V vertexTemplate = avertexTemplate;
		if( !setNormal )
			vertexTemplate.normal = glm::vec3( 0.3f, 1, 0.3f );
		CreateLine( mesh, posori.position(), posori.position() + posori.direction() * length, radius, sideCount, setNormal, vertexTemplate );
        if( !setNormal )
            vertexTemplate.normal = glm::vec3( 1, 0, 0 );
		CreateLine( mesh, posori.position() - posori.right() * length * 0.35f, posori.position() + posori.right() * length * 0.35f, radius, sideCount, setNormal, vertexTemplate );
		if( !setNormal )
            vertexTemplate.normal = glm::vec3( 0, 0, 1 );
		CreateLine( mesh, posori.position(), posori.position() + posori.up() * length * 0.65f, radius, sideCount, setNormal, vertexTemplate );
	}

//	template< typename V>
//	void CreateBoxRounded( Mesh<V>& mesh, const PosOri& posori, const Vec3& radius, const float b, const V& vertexTemplate ){
//		assert( 0 == mesh.vertexCount() );
//		CreateRect( mesh,
//		/*   0 */	posori.position() - posori.right() * ( radius.x() -b ) - posori.direction() * ( radius.y() ) - posori.up() * ( radius.z() -b ),
//		/*   1 */	posori.position() + posori.right() * ( radius.x() -b ) - posori.direction() * ( radius.y() ) - posori.up() * ( radius.z() -b ),
//		/*   2 */	posori.position() + posori.right() * ( radius.x() -b ) - posori.direction() * ( radius.y() ) + posori.up() * ( radius.z() -b ),
//		/*   3 */	posori.position() - posori.right() * ( radius.x() -b ) - posori.direction() * ( radius.y() ) + posori.up() * ( radius.z() -b ),
//					posori.orientation().direction() * -1,
//					vertexTemplate );
//		CreateRect( mesh,
//		/*   4 */	posori.position() + posori.right() * ( radius.x() -b ) + posori.direction() * ( radius.y() ) - posori.up() * ( radius.z() -b ),
//		/*   5 */	posori.position() - posori.right() * ( radius.x() -b ) + posori.direction() * ( radius.y() ) - posori.up() * ( radius.z() -b ),
//		/*   6 */	posori.position() - posori.right() * ( radius.x() -b ) + posori.direction() * ( radius.y() ) + posori.up() * ( radius.z() -b ),
//		/*   7 */	posori.position() + posori.right() * ( radius.x() -b ) + posori.direction() * ( radius.y() ) + posori.up() * ( radius.z() -b ),
//					posori.orientation().direction() * 1,
//					vertexTemplate );
//		CreateRect( mesh,
//		/*   8 */	posori.position() + posori.right() * ( radius.x() -b ) - posori.direction() * ( radius.y() -b ) - posori.up() * ( radius.z() ),
//		/*   9 */	posori.position() - posori.right() * ( radius.x() -b ) - posori.direction() * ( radius.y() -b ) - posori.up() * ( radius.z() ),
//		/*  10 */	posori.position() - posori.right() * ( radius.x() -b ) + posori.direction() * ( radius.y() -b ) - posori.up() * ( radius.z() ),
//		/*  11 */	posori.position() + posori.right() * ( radius.x() -b ) + posori.direction() * ( radius.y() -b ) - posori.up() * ( radius.z() ),
//					posori.orientation().up() * -1,
//					vertexTemplate );
//		CreateRect( mesh,
//		/*  12 */	posori.position() - posori.right() * ( radius.x() -b ) - posori.direction() * ( radius.y() -b ) + posori.up() * ( radius.z() ),
//		/*  13 */	posori.position() + posori.right() * ( radius.x() -b ) - posori.direction() * ( radius.y() -b ) + posori.up() * ( radius.z() ),
//		/*  14 */	posori.position() + posori.right() * ( radius.x() -b ) + posori.direction() * ( radius.y() -b ) + posori.up() * ( radius.z() ),
//		/*  15 */	posori.position() - posori.right() * ( radius.x() -b ) + posori.direction() * ( radius.y() -b ) + posori.up() * ( radius.z() ),
//					posori.orientation().up() * 1,
//					vertexTemplate );

//		CreateRect( mesh,
//		/*  16 */	posori.position() - posori.right() * ( radius.x() ) + posori.direction() * ( radius.y() -b ) - posori.up() * ( radius.z() -b ),
//		/*  17 */	posori.position() - posori.right() * ( radius.x() ) - posori.direction() * ( radius.y() -b ) - posori.up() * ( radius.z() -b ),
//		/*  18 */	posori.position() - posori.right() * ( radius.x() ) - posori.direction() * ( radius.y() -b ) + posori.up() * ( radius.z() -b ),
//		/*  19 */	posori.position() - posori.right() * ( radius.x() ) + posori.direction() * ( radius.y() -b ) + posori.up() * ( radius.z() -b ),
//					posori.orientation().right() * -1,
//					vertexTemplate );
//		CreateRect( mesh,
//		/*  20 */	posori.position() + posori.right() * ( radius.x() ) - posori.direction() * ( radius.y() -b ) - posori.up() * ( radius.z() -b ),
//		/*  21 */	posori.position() + posori.right() * ( radius.x() ) + posori.direction() * ( radius.y() -b ) - posori.up() * ( radius.z() -b ),
//		/*  22 */	posori.position() + posori.right() * ( radius.x() ) + posori.direction() * ( radius.y() -b ) + posori.up() * ( radius.z() -b ),
//		/*  23 */	posori.position() + posori.right() * ( radius.x() ) - posori.direction() * ( radius.y() -b ) + posori.up() * ( radius.z() -b ),
//					posori.orientation().right() * 1,
//					vertexTemplate
//					);
//		CreateRect( mesh, 3, 2, 13, 12 );
//		CreateRect( mesh, 8, 9, 0, 1 );
//		CreateRect( mesh, 23, 22, 14, 13 );
//		CreateRect( mesh, 7, 6, 15, 14 );
//		CreateRect( mesh, 12, 15, 19, 18 );
//		CreateRect( mesh, 3, 18, 17, 0 );
//		CreateRect( mesh, 23, 2, 1, 20 );
//		CreateRect( mesh, 7, 22, 21, 4 );
//		CreateRect( mesh, 6, 5, 16, 19 );

//		CreateRect( mesh, 1, 0, 9, 8 );
//		CreateRect( mesh, 21, 20, 8, 11 );
//		CreateRect( mesh, 17, 16, 10, 9 );
//		CreateRect( mesh, 11, 10, 5, 4 );

//		mesh.addIndex( 23, 13, 2 );
//		mesh.addIndex( 12, 18, 3 );
//		mesh.addIndex( 19, 15, 6 );
//		mesh.addIndex( 14, 22, 7 );

//		mesh.addIndex( 1, 8, 20 );
//		mesh.addIndex( 21, 11, 4 );
//		mesh.addIndex( 16, 5, 10 );
//		mesh.addIndex( 17, 9, 0 );

//		//CreateRect( mesh, 17, 16, 10, 9 );


//		//mesh.addIndex( i + 0, i + 4 * 4 + 1, i + 2 * 4 + 1 );
////#define i( rect, index )( start_index + rect * 4 + index )
//	//	CreateRect( mesh, i( 0, 0 ), i( 0, 1 ), i( 3, 0 ), i( 3, 3 ) );
////		mesh.addIndex( i + 1, i + 4 * 4 + 2, i + 2 * 4 + 1 );


//	}
//	template< typename VS, typename VT>
//	Mesh<VT>* Convert( const Mesh<VS>& source, std::function<void(VT&,const VS&)> vc ){
//		Mesh<VT>* target = new Mesh<VT>();
//		for( uint v = 0; v < source.vertexCount(); v++ ){
//			const VS& vs = source.vertex( v );
//			VT vt;
//			vc( vt, vs );
//			target->addVertex( vt );
//		}
//		for( uint i = 0; i < source.indexCount(); i++ ){
//			target->addIndex( source.index( i ) );
//		}
//		return target;
//	}

//	template< typename V>
//	Mesh<V>* Merge( const List<Mesh<V>*>& sources ){
//		assert( !sources.isEmpty() );
//		int total_vertex_count = 0;
//		int total_index_count = 0;
//		for( auto source : sources ){
//			total_vertex_count += source->vertexCount();
//			total_index_count += source->indexCount();
//		}
//		Mesh<V>* target = new Mesh<V>( total_vertex_count, total_index_count );
//		for( auto source : sources ){
//			int vertex_offset = target->vertexCount();
//			const V* src_vertex = &source->vertex( 0 );
//			for( uint v = 0; v < source->vertexCount(); v++ ){
//				target->addVertex( *src_vertex );
//				src_vertex++;
//			}
//			for( uint i = 0; i < source->indexCount(); i++ ){
//				target->addIndex( vertex_offset + source->index( i ) );
//			}
//		}
//		return target;
//	}

//	template< typename V>
//	Mesh<V>* Extract( const Mesh<V>& source, std::function<bool( const V& vertex )> filter ){
//		struct VertexUsage {
//			uint8_t usage = 0; // 0 == not used, 1 == use by filter, 2 == use by dependency
//			uint32_t target_index = 0;
//		};
//		Array<VertexUsage> vertex_usage( source.vertexCount() );
//		for( uint v = 0; v < source.vertexCount(); v++ ){
//			bool inc = filter( source.vertex( v ) );
//			if( inc )
//				vertex_usage[ v ].usage = 1;
//		}
//		for( uint i = 0; i < source.indexCount(); i += 3 ){
//			uint v0 = source.index( i + 0 );
//			uint v1 = source.index( i + 1 );
//			uint v2 = source.index( i + 2 );
//			VertexUsage& u0 = vertex_usage[ v0 ];
//			VertexUsage& u1 = vertex_usage[ v1 ];
//			VertexUsage& u2 = vertex_usage[ v2 ];
//			if( u0.usage == 1 || u1.usage == 1 || u2.usage == 1 ){
//				if( u0.usage == 0 )u0.usage = 2;
//				if( u1.usage == 0 )u1.usage = 2;
//				if( u2.usage == 0 )u2.usage = 2;
//			}
//		}
//		Mesh<V>* target = new Mesh<V>();
//		for( uint v = 0; v < source.vertexCount(); v++ ){
//			VertexUsage& u = vertex_usage[ v ];
//			if( 0 < u.usage ){
//				const V& vertex = source.vertex( v );
//				u.target_index = target->addVertex( vertex );
//			}
//		}
//		for( uint i = 0; i < source.indexCount(); i += 3 ){
//			uint v0 = source.index( i + 0 );
//			uint v1 = source.index( i + 1 );
//			uint v2 = source.index( i + 2 );
//			VertexUsage& u0 = vertex_usage[ v0 ];
//			VertexUsage& u1 = vertex_usage[ v1 ];
//			VertexUsage& u2 = vertex_usage[ v2 ];
//			if( 0 < u0.usage && 0 < u1.usage && 0 < u2.usage ){
//				target->addIndex( u0.target_index, u1.target_index, u2.target_index );
//			}
//		}
//		return target;
//	}
	template< typename V>
    Mesh<V>* FilteredByVertex( const MeshPNT& src, std::function<bool( int v, const VertexPNT& vertex )> filter, bool assertAllTrianglesAreSame = true ){
		bool oks[ src.vertexCount() ];
		uint32_t v2trg[ src.vertexCount() ];
		Mesh<V>* trg = new Mesh<V>();
		for( int v = 0; v < src.vertexCount(); v++ ){
			const VertexPNT& vertex = src.vertex( v );
			bool ok = filter( v, vertex );
			oks[ v ] = ok;
			if( ok ){
				v2trg[ v ] = trg->addVertex( vertex );
			}
		}
		for( int i = 0; i < src.indexCount(); i += 3 ){
			uint32_t i0 = src.index( i +0 );
			uint32_t i1 = src.index( i +1 );
			uint32_t i2 = src.index( i +2 );
            if( assertAllTrianglesAreSame ){
                assert( oks[ i0 ] == oks[ i1 ] && oks[ i0 ] == oks[ i2 ] );
            }
            if( oks[ i0 ] && oks[ i1 ] && oks[ i2 ] ){
				trg->addIndex( v2trg[ i0 ] );
				trg->addIndex( v2trg[ i1 ] );
				trg->addIndex( v2trg[ i2 ] );
			}
		}
		logDebug( "Filtered from", src.vertexCount(), "/", src.indexCount(), "to", trg->vertexCount(), "/", trg->indexCount() );
		return trg;
	}
}
