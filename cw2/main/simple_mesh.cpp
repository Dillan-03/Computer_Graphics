#include "simple_mesh.hpp"

SimpleMeshData concatenate( SimpleMeshData aM, SimpleMeshData const& aN )
{
	aM.positions.insert( aM.positions.end(), aN.positions.begin(), aN.positions.end() );
	aM.colors.insert( aM.colors.end(), aN.colors.begin(), aN.colors.end() );
	aM.normals.insert( aM.normals.end(), aN.normals.begin(), aN.normals.end() );
	return aM;
}



GLuint create_vao( SimpleMeshData const& aMeshData )
{
	//TODO: implement me
	GLuint positionVBO = 0; 
	glGenBuffers( 1, &positionVBO ); 
	glBindBuffer( GL_ARRAY_BUFFER, positionVBO ); 
	glBufferData( GL_ARRAY_BUFFER, aMeshData.positions.size() * sizeof(Vec3f), aMeshData.positions.data(), GL_STATIC_DRAW );  

	GLuint colorVBO = 0; 
	glGenBuffers( 1, &colorVBO); 
	glBindBuffer( GL_ARRAY_BUFFER, colorVBO ); 
	glBufferData( GL_ARRAY_BUFFER, aMeshData.colors.size() * sizeof(Vec3f), aMeshData.colors.data(), GL_STATIC_DRAW ); 

	//Normals 
	GLuint normalVBO = 0; // New VBO for normals
    glGenBuffers(1, &normalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, aMeshData.normals.size() * sizeof(Vec3f), aMeshData.normals.data(), GL_STATIC_DRAW);

	//textures
	GLuint texturesVBO = 0; // New VBO for textures
    glGenBuffers(1, &texturesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texturesVBO);
    glBufferData(GL_ARRAY_BUFFER, aMeshData.texcoords.size() * sizeof(Vec2f), aMeshData.texcoords.data(), GL_STATIC_DRAW);

	GLuint vao = 0; 
	glGenVertexArrays( 1, &vao ); 
	glBindVertexArray( vao ); 


	glBindBuffer( GL_ARRAY_BUFFER, positionVBO ); 
	glVertexAttribPointer( 
	0, // location = 0 in vertex shader 
	3, GL_FLOAT, GL_FALSE, // 2 floats, not normalized to [0..1] (GL FALSE) 
	0, // stride = 0 indicates that there is no padding between inputs 
	0 // data starts at offset 0 in the VBO. 
	); 
	glEnableVertexAttribArray( 0 ); 

	glBindBuffer( GL_ARRAY_BUFFER, colorVBO ); 
	glVertexAttribPointer( 
	1, // location = 1 in vertex shader 
	3, GL_FLOAT, GL_FALSE, // 3 floats, not normalized to [0..1] (GL FALSE) 
	0, // see above 
	0 // see above 
	);
	glEnableVertexAttribArray( 1 ); 

	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0); // New attribute for normals
    glEnableVertexAttribArray(2);

	glBindBuffer( GL_ARRAY_BUFFER, texturesVBO );
	glVertexAttribPointer(
	3, // location = 3 in vertex shader
	2, GL_FLOAT, GL_FALSE, // 2 floats, not normalized to 0..1 (GL FALSE)
	0,
	nullptr
	);	
	glEnableVertexAttribArray(3);


	// Reset state
	glBindVertexArray( 0 ); 
	glBindBuffer( GL_ARRAY_BUFFER, 0 ); 

	// Clean up buffers 
	// Note: these are not deleted fully, as the VAO holds a reference to them. 
	glDeleteBuffers( 1, &colorVBO ); 
	glDeleteBuffers( 1, &positionVBO ); 
	glDeleteBuffers( 1, &normalVBO); // Delete normal VBO
	glDeleteBuffers( 1, &texturesVBO);



	return vao;
}