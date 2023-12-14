#ifndef SIMPLE_MESH_HPP_C6B749D6_C83B_434C_9E58_F05FC27FEFC9
#define SIMPLE_MESH_HPP_C6B749D6_C83B_434C_9E58_F05FC27FEFC9

#include <glad.h>
#include <vector>
#include "../vmlib/vec3.hpp"
#include "../vmlib/vec2.hpp"

/* Declaring 3 functions, to create VAO's with and without textures, and one function to concatenate.
Two structs are also declared here for vectors */

struct SimpleMeshData
{
	std::vector<Vec3f> positions;
	std::vector<Vec3f> colors;
	std::vector<Vec3f> normals;
	std::vector<Vec2f> texcoords;
};

struct SimpleMeshDataNoTexture
{
	std::vector<Vec3f> positions;
	std::vector<Vec3f> colors;
	std::vector<Vec3f> normals;
};

SimpleMeshDataNoTexture concatenate( SimpleMeshDataNoTexture, SimpleMeshDataNoTexture const& );

GLuint create_vao( SimpleMeshData const& );

GLuint create_novaoTexture( SimpleMeshDataNoTexture const& aMeshData );


#endif // SIMPLE_MESH_HPP_C6B749D6_C83B_434C_9E58_F05FC27FEFC9
