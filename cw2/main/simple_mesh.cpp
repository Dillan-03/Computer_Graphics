#include "simple_mesh.hpp"

/* This file utilizes 3 functions. For creating VAOs, and for concatenating  
1. concatenate: to create the space vehicle by concatenating shapes
2. create_vao: set up necessary OpenGL state and resources to render these meshes efficiently  
3. create_novaoTexture same as 2, just without textures */

// This takes in two SimpleMeshDataNoTexture objects (cone, cylinder, or cube)
SimpleMeshDataNoTexture concatenate( SimpleMeshDataNoTexture aM, SimpleMeshDataNoTexture const& aN )
{
	aM.positions.insert( aM.positions.end(), aN.positions.begin(), aN.positions.end() );
	aM.colors.insert( aM.colors.end(), aN.colors.begin(), aN.colors.end() );
	aM.normals.insert( aM.normals.end(), aN.normals.begin(), aN.normals.end() );
	return aM;
}


GLuint create_vao( SimpleMeshData const& aMeshData )
{
	// Generates and binds a buffer for vertex positions
	// Copies position data into buffer
	GLuint positionVBO = 0; 
	glGenBuffers( 1, &positionVBO ); 
	glBindBuffer( GL_ARRAY_BUFFER, positionVBO ); 
	glBufferData( GL_ARRAY_BUFFER, aMeshData.positions.size() * sizeof(Vec3f), aMeshData.positions.data(), GL_STATIC_DRAW );  

	// Does the same for colours
	GLuint colorVBO = 0; 
	glGenBuffers( 1, &colorVBO); 
	glBindBuffer( GL_ARRAY_BUFFER, colorVBO ); 
	glBufferData( GL_ARRAY_BUFFER, aMeshData.colors.size() * sizeof(Vec3f), aMeshData.colors.data(), GL_STATIC_DRAW ); 

	// Does the same for normals
	GLuint normalVBO = 0; 
    glGenBuffers(1, &normalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, aMeshData.normals.size() * sizeof(Vec3f), aMeshData.normals.data(), GL_STATIC_DRAW);

	// Does the same for textures
	GLuint texturesVBO = 0; 
    glGenBuffers(1, &texturesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texturesVBO);
    glBufferData(GL_ARRAY_BUFFER, aMeshData.texcoords.size() * sizeof(Vec2f), aMeshData.texcoords.data(), GL_STATIC_DRAW);

	// Generates and binds a VAO
	GLuint vao = 0; 
	glGenVertexArrays( 1, &vao ); 
	glBindVertexArray( vao ); 

	// Set up vertex attribute pointers for positions, colours, normals, and textures in VAO
	glBindBuffer( GL_ARRAY_BUFFER, positionVBO ); 
	glVertexAttribPointer( 
	0, // Location = 0 in vertex shader 
	3, GL_FLOAT, GL_FALSE, // 2 floats, not normalized to [0..1] (GL FALSE) 
	0, // Stride = 0 indicates that there is no padding between inputs 
	0 // Data starts at offset 0 in the VBO. 
	); 
	glEnableVertexAttribArray( 0 ); 

	glBindBuffer( GL_ARRAY_BUFFER, colorVBO ); 
	glVertexAttribPointer( 
	1, // Location = 1 in vertex shader 
	3, GL_FLOAT, GL_FALSE, // 3 floats, not normalized to [0..1] (GL FALSE) 
	0, // Same as above 
	0 // Same as above 
	);
	glEnableVertexAttribArray( 1 ); 

	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glVertexAttribPointer(
	2, // Location = 2 in vertex shader
	3, GL_FLOAT, GL_FALSE, // 3 floats, not normalized to [0..1] (GL FALSE)
	0, // Same as above
	0  // Same as above
	); 
    glEnableVertexAttribArray(2);

	glBindBuffer( GL_ARRAY_BUFFER, texturesVBO );
	glVertexAttribPointer(
	3, // Location = 3 in vertex shader
	2, GL_FLOAT, GL_FALSE, // 2 floats, not normalized to 0..1 (GL FALSE)
	0,
	nullptr
	);	
	glEnableVertexAttribArray(3);

	// Reset state (unbinds VAO and VBO)
	glBindVertexArray( 0 ); 
	glBindBuffer( GL_ARRAY_BUFFER, 0 ); 

	// Clean up buffers 
	// Note: these are not deleted fully, as the VAO holds a reference to them. 
	glDeleteBuffers( 1, &colorVBO ); 
	glDeleteBuffers( 1, &positionVBO ); 
	glDeleteBuffers( 1, &normalVBO); 
	glDeleteBuffers( 1, &texturesVBO);

	return vao;
}

// Vao for no Textures
// Works in the same way as above but without texture information 
GLuint create_novaoTexture( SimpleMeshDataNoTexture const& aMeshData )
{
	GLuint positionVBO = 0; 
	glGenBuffers( 1, &positionVBO ); 
	glBindBuffer( GL_ARRAY_BUFFER, positionVBO ); 
	glBufferData( GL_ARRAY_BUFFER, aMeshData.positions.size() * sizeof(Vec3f), aMeshData.positions.data(), GL_STATIC_DRAW );  

	GLuint colorVBO = 0; 
	glGenBuffers( 1, &colorVBO); 
	glBindBuffer( GL_ARRAY_BUFFER, colorVBO ); 
	glBufferData( GL_ARRAY_BUFFER, aMeshData.colors.size() * sizeof(Vec3f), aMeshData.colors.data(), GL_STATIC_DRAW ); 

	GLuint normalVBO = 0; 
    glGenBuffers(1, &normalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, aMeshData.normals.size() * sizeof(Vec3f), aMeshData.normals.data(), GL_STATIC_DRAW);

	GLuint vao = 0; 
	glGenVertexArrays( 1, &vao ); 
	glBindVertexArray( vao ); 


	glBindBuffer( GL_ARRAY_BUFFER, positionVBO ); 
	glVertexAttribPointer( 
	0,
	3, GL_FLOAT, GL_FALSE, 
	0, 
	0 
	); 
	glEnableVertexAttribArray( 0 ); 

	glBindBuffer( GL_ARRAY_BUFFER, colorVBO ); 
	glVertexAttribPointer( 
	1, 
	3, GL_FLOAT, GL_FALSE, 
	0, 
	0 
	);
	glEnableVertexAttribArray( 1 ); 

	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(2);

	glBindVertexArray( 0 ); 
	glBindBuffer( GL_ARRAY_BUFFER, 0 ); 

	glDeleteBuffers( 1, &colorVBO ); 
	glDeleteBuffers( 1, &positionVBO ); 
	glDeleteBuffers( 1, &normalVBO); 

	return vao;
}