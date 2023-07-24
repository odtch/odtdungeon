#ifndef VRT_OHITCALC_H
#define VRT_OHITCALC_H

void OHitCalc( out OHit hit ){
	hit.instance_index = globals.layers[ pld.layer_index ].first_instance_index + gl_InstanceID;
	hit.material_index = instances[ hit.instance_index ].materialIndex;
	hit.material_flags = materials[ hit.material_index ].flags;
	const int primitiveID = gl_PrimitiveID; // Get the ID of the triangle
	const uint mesh_index = instances[ hit.instance_index ].meshIndex;
	const uint i0 = indicesBuffer[ mesh_index ].indices[ 3 * primitiveID + 0 ];
	const uint i1 = indicesBuffer[ mesh_index ].indices[ 3 * primitiveID + 1 ];
	const uint i2 = indicesBuffer[ mesh_index ].indices[ 3 * primitiveID + 2 ];
	hit.vertex[0] = verticesBuffer[ mesh_index ].vertices[ i0 ];
	hit.vertex[1] = verticesBuffer[ mesh_index ].vertices[ i1 ];
	hit.vertex[2] = verticesBuffer[ mesh_index ].vertices[ i2 ];
	hit.ray_direction = gl_WorldRayDirectionEXT;
	hit.ray_distance = gl_HitTEXT;
	hit.barycentrics = vec3( 1.0 - attributes.x - attributes.y, attributes.x, attributes.y );
	hit.object_position = hit.vertex[0].pos * hit.barycentrics.x + hit.vertex[1].pos * hit.barycentrics.y + hit.vertex[2].pos * hit.barycentrics.z;
	if( 0 != ( hit.material_flags & MaterialFlag_CalcNormalFromTriangle ) ){
		hit.object_normal = cross( hit.vertex[1].pos - hit.vertex[0].pos, hit.vertex[2].pos - hit.vertex[0].pos );
	} else {
		hit.object_normal = hit.vertex[0].nrm * hit.barycentrics.x + hit.vertex[1].nrm * hit.barycentrics.y + hit.vertex[2].nrm * hit.barycentrics.z;
	}
	hit.world_position = gl_ObjectToWorldEXT * vec4( hit.object_position, 1.0f );;
	hit.world_normal = normalize( ( hit.object_normal * gl_WorldToObjectEXT ).xyz );
	hit.texture_coord = hit.vertex[0].txt * hit.barycentrics.x + hit.vertex[1].txt * hit.barycentrics.y + hit.vertex[2].txt * hit.barycentrics.z;
	hit.texture_coord.y += instances[ hit.instance_index ].textureOffsetY;
}

#endif
