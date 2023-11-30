#include "simple_mesh.hpp"

// SimpleMeshData concatenate( SimpleMeshData aM, SimpleMeshData const& aN )
// {
// 	aM.positions.insert( aM.positions.end(), aN.positions.begin(), aN.positions.end() );
// 	//aM.normals.insert( aM.normals.end(), aN.normals.begin(), aN.normals.end() );
// 	aM.texcoords.insert( aM.texcoords.end(), aN.texcoords.begin(), aN.texcoords.end() );
// 	return aM;
// }


GLuint create_vao( SimpleMeshData const& aMeshData )
{
	//TODO: implement me
	GLuint positionsVBO = 0; 
	glGenBuffers( 1, &positionsVBO ); 
	glBindBuffer( GL_ARRAY_BUFFER, positionsVBO ); 
	glBufferData( GL_ARRAY_BUFFER, aMeshData.positions.size() * sizeof(Vec3f), aMeshData.positions.data(), GL_STATIC_DRAW );  

	//Texture VBO
	GLuint texCoordVBO = 0; 
	glGenBuffers( 1, &texCoordVBO); 
	glBindBuffer( GL_ARRAY_BUFFER, texCoordVBO ); 
	glBufferData( GL_ARRAY_BUFFER, aMeshData.texcoords.size() * sizeof(Vec2f), aMeshData.texcoords.data(), GL_STATIC_DRAW ); 

	//vao
	GLuint vao = 0; 
	glGenVertexArrays( 1, &vao ); 
	glBindVertexArray( vao ); 


	glBindBuffer( GL_ARRAY_BUFFER, positionsVBO ); 
	glVertexAttribPointer( 
	0, // location = 0 in vertex shader 
	3, GL_FLOAT, GL_FALSE, // 
	0, // stride = 0 indicates that there is no padding between inputs 
	0 // data starts at offset 0 in the VBO. 
	); 
	glEnableVertexAttribArray( 0 ); 


	glBindBuffer( GL_ARRAY_BUFFER, texCoordVBO );
	glVertexAttribPointer(
	1, // location = 1 in vertex shader
	2, GL_FLOAT, GL_FALSE, // 2 floats, not normalized to 0..1 (GL FALSE)
	0,
	nullptr
	);	
	glEnableVertexAttribArray(1);


	

	// Reset state
	glBindVertexArray( 0 ); 
	glBindBuffer( GL_ARRAY_BUFFER, 0 ); 

	// Clean up buffers 
	glDeleteBuffers( 1, &positionsVBO ); 
	glDeleteBuffers(1, &texCoordVBO); // Delete normal VBO



	return vao;

	
	
}

