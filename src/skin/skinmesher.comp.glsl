#version 430
#extension GL_EXT_debug_printf : require

// # extension GL_ARB_separate_shader_objects : enable

// # define WORKGROUP_SIZE 32

layout( set = 0, binding = 0, std430 ) readonly buffer Matrices
{
    uint count; float filler11; float filler12; float filler13;
    mat4x4 matrices[100];
    float a; float b;
//    float b;
} matricesBuffer;

struct VertexSkin {
    vec3 pos;
    vec3 nrm;
    vec2 txt;
    vec3 bone_weights;
    ivec3 bone_indexes;
};
layout( set = 0, binding = 1, std430 ) readonly buffer Inputs
{
    VertexSkin vertices[];
} source_mesh;

struct VertexPNT {
    vec3 pos;
    vec3 nrm;
    vec2 txt;
};
layout( set = 0, binding = 2, std430 ) buffer Outputs
{
    VertexPNT vertices[];
} target_mesh;

void main()
{
//    if( gl_GlobalInvocationID.x == 0 ){
//	//debugPrintfEXT( "testcompute %d %d %d\n", gl_GlobalInvocationID.x, matricesBuffer.count, matricesBuffer.count2 );
//	for( uint v = 0; v < 4; v++ ){
////	    debugPrintfEXT( "shd %d  %f %f %f  %f %f %f %d %d\n",
////			v,
////		    source_mesh.vertices[v].pos.x, source_mesh.vertices[v].pos.y, source_mesh.vertices[v].pos.z,
////		    source_mesh.vertices[v].nrm.x, source_mesh.vertices[v].nrm.y, source_mesh.vertices[v].nrm.z,
////		    source_mesh.vertices[v].bone_indexes.x, source_mesh.vertices[v].bone_indexes.y );
//	    debugPrintfEXT( "shd %d  %f %f %f  %f %f %f\n",
//			v,
//		    target_mesh.vertices[v].pos.x, target_mesh.vertices[v].pos.y, target_mesh.vertices[v].pos.z,
//		    target_mesh.vertices[v].nrm.x, target_mesh.vertices[v].nrm.y, target_mesh.vertices[v].nrm.z );
//	}
//	/*
//	debugPrintfEXT( "testcompute %d %f %f\n", gl_GlobalInvocationID.x, matricesBuffer.a, matricesBuffer.b );
//	debugPrintfEXT( " 0 %f  %f %f %f %f\n",
//			matricesBuffer.matrices[ 0 ][0][0]
//		,matricesBuffer.matrices[ 0 ][3][0]
//		,matricesBuffer.matrices[ 0 ][3][1]
//		,matricesBuffer.matrices[ 0 ][3][2]
//		,matricesBuffer.matrices[ 0 ][3][3]
//		);
//	debugPrintfEXT( " 1 %f  %f %f %f %f\n",
//			matricesBuffer.matrices[ 1 ][0][0]
//		,matricesBuffer.matrices[ 1 ][3][0]
//		,matricesBuffer.matrices[ 1 ][3][1]
//		,matricesBuffer.matrices[ 1 ][3][2]
//		,matricesBuffer.matrices[ 1 ][3][3]
//		);
//	mat4 m = matricesBuffer.matrices[ 0 ];
//	debugPrintfEXT( " 0 %f  %f %f %f %f\n"
//			,m[0][0], m[3][0], m[3][1], m[3][2], m[3][3]
//		);
//	m = matricesBuffer.matrices[ 1 ];
//		debugPrintfEXT( " 1 %f  %f %f %f %f\n"
//				,m[0][0], m[3][0], m[3][1], m[3][2], m[3][3]
//			);
//		*/
//    }
    VertexSkin source_vertex = source_mesh.vertices[ gl_GlobalInvocationID.x ];
    VertexPNT target_vertex;
    target_vertex.pos = vec3( 0, 0, 0 );
    target_vertex.nrm = vec3( 0, 0, 0 );
    target_vertex.txt = source_vertex.txt;
    for( int bi = 0; bi < 3; bi++ ){
		int bone_index = source_vertex.bone_indexes[ bi ];
		mat4 mat = matricesBuffer.matrices[ bone_index ];
		target_vertex.pos += ( mat * vec4( source_vertex.pos, 1 ) ).xyz * source_vertex.bone_weights[ bi ];
		target_vertex.nrm += ( mat * vec4( source_vertex.nrm, 0 ) ).xyz * source_vertex.bone_weights[ bi ];
		//target_vertex->normal   += sm.mapNormal( Vec3( source_vertex->normal   ) ).asGlm() * source_vertex->bone_weights[ bi ];
    }
    //target_mesh.vertices[ gl_GlobalInvocationID.x ] = target_vertex;;
    target_mesh.vertices[ gl_GlobalInvocationID.x ].pos = target_vertex.pos;
    target_mesh.vertices[ gl_GlobalInvocationID.x ].nrm = normalize( target_vertex.nrm );
    //target_mesh.vertices[ gl_GlobalInvocationID.x ].nrm = source_vertex.nrm;
    //target_mesh.vertices[ gl_GlobalInvocationID.x ].pos = source_vertex.pos;
    target_mesh.vertices[ gl_GlobalInvocationID.x ].txt = target_vertex.txt;
}
/*

l ayout (local_size_x = WORKGROUP_SIZE, local_size_y = WORKGROUP_SIZE, local_size_z = 1 ) in;

struct Pixel{
  vec4 value;
};

layout(std140, binding = 0) buffer buf
{
   Pixel imageData[];
};

void main() {

  /*
  In order to fit the work into workgroups, some unnecessary threads are launched.
  We terminate those threads here. 
  * /
  if(gl_GlobalInvocationID.x >= WIDTH || gl_GlobalInvocationID.y >= HEIGHT)
    return;

  float x = float(gl_GlobalInvocationID.x) / float(WIDTH);
  float y = float(gl_GlobalInvocationID.y) / float(HEIGHT);

  /*
  What follows is code for rendering the mandelbrot set. 
  * /
  vec2 uv = vec2(x,y);
  float n = 0.0;
  vec2 c = vec2(-.445, 0.0) +  (uv - 0.5)*(2.0+ 1.7*0.2  ), 
  z = vec2(0.0);
  const int M =128;
  for (int i = 0; i<M; i++)
  {
    z = vec2(z.x*z.x - z.y*z.y, 2.*z.x*z.y) + c;
    if (dot(z, z) > 2) break;
    n++;
  }
          
  // we use a simple cosine palette to determine color:
  // http://iquilezles.org/www/articles/palettes/palettes.htm         
  float t = float(n) / float(M);
  vec3 d = vec3(0.3, 0.3 ,0.5);
  vec3 e = vec3(-0.2, -0.3 ,-0.5);
  vec3 f = vec3(2.1, 2.0, 3.0);
  vec3 g = vec3(0.0, 0.1, 0.0);
  vec4 color = vec4( d + e*cos( 6.28318*(f*t+g) ) ,1.0);
          
  // store the rendered mandelbrot set into a storage buffer:
  imageData[WIDTH * gl_GlobalInvocationID.y + gl_GlobalInvocationID.x].value = color;
}
*/
